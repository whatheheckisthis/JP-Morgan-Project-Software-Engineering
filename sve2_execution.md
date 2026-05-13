# SVE2 Register-File Execution Correctness

## Scope

`S_SVE2 = ⟨Z₀..₃₁, P₀..₁₅, FFR, VL⟩` is deterministic architectural state. SVE2 is modelled as vector-state execution, not as a performance optimisation.

## Feature Detection Sequence

Feature detection occurs once during VCPU creation:

```text
Step 1: Read ID_AA64PFR0_EL1.SVE
        → if 0b0000: SVE absent; VCPU cannot be SVE-capable; halt

Step 2: Read ID_AA64ZFR0_EL1.SVEver
        → if != 0b0001: SVE2 absent; VCPU cannot use SVE2 instruction set; halt

Step 3: Check KVM_CAP_ARM_SVE via ioctl(KVM_CHECK_EXTENSION)
        → if 0: kernel does not support SVE virtualisation; halt

Step 4: Negotiate VL via KVM_SET_ONE_REG(KVM_REG_ARM64_SVE_VLS)
        → VL is fixed at this point; no subsequent modification permitted
```

Bypassing steps 1 through 3 before `KVM_SET_ONE_REG(KVM_REG_ARM64_SVE_VLS)` is a configuration error.

## Register State

| Component | Width | Correctness role |
| --- | --- | --- |
| `Z₀..Z₃₁` | `VL` bits each | 32 full-width vector registers. |
| `P₀..P₁₅` | `VL/8` bits each | Predicate registers; one bit per byte lane. |
| `FFR` | `VL/8` bits | First-Fault Register for fault-first loads. |
| `VL` | scalar | Vector length in bits: `{128, 256, 512, 1024, 2048}`. |

`VL` is invariant after configuration:

```text
I₃: VL ∈ {128, 256, 512, 1024, 2048} ∧ VL ≤ VL_max(substrate)
I₄: VL invariant ∀ t ≥ t_configured
```

`ZCR_EL2.LEN = (VL / 128) - 1`; guest `ZCR_EL1.LEN` is clamped to `min(ZCR_EL1.LEN, ZCR_EL2.LEN)`.

`RDVL Xn, #1` must return `VL / 8` bytes:

```text
I₁₁: RDVL result × 8 = configured VL; divergence is an observable fault
```

## Save/Restore Ordering

`SVE_SAVE_EXTRA` save ordering is mandatory:

```text
SAVE:
  1. Z₀..Z₃₁    (full VL-width; 32 × VL/8 bytes)
  2. P₀..P₁₅    (VL/64 bytes each; 16 × VL/64 bytes total)
  3. FFR         (VL/64 bytes; saved last)

RESTORE:
  1. FFR
  2. P₀..P₁₅
  3. Z₀..Z₃₁
```

Save-area size is allocated once at VCPU creation:

```text
(32 × VL/8) + (16 × VL/64) + (VL/64) bytes
```

Violation of this ordering is invariant `I₆` failure.

## NTT Cryptographic Execution Constraints

For CRYSTALS-Kyber / CRYSTALS-Dilithium NTT execution over SVE2:

```text
- NTT butterfly: SQRDMLAH, MUL Zd.H, Zn.H, Zm.H[idx]
- Active lanes gated by P₀..P₃; FFR not used in NTT inner loop
- No data-dependent branch in NTT execution path
- No data-dependent memory access pattern in NTT coefficient array traversal
- VL-invariance (I₄) is a prerequisite; transform length = k × VL; k fixed at initialisation
- Constant-time assertion: ∀ inputs of equal length, execution time is identical
```
