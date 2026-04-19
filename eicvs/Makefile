auth:
	# Run the auth class pipeline
	PYTHONPATH=.. python -c "from eicvs.services.trace.capture_auth import run; run('tests/fixtures/auth_vectors.json')"

ssrf:
	# Run the ssrf class pipeline
	PYTHONPATH=.. python -c "from eicvs.services.trace.capture_ssrf import run; run('tests/fixtures/ssrf_vectors.json')"

deserialization:
	# Run the deserialization class pipeline
	PYTHONPATH=.. python -c "from eicvs.services.trace.capture_deserialization import run; run('tests/fixtures/deserialization_vectors.json')"

supply_chain:
	# Run the supply_chain class pipeline
	PYTHONPATH=.. python -c "from eicvs.services.trace.capture_supply_chain import run; run('tests/fixtures/supply_chain_vectors.json')"

iam:
	# Run the iam class pipeline
	PYTHONPATH=.. python -c "from eicvs.services.trace.capture_iam import run; run('tests/fixtures/iam_vectors.json')"

all: auth ssrf deserialization supply_chain iam

verify-chain:
	# Invoke evidence_ledger.verify_chain()
	PYTHONPATH=.. python -c "from eicvs.evidence.evidence_ledger import verify_chain; raise SystemExit(0 if verify_chain() else 1)"

audit-gate:
	# Exit non-zero if any OPEN gap exists in the gap register
	python -c "import csv; import sys; rows=list(csv.DictReader(open('evidence/evidence_gap_register.csv', encoding='utf-8'))); sys.exit(1 if any(r['status']=='OPEN' for r in rows) else 0)"

gap-check:
	# Print all rows from the gap register
	python -c "import pathlib; print(pathlib.Path('evidence/evidence_gap_register.csv').read_text(encoding='utf-8').rstrip())"
