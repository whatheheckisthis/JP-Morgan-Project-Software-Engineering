from __future__ import annotations

from dataclasses import dataclass

from eicvs.evidence.evidence_ledger import ControlTrigger


@dataclass(frozen=True)
class IamRequest:
    vector_id: str
    role_hierarchy_allows_escalation: bool
    resource_access_control_present: bool
    category: str


@dataclass(frozen=True)
class IamAnalysis:
    privilege_escalation_path_present: bool
    missing_access_control_on_resource: bool


def analyse(request: IamRequest) -> IamAnalysis:
    escalation = request.role_hierarchy_allows_escalation
    missing_acl = not request.resource_access_control_present
    return IamAnalysis(privilege_escalation_path_present=escalation, missing_access_control_on_resource=missing_acl)


def _map_controls(analysis: IamAnalysis) -> list[ControlTrigger]:
    triggers: list[ControlTrigger] = []
    if analysis.privilege_escalation_path_present:
        triggers.append(ControlTrigger(asvs_id="ASVS-V4-4.1.1", owasp_ref="A01:2021"))
    if analysis.missing_access_control_on_resource:
        triggers.append(ControlTrigger(asvs_id="ASVS-V4-4.1.3", owasp_ref="A01:2021"))

    for field_name, field_value in analysis.__dict__.items():
        if field_value and field_name not in {"privilege_escalation_path_present", "missing_access_control_on_resource"}:
            raise ValueError(field_name)
    return triggers
