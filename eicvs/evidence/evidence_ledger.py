"""Append-only SHA-256 chained evidence ledger for EICVS/BAP records."""

from __future__ import annotations

import hashlib
import json
import time
from pathlib import Path
from typing import Any


class EvidenceLedger:
    """Simple local append-only JSONL ledger with hash chaining."""

    def __init__(self, ledger_path: str | Path) -> None:
        self.ledger_path = Path(ledger_path)
        self.ledger_path.parent.mkdir(parents=True, exist_ok=True)

    def _read_last_record(self) -> dict[str, Any] | None:
        if not self.ledger_path.exists() or self.ledger_path.stat().st_size == 0:
            return None
        with self.ledger_path.open("r", encoding="utf-8") as handle:
            lines = [line.strip() for line in handle if line.strip()]
        return json.loads(lines[-1]) if lines else None

    def append(self, payload: dict[str, Any]) -> dict[str, Any]:
        previous = self._read_last_record()
        previous_id = previous["record_id"] if previous else "GENESIS"
        timestamp_ns = time.time_ns()
        payload_fp = hashlib.sha256(
            json.dumps(payload, sort_keys=True).encode("utf-8")
        ).hexdigest()
        record_id = hashlib.sha256(
            f"{previous_id}|{payload_fp}|{timestamp_ns}".encode("utf-8")
        ).hexdigest()

        record = {
            "record_id": record_id,
            "previous_id": previous_id,
            "payload_fp": payload_fp,
            "timestamp_ns": timestamp_ns,
            "payload": payload,
        }
        with self.ledger_path.open("a", encoding="utf-8") as handle:
            handle.write(json.dumps(record, sort_keys=True) + "\n")
        return record


if __name__ == "__main__":
    ledger = EvidenceLedger(Path(__file__).with_name("evidence_ledger.jsonl"))
    created = ledger.append({"source": "eicvs", "result": "PASS"})
    print(json.dumps(created, indent=2))
