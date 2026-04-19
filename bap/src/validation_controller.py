def validate_scores(scores: list[dict], threshold: float = 0.8) -> list[dict]:
    return [score for score in scores if score["risk_score"] >= threshold]
