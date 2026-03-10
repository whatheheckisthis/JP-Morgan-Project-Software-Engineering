# Intent-to-Auditable-Trust-Object (IATO)
From raw emails to risk signals — a full-lifecycle analytics pipeline over the Enron corpus using PCA, NLP, and quantitative methods for anomaly detection, profiling, and reproducible validation.

[![Python](https://img.shields.io/badge/Python-3.10%2B-3776AB?logo=python&logoColor=white)](https://www.python.org/)
[![Tests](https://img.shields.io/badge/tests-pytest-0A9EDC?logo=pytest&logoColor=white)](https://docs.pytest.org/)
[![Security](https://img.shields.io/badge/security-OpenSSL-721412?logo=openssl&logoColor=white)](https://www.openssl.org/)
[![Container](https://img.shields.io/badge/container-Docker-2496ED?logo=docker&logoColor=white)](https://www.docker.com/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

## Executive Summary

This repository started as a broad JPMC-context resource collection and matured into a structured, reproducible analytics pipeline built around the Enron email and financial corpus. The project integrates NLP over communication data with quantitative analysis over financial records to produce a unified behavioral risk lens.

Principal Component Analysis (PCA) is the central dimensionality-reduction technique used to compress noisy, high-dimensional features into interpretable components that preserve the most meaningful variance. This foundation supports downstream anomaly detection, risk profiling, and governance-focused analytics in regulated environments.

## Project Objectives

1. Build a repeatable end-to-end pipeline from ingestion to validated outputs.
2. Combine email-language and financial signals to model organizational behavior.
3. Use PCA to improve interpretability and reduce multicollinearity before modeling.
4. Enable risk-focused use cases such as anomaly detection and entity profiling.
5. Preserve auditability, traceability, and security hygiene for compliance-heavy contexts.

## Definitions

- **Corpus**: The combined Enron email and financial datasets used for analysis.
- **NLP (Natural Language Processing)**: Techniques used to parse, normalize, and extract signal from text communication.
- **Feature Engineering**: Transforming raw records into model-ready variables.
- **PCA (Principal Component Analysis)**: A dimensionality-reduction method that projects features into orthogonal components ordered by explained variance.
- **Anomaly Detection**: Identifying unusual communication or transaction patterns relative to baseline behavior.
- **Risk Profiling**: Characterizing individuals, groups, or departments by behavioral and financial risk indicators.
- **Reproducibility**: The ability to rerun the same workflow and obtain consistent results with documented parameters.

## Workflow 

```text
+------------------+       +-------------------------+       +----------------------+
| Data Sources     | ----> | Ingestion & Normalization| ---> | Feature Engineering  |
| - Enron Emails   |       | - Schema alignment       |       | - NLP features       |
| - Financial Data |       | - Cleaning/typing        |       | - Financial features |
+------------------+       +-------------------------+       +----------------------+
                                                                     |
                                                                     v
                                                           +----------------------+
                                                           | Dimensionality        |
                                                           | Reduction (PCA)       |
                                                           | - Variance isolation  |
                                                           | - Collinearity cut    |
                                                           +----------------------+
                                                                     |
                                                                     v
+------------------+       +-------------------------+       +----------------------+
| Validation       | <---- | Modeling & Scoring      | ----> | Outputs              |
| - Tests/checks   |       | - Anomaly detection     |       | - Risk profiles      |
| - Parameter logs |       | - Behavior segmentation |       | - Alert candidates   |
+------------------+       +-------------------------+       | - Governance reports |
                                                              +----------------------+
```

## Project Overview

The pipeline is organized as a coherent system spanning:

- Data ingestion
- Feature engineering
- Exploratory analysis
- Reproducible validation workflows

The architecture treats **NLP-derived communication signals** and **quantitative financial signals** as complementary views of the same underlying organizational behavior.

## Why PCA Is Central?

PCA is applied as a core preprocessing step to:

- Isolate high-variance behavioral and financial signals from noisy, high-dimensional inputs
- Improve model interpretability by reducing feature space complexity
- Reduce multicollinearity before downstream modeling and scoring
- Support stable, explainable, and repeatable analysis workflows

## Downstream Analytics

Built on top of ingestion + feature engineering + PCA, the repository supports:

- **Anomaly detection** across communication and transaction patterns
- **Risk-focused profiling** of individuals and departments
- **Security-aware governance** in regulated and compliance-heavy settings
- **Institutional forensics** and behavioral analytics use cases beyond Enron

## Reproducibility and Validation

The repository is designed to be reproducible by default:

- Validation steps are documented and executable
- Key workflows are parameterized for reruns
- Project structure supports repeatability across similar datasets

## Repository Layout

```text
src/        # reusable analytics and NLP modules
tests/      # unit and integration checks
docs/       # notebooks and reference documentation
ci/         # CI configuration mirrors and pipeline assets
```

## Core Capabilities

- **Outlier management** utilities for regression-style workflows
- **Email parsing and stemming** for text analytics pipelines
- **Notebook-driven analysis** for exploratory finance and NLP tasks
- **ITIL 4 / SecDevOps-aligned structure** for governance and maintainability

## Controls Mapping 

> This mapping is a governance alignment aid for engineering teams and audit preparation. It is not a formal certification statement.
> Control references below are **spec identifiers** to support traceability; validate against the exact release/version used by your compliance program.

| Core capability | SOC 2 control spec numbers (TSC) | ISM control spec reference | Essential Eight spec reference (ML4) |
|---|---|---|---|
| Outlier management and data-quality cleaning | **PI1.1, PI1.2, PI1.3** (processing completeness/accuracy/timeliness); supporting **CC7.2** (anomaly detection/monitoring). | **ISM controls catalogue: System/Information Integrity + Event Logging** control IDs (org-selected baseline). | **M3 Restrict Admin Privileges**, **M4 Patch Applications**, **M8 Regular Backups** at **Maturity Level 4**. |
| Email parsing and stemming pipelines | **CC6.1, CC6.2, CC6.3** (logical access), **C1.1** (confidentiality protection), supporting **CC7.1** (security monitoring). | **ISM controls catalogue: Access Control + Data Protection** control IDs (org-selected baseline). | **M1 Application Control**, **M5 MFA**, **M6 User Application Hardening** at **Maturity Level 4**. |
| Notebook-driven analysis and reproducible research | **CC8.1** (change management), **CC2.3** (accountability/oversight), supporting **PI1.4** (error correction). | **ISM controls catalogue: Secure Development + Audit/Logging** control IDs (org-selected baseline). | **M1 Application Control**, **M7 Macro Controls** (where Office is in scope), **M8 Regular Backups** at **Maturity Level 4**. |
| ITIL 4 / SecDevOps-aligned repository and CI structure | **CC5.2, CC5.3** (control activities), **CC7.3, CC7.4** (monitoring/remediation), **CC8.1** (change lifecycle). | **ISM controls catalogue: Secure Configuration + Continuous Monitoring + Supply Chain** control IDs (org-selected baseline). | **M2 Patch Operating Systems**, **M4 Patch Applications**, **M5 MFA**, **M3 Restrict Admin Privileges** at **Maturity Level 4**. |

## Quick Start

### 1) Prerequisites

Install the following tools:

- Python 3.10+
- pip
- OpenSSL (optional but recommended for integrity checks)
- Docker Desktop or Docker Engine (optional, for containerized execution)

### 2) Clone the repository

```bash
git clone <your-repo-url>
cd Intent-to-Auditable-Trust-Object
```

### 3) Create and activate a virtual environment

```bash
python3 -m venv .venv
source .venv/bin/activate
```

Windows CMD:

```cmd
python -m venv .venv
.venv\Scripts\activate
```

### 4) Install runtime and test dependencies

```bash
python -m pip install --upgrade pip
pip install pytest nltk
```

### 5) Run automated tests

```bash
pytest -q
```

## OpenSSL Usage (Integrity / Security)

### Generate SHA-256 checksum

```bash
openssl dgst -sha256 path/to/file
```

### Verify a known checksum

```bash
echo "<expected_sha256>  path/to/file" | sha256sum -c -
```

## Docker Usage

```bash
docker run --rm -it \
  -v "${PWD}:/workspace" \
  -w /workspace \
  python:3.11-slim \
  bash -lc "pip install -U pip pytest nltk && pytest -q"
```

Windows CMD variant:

```cmd
docker run --rm -it -v "%cd%:/workspace" -w /workspace python:3.11-slim bash -lc "pip install -U pip pytest nltk && pytest -q"
```

## Governance Notes

The repository organization reflects ITIL 4 and SecDevOps practices:

- Clear separation between production logic (`src`) and verification (`tests`)
- Dedicated documentation surface (`docs`) for analysis traceability
- CI artifacts under `ci` to support control visibility

For additional context, see `docs/itil4_secdevops_refactor.md`.

## Contributing

1. Fork the repository and create a focused feature branch
2. Keep changes scoped with clear commit messages
3. Run the test suite locally before opening a pull request
4. Open a pull request with a concise change summary and validation notes

## Security

If you discover a potential security issue, report it responsibly and avoid posting sensitive details in public issues.

Local development hygiene:

- Validate downloaded files with checksums
- Keep dependencies updated
- Avoid committing secrets or keys
- Use environment variables for sensitive configuration
