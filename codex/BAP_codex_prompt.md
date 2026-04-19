# BAP Codex Prompt

## Purpose
BAP is a feature-engineering and anomaly-detection pipeline over at-rest communication and financial corpora that produces risk profiles, alert candidates, and governance reports.

## Inputs
- `data/sources/enron/` and `data/sources/financial/` as batch corpora.
- Inputs are immutable per run.

## Outputs
- `outputs/<run_id>/risk_profiles.<run_id>.csv`
- `outputs/<run_id>/alert_candidates.<run_id>.csv`
- `outputs/<run_id>/governance_report.<run_id>.json`
- One ledger evidence record per run, appended after output emission.

## Invariants
- No streaming/live ingestion.
- No read dependency on EICVS outputs.
- No advisory output; scores and alerts only.
