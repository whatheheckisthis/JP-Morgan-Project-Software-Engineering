# Data Directory Guide

This repository keeps runtime datasets outside version control when they are large or sensitive.

## Structure

- `raw/enron/` — immutable source drops (emails, metadata, original extracts)
- `processed/` — cleaned, normalized, and feature-engineered intermediates
- `curated/` — analysis-ready slices and exports used by modeling and governance output

## Production-grade files included

1. `data/raw/enron/production_email_inputs.csv`
2. `data/raw/enron/production_financial_inputs.csv`
3. `data/processed/production_feature_matrix.csv`
4. `data/curated/production_risk_profiles.csv`

## Notes

- Keep personally sensitive data out of Git history.
- Prefer checksums and reproducible scripts for data lineage.
- Use synthetic fixtures in tests instead of full corporate datasets.


## JPM case-management corpus

- Enterprise artifact ingestion location: `data/raw/jp_morgan_case_management/`.
- Required artifact contract: `data/raw/jp_morgan_case_management/artifact_manifest.csv`.
- Architecture detail: `docs/architecture/wiser_architecture.md`.
