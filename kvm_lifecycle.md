# KVM VCPU Lifecycle Correctness

## Scope

`S_KVM = ⟨S_VCPU, exit_reason, run_state⟩` models the KVM/ARM64 VCPU lifecycle, KVM interface obligations, and migration constraints. State transitions not listed here are illegal.

## VCPU State Machine

```text
S_VCPU = ⟨created, configured, running, vmexit, migrating, destroyed⟩
```

Legal transitions:

```text
created     → configured   : KVM_SET_ONE_REG applied; VL negotiated; feature set finalised
configured  → running      : KVM_RUN issued; VM entry preconditions satisfied
running     → vmexit       : trap, fault, interrupt, HVC, or PSCI causes architectural exit
vmexit      → running      : exit reason handled; KVM_RUN reissued
vmexit      → migrating    : VCPU state serialised for live migration
migrating   → configured   : VCPU state deserialised on target host; feature set re-validated
configured  → destroyed    : VCPU fd closed; all associated state freed
* → destroyed              : unrecoverable fault; immediate teardown
```

A VCPU in `running` cannot transition directly to `migrating`. A VCPU in `destroyed` has no valid successors.

## KVM Interface Obligations

| Interface | Correct usage |
| --- | --- |
| `KVM_CHECK_EXTENSION(KVM_CAP_ARM_SVE)` | Must return non-zero before any SVE VCPU is created. |
| `KVM_SET_ONE_REG(KVM_REG_ARM64_SVE_VLS)` | Sets `VL`; valid only during `configured` state. |
| `KVM_SET_ONE_REG(KVM_REG_ARM64_*)` | General VCPU register initialisation before running. |
| `KVM_RUN` | Triggers VM entry and blocks until VM exit. |
| `KVM_GET_ONE_REG` | Reads register state; valid in `vmexit` state. |

`KVM_SET_ONE_REG` calls targeting SVE registers after the VCPU has entered `running` are rejected, not deferred or silently accepted.

## VM Entry Coupling

`configured → running` is valid only when the EL2 preconditions hold:

```text
HCR_EL2.VM = 1
∧ VTTBR_EL2.BADDR points to valid Stage-2 root table
∧ VTCR_EL2 fields match declared IPA space
∧ SVE context saved or zero-initialised
∧ DAIF coherent with interrupt routing
```

`KVM_RUN` is therefore coupled to `S_EL2`, `S_Stage2`, and `S_SVE2`; it is not a scheduler-only action.

## Migration Constraints

A live-migration serialisation contains:

```text
{ S_EL2, S_Stage2.VMID, S_SVE2, S_KVM.run_state,
  all EL1 system registers,
  MTE tag memory (if MTE enabled),
  BRBE context (if BRBE enabled and guest-accessible) }
```

The target host must satisfy:

```text
Σ_stable(target) ⊆ Σ_stable(source)
```

Migration that drops architectural features, downgrades MTE3 to MTE2, or changes PAC semantics from QARMA3 to QARMA5 under a supposedly stable configuration is a correctness violation.
