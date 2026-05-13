# Stage-2 Memory Isolation Correctness

## Scope

`S_Stage2 = ⟨VMID, IPA→PA_table, fault_log⟩` models hardware-enforced second-stage address translation for EL1/EL0 memory accesses. Stage-2 translation is not a software policy check; it is the MMU traversal controlled by `VTTBR_EL2`, `VTCR_EL2`, and the Stage-2 translation tables.

## Translation Model

```text
IPA → PA
```

Every guest physical access that produces an IPA is retranslated by the Stage-2 MMU before a physical bus transaction. EL1 has no instruction that modifies `IPA→PA_table` and no visibility into EL2-owned Stage-2 mappings.

## VTTBR_EL2

`VTTBR_EL2` contains:

- `BADDR[47:1]`: base physical address of the Stage-2 Level-0 or Level-1 root table.
- `VMID[63:48]` or `VMID[55:48]`: width selected by `VTCR_EL2.VS`.

`VMID(VTTBR_EL2)` uniqueness is invariant across all concurrently running VCPUs:

```text
I₈: VMID(VTTBR_EL2) unique across all S_VCPU = running instances
```

VMID reuse without `VMALLS12E1IS` is a correctness violation because stale Stage-2 TLB entries are VMID-tagged.

## VTCR_EL2 Constraints

| Field | Constraint |
| --- | --- |
| `T0SZ` | IPA address space size is `64 - T0SZ` bits and matches guest RAM layout. |
| `SL0` | Starting level of Stage-2 walk is determined from `T0SZ` per ARM ARM Table D8-7. |
| `IRGN0` | Inner cacheability for Stage-2 table walks. |
| `ORGN0` | Outer cacheability for Stage-2 table walks. |
| `SH0` | Shareability domain for Stage-2 table walks. |
| `PS` | Output physical address size is no larger than the host PA range. |
| `VS` | VMID width: `0` for 8-bit VMIDs or `1` for 16-bit VMIDs. |

`VTCR_EL2.PS` is fixed at VCPU creation and supports:

```text
I₂: VTCR_EL2.PS ≥ host_PA_range at VCPU creation; not modifiable thereafter
```

## Isolation Invariants

```text
I_Stage2_1: ∀ guest IPA not in Stage-2 table → Stage-2 fault → EL2 trap
I_Stage2_2: ∀ device MMIO IPA → Stage-2 PTE.MemAttr = nGnRE or nGnRnE
I_Stage2_3: VMID(VTTBR_EL2) unique across all S_VCPU = running instances
I_Stage2_4: No guest IPA maps to EL2-private PA regions
I_Stage2_5: (on V3/RME hosts) No guest IPA maps to Realm PAS or Secure PAS
```

Device MMIO ranges, including Azure Boost NVMe and network datapaths, are represented as Device-nGnRE or Device-nGnRnE Stage-2 mappings. Normal-cacheable Stage-2 mappings are not used for Azure Boost device ranges.

## RME / CCA Constraints

On Neoverse V3 hosts with RME/CCA:

```text
I_RME_1: No VTTBR_EL2 entry references Realm PAS granules
I_RME_2: No VTTBR_EL2 entry references Secure PAS granules
I_RME_3: Granule Protection Table at EL3 enforces PAS partitioning;
          Stage-2 faults on Realm/Secure granules are GPT-enforced
```

Stage-2 table faults involving Realm PAS or Secure PAS are GPT-enforced architectural events, not software assertions.
