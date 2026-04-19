def engineer_features(normalised: dict) -> dict:
    return {
        "nlp": ["email_volume", "after_hours_ratio", "subject_entropy"],
        "financial": ["txn_volume", "large_txn_ratio", "debit_credit_ratio"],
        "metadata": normalised,
    }
