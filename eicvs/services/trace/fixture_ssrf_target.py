from __future__ import annotations

from dataclasses import dataclass
from typing import Final

from eicvs.evidence.evidence_ledger import ControlTrigger

LOOPBACK_HOST: Final[str] = "127.0.0.1"


@dataclass(frozen=True)
class SsrfRequest:
    vector_id: str
    target_host: str
    socket_host: str
    category: str


@dataclass(frozen=True)
class SsrfAnalysis:
    external_host_reached: bool
    non_loopback_socket_opened: bool


def analyse(request: SsrfRequest) -> SsrfAnalysis:
    external = request.target_host not in {"127.0.0.1", "localhost"}
    non_loopback = request.socket_host != LOOPBACK_HOST
    return SsrfAnalysis(external_host_reached=external, non_loopback_socket_opened=non_loopback)


def _map_controls(analysis: SsrfAnalysis) -> list[ControlTrigger]:
    triggers: list[ControlTrigger] = []
    if analysis.external_host_reached:
        triggers.append(ControlTrigger(asvs_id="ASVS-V10-10.3.1", owasp_ref="A10:2021"))
    if analysis.non_loopback_socket_opened:
        triggers.append(ControlTrigger(asvs_id="ASVS-V10-10.3.2", owasp_ref="A10:2021"))

    for field_name, field_value in analysis.__dict__.items():
        if field_value and field_name not in {"external_host_reached", "non_loopback_socket_opened"}:
            raise ValueError(field_name)
    return triggers
