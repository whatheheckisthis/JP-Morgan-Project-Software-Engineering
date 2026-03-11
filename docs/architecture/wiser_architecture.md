# WISER Architecture (Exhaustive View)

This document provides an exhaustive architecture inventory for the repository and the extended corpus ingest path that includes Enron + JPM case-management artifacts.

## 1) End-to-end topology

1. **Corpus acquisition layer**
   - Enron raw email/financial feeds (`data/raw/enron/`).
   - JPM case-management artifacts (`data/raw/jp_morgan_case_management/`).
2. **Ingestion & normalization layer**
   - `src/pipeline/ingestion.c` loads CSV records.
   - `normalize_rows(...)` aligns entities by `employee_id`.
3. **Feature engineering layer**
   - `src/pipeline/feature_engineering.c` computes text metrics and financial pressure features.
4. **Dimensionality reduction layer**
   - `src/pipeline/pca.cpp` projects feature matrix into reduced dimensions.
5. **Modeling & scoring layer**
   - `src/pipeline/modeling.cpp` computes anomaly scores and risk bands.
6. **Validation & governance layer**
   - `src/pipeline/validation.cpp` emits run-level validation status.
   - Curated profiles are stored for downstream governance reporting.

## 2) Component inventory

### 2.1 Source components

- `pipeline.h`: canonical C interface and record contracts.
- `ingestion.c`: file read + schema parsing + alignment.
- `feature_engineering.c`: token, casing, punctuation, and financial ratio derivation.
- `pca.cpp` / `pca.hpp`: deterministic projection API.
- `modeling.cpp` / `modeling.hpp`: centroid-distance anomaly scoring + tiering.
- `validation.cpp` / `validation.hpp`: consistency checks and run summary struct.

### 2.2 Data contracts

- **Raw email input**
  - `employee_id`, `email_count`, `message_length`, `email_text`
- **Raw financial input**
  - `employee_id`, `salary`, `bonus`, `expenses`, `poi`
- **Processed feature matrix**
  - text metrics + volume features + financial pressure ratios
- **Curated risk profile**
  - `employee_id`, `anomaly_score`, `risk_band`

## 3) Corpus ingestion: Enron + JPM case-management artifacts

### 3.1 Enron corpus

- Checked-in production-grade starter files:
  - `data/raw/enron/production_email_inputs.csv`
  - `data/raw/enron/production_financial_inputs.csv`

### 3.2 JPM case-management corpus fetch (artifact contract)

The repository now includes an artifact manifest + storage location for JPM case-management ingestion:

- `data/raw/jp_morgan_case_management/artifact_manifest.csv`
- `data/raw/jp_morgan_case_management/README.md`

Expected artifact categories:

1. Case header exports (case id, owner, state, severity, timestamps)
2. Event timeline exports (state transitions, approvals, escalations)
3. Party/account linkage exports (entity joins)
4. Alert decision exports (true/false positive outcomes)
5. Attachment metadata (document hashes, types, retention tags)
6. Audit trail exports (who/when/what)
7. Sanctions/watchlist decision snapshots
8. Disposition outcomes and remediation closure records

## 4) WISER operating modes

1. **Local deterministic mode**: use checked-in production-grade CSVs.
2. **Enterprise corpus mode**: populate JPM artifact directory from regulated source systems according to the manifest contract.
3. **Hybrid mode**: Enron baseline + JPM artifacts for expanded behavioral profiling.

## 5) Governance + control points

- Source-to-processed provenance is explicit in directory transitions.
- Curated outputs are deterministic and auditable.
- Validation summary enforces row-count consistency across major stages.
- Artifact manifest enforces minimum required corpus completeness for enterprise ingest.
