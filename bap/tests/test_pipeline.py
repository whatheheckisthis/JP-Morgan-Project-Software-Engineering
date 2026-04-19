from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from bap.src.pipeline import run_pipeline


def test_pipeline_creates_expected_outputs(tmp_path: Path) -> None:
    (tmp_path / "data" / "sources" / "enron").mkdir(parents=True)
    (tmp_path / "data" / "sources" / "financial").mkdir(parents=True)

    result = run_pipeline(tmp_path, run_id="run_test")

    assert result["run_id"] == "run_test"
    assert (tmp_path / "outputs" / "run_test" / "risk_profiles.run_id.csv").exists()
    assert (tmp_path / "outputs" / "run_test" / "alert_candidates.run_id.csv").exists()
    assert (tmp_path / "outputs" / "run_test" / "governance_report.run_id.json").exists()
