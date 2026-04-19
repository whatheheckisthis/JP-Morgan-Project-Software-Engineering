# Architecture Overview

This repository now contains two parallel, independently executable components:

- **EICVS (Exploit-Informed Control Validation System):** deterministic fixture-driven control validation with append-only evidence output.
- **BAP (Behavioural Analytics Pipeline):** staged behavioural analytics pipeline that ingests at-rest corpora and emits run-scoped artifacts.

Both components converge at a shared append-only evidence ledger model, but do not consume one another's outputs directly.
