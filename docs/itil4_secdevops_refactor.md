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

| Capability | SOC 2 | ISM | Essential Eight ML4 |
|---|---|---|---|
| Data quality and outlier cleaning (`src/analytics/outliers`) | Processing Integrity (PI) and supporting Common Criteria for quality controls in data processing workflows. | Integrity-focused controls for trusted data handling and reliable analytics outcomes. | Restrict administrative privileges, patch applications, and regular backups to protect and recover analytic workflows. |
| Email parsing and text normalization (`src/nlp`) | Security and Confidentiality criteria for controlled handling of analytical text artifacts. | Access control and information protection controls for processed content. | Application control, MFA, and user application hardening to reduce misuse and unauthorized execution. |
| Test scaffolding and CI definitions (`tests/`, `ci/github-actions`) | Common Criteria for change management, monitoring, and auditable SDLC quality gates. | Secure development, verification, and monitoring controls for software assurance. | Patch applications/OS, MFA, and admin privilege restriction to harden build and validation pathways. |
| Documentation and traceability (`docs/`) | Common Criteria evidence support for policy/procedure traceability and audit readiness. | Governance and audit evidence management controls. | Regular backups and application hardening to retain and protect governance evidence artifacts. |

### Practical implementation notes

- Treat this matrix as a **control intent map** that links engineering assets to audit themes.
- Maintain evidence by keeping tests green, CI definitions versioned, and documentation current.
- If formal compliance assessment is required, map these domains to organization-specific control IDs and evidence registers.
