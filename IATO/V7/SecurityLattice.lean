import IATO.V7.Basic
import Mathlib.Tactic

namespace IATO.V7

abbrev SecurityProduct (Lconf Lint : Type _) := Lconf × Lint

instance {Lconf Lint : Type _} [CompleteLattice Lconf] [CompleteLattice Lint] :
    CompleteLattice (SecurityProduct Lconf Lint) := inferInstance

instance {Lconf Lint : Type _} [CompleteLattice Lconf] [CompleteLattice Lint] :
    SecurityLabel (SecurityProduct Lconf Lint) where

 theorem flow_trans {Lconf Lint : Type _} [CompleteLattice Lconf] [CompleteLattice Lint]
    (a b c : SecurityProduct Lconf Lint) : CanFlow a b → CanFlow b c → CanFlow a c := by
  intro hab hbc
  exact le_trans hab hbc

theorem join_min_upper {Lconf Lint : Type _} [CompleteLattice Lconf] [CompleteLattice Lint]
    (a b : SecurityProduct Lconf Lint) : a ≤ a ⊔ b := by
  exact le_sup_left

theorem meet_max_lower {Lconf Lint : Type _} [CompleteLattice Lconf] [CompleteLattice Lint]
    (a b : SecurityProduct Lconf Lint) : a ⊓ b ≤ a := by
  exact inf_le_left

end IATO.V7
