# Enron Analytics Repository

[![Python](https://img.shields.io/badge/Python-3.10%2B-3776AB?logo=python&logoColor=white)](https://www.python.org/)
[![Tests](https://img.shields.io/badge/tests-pytest-0A9EDC?logo=pytest&logoColor=white)](https://docs.pytest.org/)
[![Security](https://img.shields.io/badge/security-OpenSSL-721412?logo=openssl&logoColor=white)](https://www.openssl.org/)
[![Container](https://img.shields.io/badge/container-Docker-2496ED?logo=docker&logoColor=white)](https://www.docker.com/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A professional analytics workspace for exploring and validating the Enron email and financial dataset, with reusable Python modules, notebooks, and lightweight automated tests.

## Repository Layout

```text
src/        # reusable analytics and NLP modules
tests/      # unit and integration checks
docs/       # notebooks and reference documentation
ci/         # CI configuration mirrors and pipeline assets
```

## Core Capabilities

- **Outlier management** utilities for regression-style workflows.
- **Email parsing and stemming** for text analytics pipelines.
- **Notebook-driven analysis** for exploratory finance and NLP tasks.
- **ITIL 4 / SecDevOps-aligned structure** to support governance and maintainability.

## Quick Start

### 1) Prerequisites

Install the following tools:

- Python 3.10+
- pip
- OpenSSL (optional but recommended for file integrity checks)
- Docker Desktop or Docker Engine (optional, for containerized execution)

### 2) Clone the repository

**Bash (macOS/Linux/Git Bash):**

```bash
git clone <your-repo-url>
cd JP-Morgan-Project-Software-Engineering
```

**Windows CMD:**

```cmd
git clone <your-repo-url>
cd JP-Morgan-Project-Software-Engineering
```

### 3) Create and activate a virtual environment

**Bash (macOS/Linux/Git Bash):**

```bash
python3 -m venv .venv
source .venv/bin/activate
```

**Windows CMD:**

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

Use OpenSSL to create and validate checksums for data or artifacts shared across environments.

### Generate SHA-256 checksum

```bash
openssl dgst -sha256 path/to/file
```

### Verify a known checksum (example)

```bash
echo "<expected_sha256>  path/to/file" | sha256sum -c -
```

> On Windows CMD, you can still use OpenSSL directly if installed, or use PowerShell `Get-FileHash` as an equivalent.

## Docker Usage

Run tests inside a disposable Python container without local Python setup.

```bash
docker run --rm -it \
  -v "${PWD}:/workspace" \
  -w /workspace \
  python:3.11-slim \
  bash -lc "pip install -U pip pytest nltk && pytest -q"
```

### Windows CMD variant

```cmd
docker run --rm -it -v "%cd%:/workspace" -w /workspace python:3.11-slim bash -lc "pip install -U pip pytest nltk && pytest -q"
```

## Governance Notes

The repository organization reflects ITIL 4 and SecDevOps practices:

- Clear separation between production logic (`src`) and verification (`tests`).
- Dedicated documentation surface (`docs`) for analysis traceability.
- CI artifacts under `ci` to support control visibility.

For additional context, see `docs/itil4_secdevops_refactor.md`.


## Contributing

Contributions are welcome and appreciated.

1. Fork the repository and create a focused feature branch.
2. Keep changes scoped and include clear commit messages.
3. Run the test suite locally before opening a pull request:

```bash
pytest -q
```

4. Open a pull request with:
   - A concise summary of what changed.
   - Why the change is needed.
   - Any validation steps or outputs.

### Suggested contributor workflow

```bash
git checkout -b feat/<short-description>
# make your changes
pytest -q
git add .
git commit -m "feat: <describe change>"
git push origin feat/<short-description>
```

## Security

If you discover a potential security issue, please report it responsibly.

- **Do not** post sensitive vulnerability details in public issues.
- Share findings privately with maintainers, including:
  - Impact and affected component(s)
  - Reproduction steps
  - Suggested remediation (if available)

### Security hygiene for local development

- Validate downloaded files with checksums (see OpenSSL usage above).
- Keep dependencies updated in your local environment.
- Avoid committing secrets, credentials, or private keys.
- Use environment variables for sensitive configuration.
