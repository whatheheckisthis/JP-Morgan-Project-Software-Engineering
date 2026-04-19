from __future__ import annotations

from dataclasses import dataclass

from eicvs.evidence.evidence_ledger import ControlTrigger


@dataclass(frozen=True)
class SupplyChainRequest:
    vector_id: str
    signature_verified: bool
    version_pinned: bool
    category: str


@dataclass(frozen=True)
class SupplyChainAnalysis:
    unsigned_package_accepted: bool
    unpinned_dependency_resolved: bool


def analyse(request: SupplyChainRequest) -> SupplyChainAnalysis:
    unsigned = request.signature_verified is False
    unpinned = request.version_pinned is False
    return SupplyChainAnalysis(unsigned_package_accepted=unsigned, unpinned_dependency_resolved=unpinned)


def _map_controls(analysis: SupplyChainAnalysis) -> list[ControlTrigger]:
    triggers: list[ControlTrigger] = []
    if analysis.unsigned_package_accepted:
        triggers.append(ControlTrigger(asvs_id="ASVS-V10-10.2.1", owasp_ref="A08:2021"))
    if analysis.unpinned_dependency_resolved:
        triggers.append(ControlTrigger(asvs_id="ASVS-V10-10.2.4", owasp_ref="A08:2021"))

    for field_name, field_value in analysis.__dict__.items():
        if field_value and field_name not in {"unsigned_package_accepted", "unpinned_dependency_resolved"}:
            raise ValueError(field_name)
    return triggers
