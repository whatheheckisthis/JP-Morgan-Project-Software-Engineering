# EL2 Control Correctness

## Scope

`S_EL2 = ⟨HCR_EL2, VTTBR_EL2, VTCR_EL2, DAIF, ELR_EL2, SPSR_EL2⟩` is the constrained enforcement layer for guest execution. EL2 is not modelled as trusted; it is modelled as the architectural privilege level that enforces traps, Stage-2 translation activation, VM entry, VM exit, and exception-level transitions.

## HCR_EL2 Invariants

The following `HCR_EL2` fields are hard invariants for every VCPU in `S_VCPU ∈ {running, vmexit}`:

| Field | Enforced Value | Correctness obligation |
| --- | --- | --- |
| `HCR_EL2.VM` | `1` | Stage-2 translation is active; `HCR_EL2.VM = 0` while running is invalid. |
| `HCR_EL2.RW` | `1` | EL1 executes AArch64; AArch32 guest execution is outside scope. |
| `HCR_EL2.TSC` | `1` | Guest `SMC` traps to EL2; EL1 cannot invoke EL3 directly. |
| `HCR_EL2.TGE` | `0` | Guest owns EL1; `TGE = 1` is incompatible with KVM guest execution. |
| `HCR_EL2.TWI` | per-VCPU | WFI trap policy is declared at VCPU creation, not changed at runtime. |
| `HCR_EL2.TWE` | per-VCPU | WFE trap policy is declared at VCPU creation, not changed at runtime. |
| `HCR_EL2.VSE` | EL2-controlled | Virtual SError injection is explicit EL2 state. |

Invariant `I₁` is therefore:

```text
HCR_EL2.VM = 1 ∀ S_VCPU ∈ {running, vmexit}
```

## VM Entry Preconditions

`KVM_RUN` may enter the guest only when:

```text
pre(VM_ENTRY):
  HCR_EL2.VM = 1
  ∧ VTTBR_EL2.BADDR points to valid Stage-2 root table
  ∧ VTCR_EL2 fields consistent with IPA space declared at VCPU creation
  ∧ SVE context: fully saved from prior exit OR zero-initialised on first entry
  ∧ DAIF state: coherent with interrupt routing policy
  ∧ S_VCPU ∈ {configured, vmexit}
```

`VTTBR_EL2.BADDR`, `VTCR_EL2`, `DAIF`, and `ZCR_EL2` are part of the entry contract; they are not inferred from KVM userspace configuration after entry begins.

## VM Exit Semantics

Every exit records `kvm_run->exit_reason` atomically with the VCPU state transition to `vmexit`.

| Exit reason | Architectural cause |
| --- | --- |
| `KVM_EXIT_MMIO` | Stage-2 fault on device-mapped IPA. |
| `KVM_EXIT_HVC` | Guest `HVC` instruction and hypercall dispatch. |
| `KVM_EXIT_SYSTEM_EVENT` | PSCI call or system reset request. |
| `KVM_EXIT_IRQ` | Virtual IRQ injection acknowledgement. |
| `KVM_EXIT_EXCEPTION` | Unhandled synchronous exception at EL1. |

Consuming EL1 register state without issuing a complete response for the recorded `exit_reason` is a partial exit-handling violation.

## Save/Restore Obligations

EL2-owned state saved unconditionally at every VM exit:

```text
{ HCR_EL2, VTTBR_EL2, VTCR_EL2,
  SCTLR_EL2, TCR_EL2, MAIR_EL2,
  SPSR_EL2, ELR_EL2, SP_EL2,
  DAIF, ZCR_EL2 }
```

`DAIF` is restored atomically at EL1 re-entry. Restoring `DAIF.I` without `DAIF.F`, `DAIF.A`, and `DAIF.D` is a correctness violation under `I₇`.

`ZCR_EL2.LEN = (VL / 128) - 1` is EL2-owned state. It bounds effective SVE vector length and is restored with EL2 state, not treated as an EL1-owned preference.

## EL2 Entry Microarchitectural Obligations

`CLEARBHB` is issued at every EL2 entry on Neoverse N2 and V3 hosts regardless of the value reported in `ID_AA64PFR0_EL1.CSV2`. RSB stuffing is required before any `RET`-based EL2 control flow executes. These obligations support `I₁₄` and the microarchitectural non-interference component of `CORRECT(S)`.
