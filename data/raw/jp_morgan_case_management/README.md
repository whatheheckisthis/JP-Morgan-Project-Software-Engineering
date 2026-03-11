# JPM Case Management Raw Artifacts

This folder is reserved for enterprise corpus ingestion from JPM case-management systems.

## Required files

Use `artifact_manifest.csv` as the minimum contract for what must be fetched before running enterprise-mode analytics.

## Placement rules

- Keep original exports immutable in this directory.
- Do not overwrite source files during normalization.
- Maintain checksum and extraction metadata per file where available.
