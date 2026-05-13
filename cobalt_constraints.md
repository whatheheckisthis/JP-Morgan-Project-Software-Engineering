# Azure Cobalt ARM Constraints

## Scope

This file records the Azure Cobalt 100 and Azure Cobalt 200 substrate constraints used by `Σ_host`, `Σ_stable`, SVE2 vector-length bounds, PAC compatibility, MTE migration, and RME Stage-2 invariants.

## Azure Cobalt 100: Neoverse N2 / ARMv8.5-A

ID register constraints:

```text
ID_AA64PFR0_EL1.SVE    ≠ 0b0000   → SVE present
ID_AA64ZFR0_EL1.SVEver = 0b0001   → SVE2 confirmed
ID_AA64PFR0_EL1.CSV2   = 0b0010   → FEAT_CSV2_2 present; Spectre-BHB HW mitigation active
ID_AA64PFR1_EL1.MTE    = 0b0011   → MTE2; async and sync tag fault modes
ID_AA64ISAR1_EL1.APA   = 0b0001   → PAC with QARMA5 cipher
ID_AA64DFR0_EL1.BRBE  ≠ 0         → BRBE present; isolation obligations apply
```

SVE2 vector length ceiling on N2 is 256 bits. Any VCPU requesting `VL > 256` is rejected at `KVM_SET_ONE_REG(KVM_REG_ARM64_SVE_VLS)`.

Azure Boost NVMe and network MMIO ranges are Stage-2 mapped as Device-nGnRE. No Normal-cacheable Stage-2 mapping is used for Azure Boost device ranges.

## Azure Cobalt 200: Neoverse V3 / ARMv9.2-A

ID register constraints:

```text
ID_AA64ZFR0_EL1.SVEver  = 0b0001  → SVE2 present; 512-bit VL max
ID_AA64ISAR2_EL1.APA3   = 0b0001  → PAC with QARMA3 cipher
ID_AA64PFR1_EL1.MTE     = 0b0100  → MTE3; asymmetric tagging
ID_AA64PFR0_EL1.RME    ≠ 0        → Realm Management Extension present
ID_AA64PFR1_EL1.BT     ≠ 0        → Branch Target Identification active
```

## PAC QARMA3 Transition

QARMA3 produces different PAC field values than QARMA5 for the same pointer and key input. PAC key registers including `APIAKey_EL1`, `APDAKey_EL1`, and `APGAKey_EL1` are EL2-owned for save/restore purposes. A `Σ_stable` configuration that exposes QARMA3 must be V3-homogeneous; otherwise invariant `I₁₃` is violated.

## MTE3 Save/Restore

MTE3 VCPU state includes:

```text
Save:
  TFSRE0_EL1, TFSR_EL1
  RGSR_EL1, GCR_EL1
  MTE tag memory region, size = data_region / 16
  SCTLR_EL1.{TCF, TCF0}

Restore:
  tag memory restored before SCTLR_EL1 tag checking is re-enabled
```

A VCPU with MTE3 state migrated to a host where `ID_AA64PFR1_EL1.MTE ≠ 0b0100` is rejected under `I₁₂`; silent downgrade to MTE2 is invalid.

## RME / CCA Normal-World Constraints

The repository operates in the Normal world. On V3/RME hosts:

```text
I_RME_1: No VTTBR_EL2 entry references Realm PAS granules
I_RME_2: No VTTBR_EL2 entry references Secure PAS granules
I_RME_3: GPT at EL3 enforces PAS partitioning; Stage-2 faults on Realm/Secure granules are GPT-enforced
```
