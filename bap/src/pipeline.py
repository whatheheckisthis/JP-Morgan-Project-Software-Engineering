from pathlib import Path

from .dimensionality_reducer import reduce_dimensions
from .feature_engineer import engineer_features
from .ingestor import ingest_sources
from .modelling_engine import score_entities
from .normaliser import normalise
from .output_emitter import emit_outputs
from .validation_controller import validate_scores


def run_pipeline(base_dir: str | Path, run_id: str = "run_0001") -> dict:
    base_dir = Path(base_dir)
    run_dir = base_dir / "outputs" / run_id

    raw = ingest_sources(base_dir)
    normalised = normalise(raw)
    features = engineer_features(normalised)
    reduced = reduce_dimensions(features)
    scores = score_entities(reduced)
    alerts = validate_scores(scores)

    outputs = emit_outputs(run_dir, scores, alerts)
    return {"run_id": run_id, "outputs": outputs, "alerts": alerts}
