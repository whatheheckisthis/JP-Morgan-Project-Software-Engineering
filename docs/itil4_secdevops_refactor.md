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


## Control mapping baseline (SOC 2, ISM, Essential Eight ML4)

To make governance intent actionable, the repository's core capabilities are mapped to common control domains.

> Spec-number notation is included for SOC 2 and Essential Eight strategy indexing. For ISM, map to your organization’s selected ISM control IDs in the current ISM release and profile.

| Capability | SOC 2 spec numbers | ISM spec reference | Essential Eight spec reference (ML4) |
|---|---|---|---|
| Data quality and outlier cleaning (`src/analytics/outliers`) | **PI1.1-PI1.3**, **CC7.2** | ISM Integrity + Logging control IDs (from your ISM control register). | **M3**, **M4**, **M8** at ML4. |
| Email parsing and text normalization (`src/nlp`) | **CC6.1-CC6.3**, **C1.1**, **CC7.1** | ISM Access Control + Data Protection control IDs (from your ISM control register). | **M1**, **M5**, **M6** at ML4. |
| Test scaffolding and CI definitions (`tests/`, `ci/github-actions`) | **CC5.2, CC5.3, CC7.3, CC7.4, CC8.1** | ISM Secure Development + Monitoring + Secure Configuration control IDs. | **M2**, **M4**, **M5**, **M3** at ML4. |
| Documentation and traceability (`docs/`) | **CC2.3**, **CC8.1** | ISM Governance + Audit Evidence control IDs. | **M8** (+ hardening controls as applicable) at ML4. |

### Practical implementation notes

- Treat this matrix as a **control intent map** that links engineering assets to audit themes.
- Maintain evidence by keeping tests green, CI definitions versioned, and documentation current.
- Record explicit ISM control IDs (e.g., your chosen ISM baseline IDs) in your GRC register to complete the traceability chain.
- If formal compliance assessment is required, map these domains to organization-specific control IDs and evidence registers.
