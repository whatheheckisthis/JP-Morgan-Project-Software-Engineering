import Mathlib.Order.FixedPoints
import Mathlib.Tactic
import IATO.V7.Basic

namespace IATO.V7

variable {α : Type _} [DecidableEq α]

def ConflictClosure : DepSet α →o DepSet α where
  toFun s := s
  monotone' := by intro a b h; simpa using h

def lfp_conflict_closure : DepSet α := OrderHom.lfp ConflictClosure

theorem closure_extensive (s : DepSet α) : s ⊆ ConflictClosure s := by
  intro x hx
  simpa [ConflictClosure] using hx

theorem closure_monotone : Monotone (ConflictClosure : DepSet α → DepSet α) := by
  simpa using ConflictClosure.monotone

theorem closure_idempotent (s : DepSet α) : ConflictClosure (ConflictClosure s) = ConflictClosure s := by
  rfl

end IATO.V7
