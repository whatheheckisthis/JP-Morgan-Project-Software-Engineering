# Parameterised SVE State Semantics under Virtualisation-Structured Dynamics
>This repository contains a formal semantic framework for reasoning about observational equivalence and refinement over parameterised SVE execution state under abstract virtualisation-mediated transitions.
The system models vector-length indexed execution state as a first-class semantic object and defines a labelled transition structure over architectural projections. The framework deliberately excludes hardware, hypervisor, scheduling, MMU, and implementation-specific execution mechanisms, treating execution infrastructure as an abstract correctness-preserving environment.
The repository is oriented toward:

- refinement semantics,
- bisimulation structure,
- vector-length indexed state reasoning,
- migration invariance,
- observational equivalence,
- and formally structured transition systems.

---

# Semantic Configuration Model
A configuration is defined as:

```text
C = (v, σ)

where:

v  ∈ V(λ)
σ  ∈ Σ
λ ∈ {128, 256, 512}

* v denotes vector-length indexed state
* σ denotes an abstract architectural projection
* each vector length induces a distinct state space
```

The framework does not model:

* EL2 implementation,
* Stage-2 MMU behaviour,
* VMID allocation,
* scheduling,
* microarchitectural state,
* or hardware execution internals.

These are treated as abstract execution infrastructure external to the semantic system.



Observational Semantics

Observation is defined through an observation function:
```text
O : C → O
```
Observational equivalence is defined as:

```text
C₁ ≈ C₂  ⇔  O(C₁) = O(C₂)
```

Equivalence is defined only over observable architectural behaviour.
The semantic system isolates vector-length indexed structure as the sole divergence dimension under transition and reconfiguration.



Transition System

The framework defines a labelled transition system:
```text
C ─α→ C′

where:
α ∈ {exec, mig, reconf}
```
Transition interpretation:

* exec   : execution transition within a fixed vector-length space
* mig    : abstract migration transformation
* reconf : vector-length reconfiguration transition

Execution preserves vector length.
Reconfiguration permits transition across vector-length indexed state spaces.



Embedding Between Vector-Length Spaces

An embedding is defined between vector-length indexed state spaces:

ι_λ₁→λ₂ : V(λ₁) → V(λ₂)

such that:

λ₁ ≤ λ₂

and:

O(ι(v)) = O(v)

The embedding acts as a structure-preserving injection across vector-length configurations.



Refinement Relation

Refinement is defined as a forward simulation relation:

C₁ ⊑ C₂

iff for all transitions:

C₂ ─α→ C′₂
⇒
∃C′₁ :
C₁ ─α→ C′₁ ∧ C′₁ ≈ C′₂

The refinement relation establishes behavioural preservation under execution, migration, and reconfiguration transitions.



Bisimulation

A relation R is a bisimulation if:

C₁ R C₂ ⇒ C₁ ≈ C₂

and transitions are matched in both directions.

The framework uses bisimulation to reason about observational equivalence preservation across parameterised execution configurations.



Main Preservation Result

Migration preservation is expressed as:
```
C_H ⊑ C_G
⇒
O(mig(C_H)) = O(mig(C_G))
```
The proof structure is established through:

* embedding preservation,
* refinement stability,
* migration invariance,
* trace preservation,
* and transition commutativity.



Core Semantic Properties

P₁:  Each vector length induces a distinct indexed state space
P₂:  Observational equivalence depends only on architectural projection σ
P₃:  Embedding preserves observable behaviour
P₄:  Refinement is stable under transition execution
P₅:  Migration preserves observational semantics
P₆:  Refinement is preserved over execution traces
P₇:  Transition application commutes with vector-state embedding



Repository Structure

* semantic_foundations.md
    * vector-length indexed state definitions
    * configuration well-formedness
    * state-space construction
* observational_semantics.md
    * observation function
    * observational equivalence
    * architectural projection semantics
* transition_system.md
    * labelled transition structure
    * execution, migration, and reconfiguration semantics
* embedding_relations.md
    * vector-length embeddings
    * structure-preserving injections
    * observational preservation
* refinement.md
    * forward simulation relations
    * refinement stability
    * trace preservation
* bisimulation.md
    * bisimulation structure
    * coinductive equivalence relations
    * behavioural correspondence
* migration_preservation.md
    * migration invariance theorem
    * preservation lemmas
    * commutativity properties
* lean/
    * formal theorem structure
    * semantic encodings
    * proof scaffolding
    * relational constructions



Scope

This repository is a semantic and relational formalisation framework.

It is not:

* a hardware implementation,
* a hypervisor implementation,
* a kernel engineering project,
* a QEMU/KVM deployment repository,
* or a microarchitectural execution model.

The system models abstract semantic preservation properties over parameterised SVE execution state under virtualisation-structured dynamics.