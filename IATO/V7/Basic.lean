import Mathlib.Order.Lattice
import Mathlib.Data.Finset.Lattice
import Mathlib.Tactic

/-!
Import DAG:
Basic
├─ Mathlib.Order.Lattice
├─ Mathlib.Data.Finset.Lattice
└─ Mathlib.Tactic
-/

namespace IATO.V7

class SecurityLabel (α : Type _) extends CompleteLattice α

abbrev DepSet (α : Type _) := Finset α

abbrev CanFlow {α : Type _} [LE α] (a b : α) : Prop := a ≤ b

theorem depSet_disjoint_separation {α : Type _} [DecidableEq α]
    (s t : DepSet α) : Disjoint s t ↔ s ∩ t = (∅ : DepSet α) := by
  simpa [Finset.disjoint_left] using Finset.disjoint_iff_inter_eq_empty

theorem deps_join_monotone_left {α : Type _} [Sup α] [LE α] [SemilatticeSup α] (a b : α) :
    a ≤ a ⊔ b := by
  exact le_sup_left

end IATO.V7
