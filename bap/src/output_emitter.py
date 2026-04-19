import csv
import json
from pathlib import Path


def emit_outputs(run_dir: Path, scores: list[dict], alerts: list[dict]) -> dict:
    run_dir.mkdir(parents=True, exist_ok=True)
    risk_profiles = run_dir / "risk_profiles.run_id.csv"
    alert_candidates = run_dir / "alert_candidates.run_id.csv"
    governance_report = run_dir / "governance_report.run_id.json"

    with risk_profiles.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=["entity_id", "risk_score"])
        writer.writeheader()
        writer.writerows(scores)

    with alert_candidates.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=["entity_id", "risk_score"])
        writer.writeheader()
        writer.writerows(alerts)

    governance_report.write_text(
        json.dumps({"entities_scored": len(scores), "alerts": len(alerts)}, indent=2),
        encoding="utf-8",
    )

    return {
        "risk_profiles": str(risk_profiles),
        "alert_candidates": str(alert_candidates),
        "governance_report": str(governance_report),
    }
