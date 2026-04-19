# EICVS Codex Prompt

## Purpose
EICVS is a fixture-bounded validation engine that executes structured vulnerability-class vectors against deterministic control logic and emits cryptographically chained evidence records.

## Inputs
- `tests/fixtures/<class>_vectors.json`: versioned fixtures only.
- Static internal mapping to OWASP ASVS 4.0.3 and OWASP Top 10 2021.
- No external state fetching; loopback-only network policy.

## Outputs
- Primary: append-only `EvidenceRecord` appended to shared ledger.
- Secondary: `evidence/evidence_gap_register.csv` for human governance.

## Invariants
- Exactly one evidence record per execution.
- Result states limited to `PASS`, `FAIL`, `ERROR`.
- No advisory/remediation outputs.
- No integration with live systems or runtime external dependencies.
