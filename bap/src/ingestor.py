from pathlib import Path


def ingest_sources(base_dir: Path) -> dict:
    return {
        "email_corpus": str(base_dir / "data" / "sources" / "enron"),
        "financial_records": str(base_dir / "data" / "sources" / "financial"),
    }
