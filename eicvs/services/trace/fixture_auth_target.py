from __future__ import annotations

from dataclasses import dataclass

from eicvs.evidence.evidence_ledger import ControlTrigger


@dataclass(frozen=True)
class AuthRequest:
    vector_id: str
    credential_strength: str
    mfa_present: bool
    category: str


@dataclass(frozen=True)
class AuthAnalysis:
    weak_credential_accepted: bool
    missing_mfa_signal: bool


def analyse(request: AuthRequest) -> AuthAnalysis:
    weak = request.credential_strength in {"weak", "very_weak"}
    missing_mfa = not request.mfa_present
    return AuthAnalysis(weak_credential_accepted=weak, missing_mfa_signal=missing_mfa)


def _map_controls(analysis: AuthAnalysis) -> list[ControlTrigger]:
    triggers: list[ControlTrigger] = []
    if analysis.weak_credential_accepted:
        triggers.append(ControlTrigger(asvs_id="ASVS-V2-2.1.1", owasp_ref="A07:2021"))
    if analysis.missing_mfa_signal:
        triggers.append(ControlTrigger(asvs_id="ASVS-V2-2.2.1", owasp_ref="A07:2021"))

    for field_name, field_value in analysis.__dict__.items():
        if field_value and field_name not in {"weak_credential_accepted", "missing_mfa_signal"}:
            raise ValueError(field_name)
    return triggers
