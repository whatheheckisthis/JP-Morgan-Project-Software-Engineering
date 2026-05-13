# QEMU Architectural Projection Correctness

## Scope

`S_QEMU = ⟨Σ_stable, ID_reg_mask, VCPU_cfg⟩` models QEMU as a bounded architectural projection layer for KVM/ARM64 execution. QEMU does not emulate missing features in this model; it exposes only fleet-stable features.

## Projection Constraint

```text
Σ_host ∩ Σ_guest → Σ_stable
```

- `Σ_host`: physical Neoverse N2 or V3 feature set.
- `Σ_guest`: feature set requested by VCPU configuration.
- `Σ_stable`: features present on the host, exposed to the guest, and behaviourally identical across all valid hosts in the deployment fleet.

Features in `Σ_host \ Σ_stable` are masked from the guest. Features in `Σ_guest \ Σ_host` are rejected at VCPU creation. Neither class is emulated.

Invariant `I₅` is:

```text
Σ_stable = Σ_host ∩ Σ_guest computed once at VCPU creation
```

## ID Register Masking

QEMU enforces `Σ_stable` through controlled ID register values:

| Register | Controlled fields |
| --- | --- |
| `ID_AA64PFR0_EL1` | `SVE`, `GIC`, `RAS`, `DIT`, `CSV2` |
| `ID_AA64ZFR0_EL1` | `SVEver`, `SHA3`, `SM4`, `F32MM`, `F64MM` |
| `ID_AA64ISAR1_EL1` | `APA`, `API`, `GPA`, `GPI`, `LRCPC`, `DPB` |
| `ID_AA64ISAR2_EL1` | `APA3`, `GPA3`, `RPRES` |
| `ID_AA64PFR1_EL1` | `MTE`, `SSBS`, `BT`, `RNDs` |

Fields outside `Σ_stable` are returned as `0b0000`. Guest feature detection must test architectural availability thresholds, not raw masked values as if they were host values.

## Heterogeneous Substrate Projection

| Substrate | ISA | Microarchitecture | SVE2 VL max | PAC cipher | Notable `Σ_host` additions |
| --- | --- | --- | --- | --- | --- |
| Azure Cobalt 100 | ARMv8.5-A | Neoverse N2 | 256-bit | QARMA5 | BRBE, MTE2, CSV2_2 |
| Azure Cobalt 200 | ARMv9.2-A | Neoverse V3 | 512-bit | QARMA3 | MTE3, RME/CCA, BTI, FEAT_HBC |

For a fleet spanning both substrates, `Σ_stable` excludes QARMA3 PAC, MTE3 asymmetric tagging, 512-bit SVE2 VL, and RME Realm PAS. These appear as `0b0000` in `ID_AA64*` registers for cross-substrate VCPU configurations.

## Projection Correctness

`Projection_Correct(S)` holds only when every exposed feature is present and behaviourally equivalent on every valid host in the fleet. Exposing QARMA3 to a mixed N2/V3 fleet, exposing 512-bit SVE2 VL to N2, or exposing RME Realm PAS to Normal-world guests violates projection correctness.
