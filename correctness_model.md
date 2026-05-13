# Global Execution Correctness Model

## Global State

```text
S = ⟨S_EL2, S_Stage2, S_SVE2, S_QEMU, S_KVM, S_Microarch⟩
```

```text
S_EL2       = ⟨HCR_EL2, VTTBR_EL2, VTCR_EL2, DAIF, ELR_EL2, SPSR_EL2⟩
S_Stage2    = ⟨VMID, IPA→PA_table, fault_log⟩
S_SVE2      = ⟨Z₀..₃₁, P₀..₁₅, FFR, VL⟩
S_QEMU      = ⟨Σ_stable, ID_reg_mask, VCPU_cfg⟩
S_KVM       = ⟨S_VCPU, exit_reason, run_state⟩
S_Microarch = ⟨BTB_state, BHB_state, LLC_state, RSB_state, pipeline_state⟩
```

No component is implicit across VM entry or VM exit. Every state transition names the register, table, feature set, or microarchitectural state it affects.

## Correctness Definition

```text
CORRECT(S) ⟺
  Arch_Correct(S)
  ∧ Isolation_Correct(S)
  ∧ Microarch_Noninterference(S)
  ∧ Projection_Correct(S)
```

- `Arch_Correct(S)`: `S_EL2`, `S_Stage2`, `S_SVE2`, and `S_KVM` transitions conform to ARM ARM and KVM/ARM64 behaviour.
- `Isolation_Correct(S)`: guests cannot observe other guests or EL2 through architectural instruction or exception mechanisms.
- `Microarch_Noninterference(S)`: `BTB_state`, `BHB_state`, `LLC_state`, `RSB_state`, and `pipeline_state` do not reveal protected state.
- `Projection_Correct(S)`: `Σ_stable` is a behaviourally stable subset of `Σ_host` across the deployment fleet.

## Full Invariant Set

```text
I₁:  HCR_EL2.VM = 1               ∀ S_VCPU ∈ {running, vmexit}
I₂:  VTCR_EL2.PS ≥ host_PA_range  at VCPU creation; not modifiable thereafter
I₃:  VL ∈ {128, 256, 512, 1024, 2048} ∧ VL ≤ VL_max(substrate)
I₄:  VL invariant ∀ t ≥ t_configured
I₅:  Σ_stable = Σ_host ∩ Σ_guest   computed once at VCPU creation
I₆:  SVE_SAVE_EXTRA: Z then P then FFR; violation is a correctness fault
I₇:  DAIF restored atomically at EL1 re-entry; partial restoration not permitted
I₈:  VMID(VTTBR_EL2) unique across all S_VCPU = running instances
I₉:  (V3/RME) No Stage-2 PTE maps Realm or Secure PAS
I₁₀: BRBE disabled or context-saved before each VM entry unless guest BRBE granted
I₁₁: RDVL result × 8 = configured VL; divergence is an observable fault
I₁₂: MTE3 VCPU not migrated to host where MTE3 absent; silent downgrade rejected
I₁₃: QARMA3 PAC keys not masked from guest on V3-homogeneous fleet only
I₁₄: CLEARBHB issued at EL2 entry on N2/V3 hosts regardless of CSV2 field value
```

## Architectural Correctness

`Arch_Correct(S)` requires:

- `HCR_EL2.VM`, `HCR_EL2.RW`, `HCR_EL2.TSC`, `HCR_EL2.TGE`, `HCR_EL2.TWI`, `HCR_EL2.TWE`, and `HCR_EL2.VSE` satisfy EL2 invariants.
- `VTTBR_EL2.BADDR`, `VTTBR_EL2.VMID`, and `VTCR_EL2` fields match the declared Stage-2 configuration.
- `ZCR_EL2.LEN = (VL / 128) - 1` and `RDVL Xn, #1` reports `VL / 8`.
- `KVM_RUN`, `KVM_GET_ONE_REG`, and `KVM_SET_ONE_REG` are used only in legal `S_VCPU` states.

## Isolation Correctness

`Isolation_Correct(S)` requires Stage-2 faults for unmapped IPA, Device-nGnRE or Device-nGnRnE mappings for device MMIO IPA, VMID uniqueness, no guest mapping to EL2-private PA ranges, and no Normal-world Stage-2 PTE to Realm PAS or Secure PAS on RME hosts.

## Microarchitectural Non-Interference

```text
∀ attacker VCPU A, victim VCPU B:
  obs(S_Microarch | execution of B) is independent of B's protected memory contents
```

This includes Spectre-v1, Spectre-v2, Spectre-BHB, Meltdown-class speculative load concerns, cache timing, RSB underflow, BRBE exposure, and pipeline latency.

## Projection Correctness

`Projection_Correct(S)` requires:

```text
Σ_host ∩ Σ_guest → Σ_stable
Σ_stable ⊆ Σ_host
```

QEMU masks `ID_AA64PFR0_EL1`, `ID_AA64ZFR0_EL1`, `ID_AA64ISAR1_EL1`, `ID_AA64ISAR2_EL1`, and `ID_AA64PFR1_EL1` fields not present in `Σ_stable`.

## NTT Cryptographic Execution Constraints

For SVE2 NTT implementations targeting CRYSTALS-Kyber and CRYSTALS-Dilithium:

```text
- NTT butterfly uses SQRDMLAH and MUL Zd.H, Zn.H, Zm.H[idx]
- P₀..P₃ gate active lanes
- FFR is not used in the NTT inner loop
- No data-dependent branch occurs in the NTT path
- No data-dependent coefficient-array memory traversal occurs
- Transform length = k × VL, with k fixed at initialisation
- ∀ inputs of equal length, execution time is identical
```

The NTT constraint depends on invariant `I₄`; if `VL` changes after configuration, transform length and constant-time behaviour are no longer stable.
