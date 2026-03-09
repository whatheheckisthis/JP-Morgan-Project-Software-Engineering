"""Email text parsing utilities for secure text analytics pipelines."""

from __future__ import annotations

import string
from nltk.stem.snowball import SnowballStemmer


TRANSLATION_TABLE = str.maketrans("", "", string.punctuation)


def parse_out_text(email_file):
    """Parse and stem the email body text after metadata headers."""
    email_file.seek(0)
    all_text = email_file.read()
    content = all_text.split("X-FileName:")

    if len(content) <= 1:
        return ""

    text_string = content[1].translate(TRANSLATION_TABLE)
    tokens = text_string.split()
    stemmer = SnowballStemmer("english")
    stemmed_tokens = [stemmer.stem(token) for token in tokens]
    return " ".join(stemmed_tokens)


# Backward-compatible alias used by legacy notebooks.
parseOutText = parse_out_text
