#!/usr/bin/env python3
"""Workflow diagnostic helper for GitHub Actions.

This script is meant to be run by interactive agents to quickly spot
common CI/workflow issues without executing the pipelines. It focuses on
structural problems such as mismatched artifact names, missing job
references, or actions that are not pinned to a version.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, Iterable, List, Sequence, Set, Tuple

try:
    import yaml  # type: ignore
except ImportError:  # pragma: no cover - optional dependency
    yaml = None


class WorkflowDoctorError(RuntimeError):
    """Raised when workflows cannot be inspected."""


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Analyze GitHub Actions workflows for structural issues such as "
            "missing job references or mismatched artifact names."
        )
    )
    parser.add_argument(
        "paths",
        nargs="*",
        default=[Path(".github/workflows")],
        type=Path,
        help="Workflow files or directories to inspect (defaults to .github/workflows)",
    )
    parser.add_argument(
        "--strict",
        action="store_true",
        help=(
            "Treat warnings as errors (non-zero exit code) to integrate with "
            "other automation."
        ),
    )
    return parser.parse_args(argv)


def load_workflow(path: Path) -> Dict[str, Any]:
    text = path.read_text(encoding="utf-8")
    if yaml is not None:
        try:
            return yaml.safe_load(text) or {}
        except Exception as exc:  # pragma: no cover - yaml parser errors
            raise WorkflowDoctorError(f"Failed to parse {path}: {exc}") from exc
    # Fallback: attempt JSON parse so the tool still produces a useful
    # message when PyYAML is unavailable.
    try:
        return json.loads(text)
    except json.JSONDecodeError as exc:
        hint = (
            "PyYAML is required to parse this workflow. Install it with "
            "`python -m pip install pyyaml` or run the tool in an environment "
            "that provides it."
        )
        raise WorkflowDoctorError(f"Failed to parse {path}: {exc}\n{hint}") from exc


def iter_workflow_files(paths: Iterable[Path]) -> Iterable[Path]:
    for base in paths:
        if base.is_dir():
            yield from sorted(base.glob("*.yml"))
            yield from sorted(base.glob("*.yaml"))
        elif base.is_file():
            yield base


def find_artifacts(steps: Sequence[Dict[str, Any]]) -> Tuple[Set[str], Set[str]]:
    uploads: Set[str] = set()
    downloads: Set[str] = set()
    for step in steps:
        if not isinstance(step, dict):
            continue
        uses = step.get("uses")
        with_block = step.get("with", {}) if isinstance(step.get("with"), dict) else {}
        if isinstance(uses, str):
            if "upload-artifact" in uses:
                name = with_block.get("name")
                if name:
                    uploads.add(str(name))
            if "download-artifact" in uses:
                name = with_block.get("name")
                if name:
                    downloads.add(str(name))
    return uploads, downloads


def find_unpinned_actions(steps: Sequence[Dict[str, Any]]) -> List[str]:
    warnings: List[str] = []
    for step in steps:
        if not isinstance(step, dict):
            continue
        uses = step.get("uses")
        if isinstance(uses, str) and "@" not in uses:
            warnings.append(uses)
    return warnings


def job_has_actions_write(job: Dict[str, Any]) -> bool:
    perms = job.get("permissions")
    if not isinstance(perms, dict):
        return False
    actions = perms.get("actions")
    return isinstance(actions, str) and actions.lower() == "write"


def analyze_workflow(path: Path) -> Tuple[List[str], List[str]]:
    data = load_workflow(path)
    warnings: List[str] = []
    notes: List[str] = []

    jobs = data.get("jobs", {}) if isinstance(data, dict) else {}
    job_names = set(jobs.keys()) if isinstance(jobs, dict) else set()
    all_uploads: Set[str] = set()
    all_downloads: Set[str] = set()

    for job_name, job in jobs.items():
        if not isinstance(job, dict):
            warnings.append(f"Job {job_name!r} is not a mapping and cannot be analyzed.")
            continue

        if "runs-on" not in job:
            warnings.append(f"Job {job_name} is missing 'runs-on' and may not schedule correctly.")

        needs = job.get("needs")
        if isinstance(needs, str):
            needs = [needs]
        if isinstance(needs, list):
            missing = [n for n in needs if n not in job_names]
            if missing:
                warnings.append(
                    f"Job {job_name} depends on missing jobs: {', '.join(sorted(missing))}."
                )

        steps = job.get("steps")
        if isinstance(steps, list):
            uploads, downloads = find_artifacts(steps)
            all_uploads.update(uploads)
            all_downloads.update(downloads)

            unpinned = find_unpinned_actions(steps)
            for action_ref in unpinned:
                warnings.append(
                    f"Job {job_name} uses an unpinned action reference: {action_ref}."
                )

            if downloads and not job_has_actions_write(job):
                notes.append(
                    f"Job {job_name} downloads artifacts but does not request "
                    "`actions: write` permissions; GitHub defaults may be sufficient, "
                    "but restricted repositories could block this."
                )
        else:
            warnings.append(f"Job {job_name} has no steps to analyze.")

    unmatched_uploads = sorted(all_uploads - all_downloads)
    unmatched_downloads = sorted(all_downloads - all_uploads)
    if unmatched_uploads:
        notes.append(
            "Artifacts uploaded but never downloaded in this workflow: "
            + ", ".join(unmatched_uploads)
        )
    if unmatched_downloads:
        warnings.append(
            "Artifacts downloaded without a corresponding upload: "
            + ", ".join(unmatched_downloads)
        )

    return warnings, notes


def main(argv: Sequence[str]) -> int:
    args = parse_args(argv)
    workflow_files = list(iter_workflow_files(args.paths))
    if not workflow_files:
        print("No workflow files found for analysis.")
        return 1

    overall_warnings: List[str] = []

    for wf in workflow_files:
        print(f"\n=== Analyzing {wf} ===")
        try:
            warnings, notes = analyze_workflow(wf)
        except WorkflowDoctorError as exc:
            print(f"Error: {exc}")
            overall_warnings.append(str(exc))
            continue

        if warnings:
            print("Potential issues:")
            for warning in warnings:
                print(f"  - {warning}")
                overall_warnings.append(warning)
        else:
            print("No structural warnings detected.")

        if notes:
            print("Notes:")
            for note in notes:
                print(f"  - {note}")

    if overall_warnings and args.strict:
        print("\nStrict mode: warnings detected.")
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
