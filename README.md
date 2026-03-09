# Enron Analytics Repository (ITIL 4 + SecDevOps Aligned)

This project contains exploratory analysis and utility code for the Enron email and finance dataset.

## Standardized Repository Structure

```text
src/        # application and reusable analysis logic
tests/      # automated checks
docs/       # notebooks, architecture and governance docs
data/       # data domains (raw/processed/curated)
ci/         # CI/CD pipeline definitions (governance mirror)
```

## Legacy-to-Modern Path Mapping

- Notebook assets were normalized into `docs/notebooks/`.
- Visualization assets moved to `docs/assets/`.
- `outlier_cleaner.py` moved to `src/analytics/outliers/outlier_cleaner.py`.
- `parse_out_email_text.py` moved to `src/nlp/email_parsing.py`.
- CI workflow mirrored at `ci/github-actions/python-app.yml`.

## Security and Operations Terminology

The refactor aligns naming to SecDevOps and ITIL 4 practices:

- Explicit separation of runtime code (`src`) and verification (`tests`).
- Clear governance surface for CI/CD controls (`ci`).
- Documented continual improvement path in `docs/itil4_secdevops_refactor.md`.

## Getting Started

1. Create and activate a Python environment.
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Run tests:
   ```bash
   pytest
   ```
