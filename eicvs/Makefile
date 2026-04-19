PYTHON ?= python3
LEDGER ?= eicvs/evidence/evidence_ledger.jsonl

.PHONY: validate test clean

validate:
	@$(PYTHON) -m pytest eicvs/tests -q

test: validate

clean:
	@rm -f $(LEDGER)
