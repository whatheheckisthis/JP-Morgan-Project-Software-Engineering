# Microarchitectural Threat Model

## Scope

`S_Microarch = ⟨BTB_state, BHB_state, LLC_state, RSB_state, pipeline_state⟩` is not directly readable through architectural instructions, but it is observable through timing and predictor side effects. Correctness includes non-observability of protected state under adversarial probing.

## Threat Classes

### Spectre-v1: Bounds-Check Bypass

Speculative execution past a bounds check can load an out-of-bounds address into cache. The resulting `LLC_state` delta may be observed by an attacker VCPU through Prime+Probe or Flush+Reload.

### Spectre-v2: Branch Target Injection

An attacker can poison `BTB_state` to redirect speculative EL2 indirect branches toward gadgets. Neoverse N2/V3 mitigation requires `FEAT_CSV2_2` support and explicit entry-path hardening. `CLEARBHB` is issued before the first speculative EL2 instruction.

### Spectre-BHB

Guest-controlled `BHB_state` can influence EL2 indirect branch prediction across privilege boundaries. `CLEARBHB` or `SMCCC_ARCH_WORKAROUND_3` is required at every EL2 entry; RSB stuffing alone is insufficient.

### Meltdown-Class Privilege Boundary Loads

Speculative reads of EL2-mapped physical memory from EL1 are mitigated by ensuring no EL2 PA range appears in Stage-2 mappings. EL1 page tables must not contain EL2 PA ranges at any privilege level.

### Cache Timing Side Channels

Neoverse N2/V3 LLC is not partitioned by VMID. Co-resident VCPUs may observe `LLC_state` through timing. VCPU pinning and co-residency constraints belong to scheduling policy; they are not assumed absent in the correctness model.

### RSB Underflow

`RSB_state` underflow can fall back to BTB-based return prediction. EL2 entry paths perform RSB stuffing before any `RET`-based EL2 control flow executes.

### Pipeline State Observability

`pipeline_state` affects execution latency. Constant-time constraints apply to EL2 paths that process guest-controlled data, including SVE2 predicate evaluation and Stage-2 fault handlers.

## Non-Interference Property

```text
∀ attacker VCPU A, victim VCPU B:
  obs(S_Microarch | execution of B) is independent of B's protected memory contents
```

This is a non-interference property. It is not implied by architectural isolation alone.

## BRBE Isolation Obligations

For `FEAT_BRBE` on Neoverse N2:

- `BRBCR_EL1.E0BRE = 0` and `BRBCR_EL1.EXCEPTION = 0` before VM entry unless guest BRBE access is explicitly granted.
- BRBE state is not shared across VCPU boundaries.
- At VM exit, BRBE is either context-saved/restored or flushed.

A guest that can infer EL2 branch history through BRBE violates isolation correctness and microarchitectural non-interference.
