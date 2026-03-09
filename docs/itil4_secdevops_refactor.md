# ITIL 4 + SecDevOps Refactor Notes

This repository now follows common software delivery terminology and directory conventions:

- `src/`: production Python modules.
- `tests/`: automated quality gates.
- `docs/`: design knowledge, analysis notebooks, and artifacts.
- `data/`: data landing zones (e.g., `raw`, `processed`, `curated`).
- `ci/`: pipeline-as-code mirrors for CI/CD governance visibility.

## ITIL 4 alignment

- **Service Value Chain / Design & Transition**: Clear separation of source code, documentation, and test assets supports controlled transition from experimentation to reusable services.
- **Change Enablement**: Naming normalization and packaging improve auditability for change records.
- **Continual Improvement**: Test scaffolding in `tests/` supports iterative quality improvements.

## SecDevOps vocabulary adopted

- **Shift-left quality controls** through unit tests.
- **Pipeline-as-code** captured in `ci/github-actions/`.
- **Data quality controls** represented by the `outlier_cleaner` utility.
- **Secure coding modernization** by removing Python 2-only string translation patterns.
