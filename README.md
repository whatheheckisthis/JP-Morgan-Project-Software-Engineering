# Assurance Engineering Portfolio
## Deterministic Control Validation and Behavioural Analytics — v1.0.0

---

## Overview

This repository contains two formally specified, implementation-ready engineering systems developed as part of an independent assurance practice. Both systems are designed for auditability, deterministic execution, and direct translation into software components without requiring design inference.

The first system — the **Exploit-Informed Control Validation System (EICVS)** — is a structured vulnerability class evaluation engine that maps known exploit patterns to OWASP control frameworks and emits cryptographically chained evidence records. The second — the **Behavioural Analytics Pipeline (BAP)** — is a multi-source analytics pipeline that ingests email and financial data, engineers behavioural features, applies dimensionality reduction and anomaly detection, and produces governance-ready audit outputs.

Both systems share the same foundational design commitments: bounded execution, no network access beyond what is strictly declared, append-only or write-once output artefacts, fail-fast error semantics, and full determinism given identical inputs. Neither system remediates, advises, or infers. Both evaluate, map, and record.

---

## Repository Structure

```
.
├── README.md
│
├── eicvs/                              # Exploit-Informed Control Validation System
│   ├── proto/                          # Protobuf trace schemas per vulnerability class
│   ├── services/trace/                 # Fixture targets and trace capture modules
│   ├── tests/
│   │   ├── fixtures/                   # Static versioned test vectors (JSON)
│   │   └── asvs/                       # Control assertion test suites
│   ├── evidence/
│   │   ├── evidence_ledger.py          # Append-only SHA-256-chained ledger
│   │   └── evidence_gap_register.csv   # Control coverage gap register
│   ├── reports/                        # JUnit XML control matrix outputs (generated)
│   ├── sdlc/                           # Threat models, control designs, test plans
│   └── Makefile
│
├── bap/                                # Behavioural Analytics Pipeline
│   ├── data/
│   │   ├── sources/enron/              # Pre-staged Enron corpus (read-only)
│   │   ├── sources/financial/          # Pre-staged financial records (read-only)
│   │   └── entity_map.csv              # Entity resolution table
│   ├── src/
│   │   ├── pipeline.py                 # Stage orchestrator
│   │   ├── ingestor.py
│   │   ├── normaliser.py
│   │   ├── feature_engineer.py
│   │   ├── dimensionality_reducer.py
│   │   ├── modelling_engine.py
│   │   ├── validation_controller.py
│   │   ├── output_emitter.py
│   │   └── exceptions.py
│   ├── tests/
│   │   ├── test_pipeline.py
│   │   └── fixtures/                   # Minimal valid test inputs
│   └── outputs/                        # Run-scoped output directories (generated)
│
├── codex/
│   ├── EICVS_codex_prompt.md           # Formal implementation contract for EICVS
│   └── BAP_codex_prompt.md             # Formal implementation contract for BAP
│
└── docs/
    └── architecture.md
```

---

## System One — Exploit-Informed Control Validation System (EICVS)

### What It Is

EICVS is a deterministic, fixture-bounded, multi-class control validation architecture. It accepts structured representations of known vulnerability classes as typed test vectors, evaluates them against explicit rule sets, maps detected violations to OWASP ASVS 4.0.3 and OWASP Top 10 2021 controls, and emits append-only, SHA-256-chained evidence records. Every state transition is declared, typed, and auditable.

The system does not execute exploit code. It does not scan live systems. It does not connect to any external service. It does not remediate. It evaluates, maps, and records.

### Vulnerability Classes

| Class | OWASP Top 10 | ASVS Chapter |
|---|---|---|
| `auth` | A07:2021 | V2 |
| `ssrf` | A10:2021 | V10 |
| `deserialization` | A08:2021 | V5 |
| `supply_chain` | A08:2021 | V10 |
| `iam` | A01:2021 | V4 |

### Pipeline

Each vulnerability class traverses a six-stage pipeline. Stages are not skippable. Failure at any stage halts the pipeline for that invocation with no fallback.

```
Stage 0   VECTOR_INGESTION          JSON test vector → validated typed request object
Stage 1   FIXTURE_DISPATCH          Request → deterministic analysis object
Stage 2   TRACE_CAPTURE             Request + analysis → typed trace record
Stage 3   CONTROL_MAPPING           Analysis → ControlTrigger list
Stage 4   DETERMINISTIC_VERIFICATION Trace + vectors → pytest result set + JUnit XML
Stage 5   EVIDENCE_EMISSION         Trace + result → appended EvidenceRecord
```

### Evidence Ledger

Every evaluation emits exactly one `EvidenceRecord` to `evidence/evidence_ledger.csv`. The ledger is append-only. No record may be modified or deleted after write. Each record's `record_id` is a SHA-256 function of `(previous_record_id, input_fp, timestamp_ns)`, forming a verifiable chain across all pipeline classes and all invocations.

```
record_id = SHA-256(previous_record_id || input_fp || timestamp_ns)
```

Chain integrity is verifiable at any time without the original input vectors:

```
make verify-chain
```

### Control Mapping

Control mappings are not looked up at runtime and not loaded from configuration. They are encoded as explicit conditional branches in each fixture's `_map_controls()` method. Every violation-indicating analysis field maps to one or more `ControlTrigger` records, each carrying an exact ASVS control ID and an exact OWASP Top 10 item. An unmapped field is a hard pipeline failure, not a silent omission.

### Running

```bash
# Run a single class
make auth

# Run all classes
make all

# Verify ledger chain integrity
make verify-chain

# Check for blocking coverage gaps
make audit-gate

# Report all open gaps
make gap-check
```

### Adding a New Vulnerability Class

A class is not deployable until all of the following are committed:

- `proto/<class_id>_trace.proto` with all required message types
- `tests/fixtures/<class_id>_vectors.json` with `positive[]`, `negative[]`, and `edge[]` vectors
- `services/trace/fixture_<class_id>_target.py` with `_map_controls()` fully enumerated
- `services/trace/capture_<class_id>.py`
- `tests/asvs/test_<class_id>_controls.py` with all required test functions
- All unmapped controls registered in `evidence/evidence_gap_register.csv`
- `sdlc/<class_id>/threat_model.md`, `control_design.md`, `test_plan.md`
- Makefile target for `<class_id>`

Partial class definitions are not deployable. The CI audit gate enforces this via gap register checks.

### Key Constraints

- Network binding is restricted to `127.0.0.1`. Any socket opened to an external address is a security defect, not a configuration error.
- Payload field values are never executed as code, passed to `eval()` or `exec()`, or deserialised into a live object graph.
- All evaluation functions are pure. Identical input produces identical output on every invocation.
- Test vectors are static, version-controlled JSON files. No vector may be generated at runtime.
- `xfail` markers are prohibited on control assertion tests. Fail-fast is enforced at both the pipeline and test layers.

---

## System Two — Behavioural Analytics Pipeline (BAP)

### What It Is

BAP is a multi-stage behavioural analytics pipeline that ingests heterogeneous source data — Enron email corpus and financial transaction records — normalises it through schema alignment and type enforcement, derives NLP and financial features per entity, applies PCA-based dimensionality reduction for variance isolation and collinearity elimination, and routes the reduced feature space through anomaly detection and behaviour segmentation models to produce risk profiles, alert candidates, and governance-ready audit reports. A validation controller operates as a parallel control layer across the modelling stage.

The pipeline does not connect to external systems. It does not fetch data at runtime. It does not produce remediation advice. It does not run persistently. Each invocation is bounded and terminates.

### Pipeline

The pipeline is a seven-stage finite state machine. Stages are not skippable. Failure at any stage halts the pipeline for that invocation.

```
Stage 0   SOURCE_INGESTION          Raw files → typed dataframes
Stage 1   NORMALISATION             Dataframes → schema-aligned, typed, derived-field dataframes
Stage 2   FEATURE_ENGINEERING       Normalised data → FeatureMatrix (one row per entity)
Stage 3   DIMENSIONALITY_REDUCTION  FeatureMatrix → ReducedFeatureMatrix + PCAMetadata
Stage 4   MODELLING_AND_SCORING     ReducedFeatureMatrix → ScoredEntityFrame
Stage 5   VALIDATION                All intermediate outputs → ValidationResult + logs
Stage 6   OUTPUT_EMISSION           ScoredEntityFrame + ValidationResult → five output files
```

### Input Sources

| Source | Format | Path |
|---|---|---|
| Enron email corpus | CSV / structured export | `data/sources/enron/` |
| Financial transaction records | CSV / typed tabular | `data/sources/financial/` |
| Entity resolution map | CSV | `data/entity_map.csv` |

All input data is pre-staged. No runtime data fetch is permitted. Source files are opened in read-only mode. No source file is modified by the pipeline.

### Feature Engineering

NLP features and financial features are derived per `entity_id` and assembled into a typed `FeatureMatrix`. All null values are resolved to typed zero before the matrix is passed downstream. No `NaN` value is permitted in the `FeatureMatrix` output.

NLP features include email volume, average recipient count, after-hours send ratio, subject entropy, body word count, unique recipient ratio, and keyword watchlist hit count.

Financial features include transaction volume, absolute transaction total, average transaction amount, amount standard deviation, large transaction ratio, unique counterparty count, and debit-to-credit ratio.

All feature derivation constants — keyword watchlist, business hours range, large transaction threshold, rolling window — are declared as module-level `Final` values. Runtime modification is prohibited.

### Dimensionality Reduction

PCA is applied after collinearity removal and standard scaling. Zero-variance columns are dropped before scaling. Columns with pairwise Pearson correlation above `COLLINEARITY_THRESHOLD` are reduced by retaining the first column in each correlated pair. `PCA_RANDOM_STATE` is a declared constant, ensuring reproducibility across invocations.

All dropped columns and PCA parameters are recorded in a `PCAMetadata` record that is carried through to the governance report.

### Modelling

Two models are applied to the reduced feature space:

**Anomaly Detection** uses Isolation Forest. Each entity receives an `anomaly_score` (raw decision function output) and an `anomaly_flag` (boolean). All hyperparameters — `IF_N_ESTIMATORS`, `IF_MAX_SAMPLES`, `IF_CONTAMINATION`, `IF_RANDOM_STATE` — are declared `Final` constants.

**Behaviour Segmentation** uses K-Means clustering. Each entity receives a `segment_label` (integer cluster assignment). All hyperparameters — `KM_N_CLUSTERS`, `KM_MAX_ITER`, `KM_N_INIT`, `KM_RANDOM_STATE` — are declared `Final` constants.

Model artefacts are not persisted to the filesystem. The scored output is the deliverable.

### Validation

The validation controller runs eight checks against all intermediate and final outputs before emission. Seven are halting: a failed check raises `ValidationError` and exits the pipeline non-zero. One is advisory: a warning is written to `validation_log` and execution continues.

| Check | Type | Assertion |
|---|---|---|
| CHECK-VAL-01 | Halting | Every entity in FeatureMatrix is present in ScoredEntityFrame |
| CHECK-VAL-02 | Halting | All anomaly scores are finite float64 |
| CHECK-VAL-03 | Halting | All segment labels are in `[0, KM_N_CLUSTERS - 1]` |
| CHECK-VAL-04 | Halting | `anomaly_flag` column contains only boolean values |
| CHECK-VAL-05 | Halting | PCA cumulative variance ≥ `PCA_MIN_CUMULATIVE_VARIANCE` |
| CHECK-VAL-06 | Halting | FeatureMatrix contains no NaN (independent re-assertion) |
| CHECK-VAL-07 | Advisory | Anomaly rate is within declared bounds |
| CHECK-VAL-08 | Halting | Parameter log contains entries for all declared constants |

### Outputs

All outputs are written to `outputs/<run_id>/`. The `run_id` is computed once at pipeline start as `YYYYMMDD_HHMMSS_<sha256_of_inputs[:8]>` and is used across all output filenames. No output file is opened in append mode. Prior run directories are not modified.

| File | Content |
|---|---|
| `risk_profiles.<run_id>.csv` | Scored record for every entity |
| `alert_candidates.<run_id>.csv` | Subset where `anomaly_flag == True`, ordered by `abs(anomaly_score)` descending |
| `governance_report.<run_id>.json` | Run metadata, PCA parameters, validation summary, model version |
| `validation_log.<run_id>.json` | Per-check results and messages |
| `parameter_log.<run_id>.json` | All declared constants serialised at run time |

### Running

```bash
# Run the full pipeline
make run

# Run the test suite
make test

# Remove generated outputs (does not touch data/ or src/)
make clean
```

### Key Constraints

- No network socket is opened at any stage under any condition.
- No ingested field value is passed to `eval()`, `exec()`, `pickle.loads()`, or any equivalent.
- All module-level `Final` constants are set at module load time. Runtime modification is prohibited.
- No `try/except` block in validation code suppresses a `ValidationError`.
- Two independent runs with identical input produce output files with identical feature values, anomaly scores, segment labels, and check results. `run_id`, `timestamp_utc`, and derived filenames will differ.

---

## Design Commitments (Both Systems)

Both systems are built on the same set of non-negotiable design properties.

**Determinism.** Given identical inputs and identical declared constants, both systems produce identical outputs. No evaluation logic depends on wall-clock time, random state, environment variables, or external system responses.

**Fail-fast.** Any condition that cannot be handled by an explicitly declared error path halts the pipeline immediately. Silent error suppression — bare `except`, catch-all fallback, default return on exception — is prohibited in all forms across both systems.

**No implicit behaviour.** No module takes any action not declared in its specification. This includes logging (named logger only; not stdout), file writes, and state mutations.

**Bounded output scope.** All filesystem writes during execution are scoped to declared output paths. No write occurs outside those paths.

**No autonomous operation.** Neither system schedules itself, responds to events, or takes any action without being explicitly invoked.

**Reproducibility.** Any output produced by either system can be reproduced from the same input, yielding records with identical content fingerprints and identical control or model evaluation results.

---

## Codex Implementation Contracts

Formal implementation contracts for both systems are provided in `codex/`. Each contract defines the full system as a set of bounded modules with explicit inputs, outputs, function contracts, data schemas, error conditions, and global invariants. They are written for direct translation into software components without requiring interpretation or design inference.

| Contract | Path |
|---|---|
| EICVS | `codex/EICVS_codex_prompt.md` |
| BAP | `codex/BAP_codex_prompt.md` |

---

## Versioning

| Artefact | Version |
|---|---|
| OWASP ASVS (EICVS) | 4.0.3 |
| OWASP Top 10 (EICVS) | 2021 |
| EICVS schema | 1.0.0 |
| BAP schema | 1.0.0 |

Version constants are declared as module-level `Final[str]` values in each module. Any change to a version constant requires updated mapping verification, a new gap register entry for newly introduced controls, and a commit message referencing the version change.

---

## Status

Both systems are in active specification. Implementation targets are sequenced against formal accreditation milestones. The EICVS codex contract is complete. The BAP codex contract is complete. Stub constant population for BAP is pending domain value decisions (`KEYWORD_WATCHLIST`, `LARGE_TXN_THRESHOLD`, `COLLINEARITY_THRESHOLD`, `IF_CONTAMINATION`, `KM_N_CLUSTERS`).
