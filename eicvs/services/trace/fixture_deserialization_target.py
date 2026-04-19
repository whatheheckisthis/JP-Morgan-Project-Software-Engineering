from __future__ import annotations

from dataclasses import dataclass

from eicvs.evidence.evidence_ledger import ControlTrigger


@dataclass(frozen=True)
class DeserializationRequest:
    vector_id: str
    source_trusted: bool
    coercion_guard_enabled: bool
    type_name: str
    category: str


@dataclass(frozen=True)
class DeserializationAnalysis:
    untrusted_object_deserialised: bool
    type_coercion_bypass_detected: bool


def analyse(request: DeserializationRequest) -> DeserializationAnalysis:
    untrusted = request.source_trusted is False
    bypass = request.coercion_guard_enabled is False and request.type_name in {"object", "any"}
    return DeserializationAnalysis(untrusted_object_deserialised=untrusted, type_coercion_bypass_detected=bypass)


def _map_controls(analysis: DeserializationAnalysis) -> list[ControlTrigger]:
    triggers: list[ControlTrigger] = []
    if analysis.untrusted_object_deserialised:
        triggers.append(ControlTrigger(asvs_id="ASVS-V5-5.5.1", owasp_ref="A08:2021"))
    if analysis.type_coercion_bypass_detected:
        triggers.append(ControlTrigger(asvs_id="ASVS-V5-5.5.3", owasp_ref="A08:2021"))

    for field_name, field_value in analysis.__dict__.items():
        if field_value and field_name not in {"untrusted_object_deserialised", "type_coercion_bypass_detected"}:
            raise ValueError(field_name)
    return triggers
