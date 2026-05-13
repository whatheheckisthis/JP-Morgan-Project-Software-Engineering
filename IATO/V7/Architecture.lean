import Mathlib.Order.Hom.CompleteLattice
import Mathlib.Order.GaloisConnection
import Mathlib.Tactic
import IATO.V7.Basic

namespace IATO.V7

variable {α : Type _} [DecidableEq α]

def pairwiseDisjointDeps (deps : List (DepSet α)) : Prop :=
  List.Pairwise Disjoint deps

def architectureSecure (deps : List (DepSet α)) : Prop :=
  pairwiseDisjointDeps deps

theorem architectureSecure_of_pairwise (deps : List (DepSet α)) :
    pairwiseDisjointDeps deps → architectureSecure deps := by
  intro h
  exact h

theorem secure_under_refinement (deps : List (DepSet α))
    (policyRefine : DepSet α →o DepSet α)
    (hSec : architectureSecure deps) : architectureSecure (deps.map policyRefine) := by
  -- TODO(proof): secure_under_refinement — show Pairwise Disjoint preserved by monotone policy refinement.
  admit

end IATO.V7
