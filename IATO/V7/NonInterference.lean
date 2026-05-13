import Mathlib.Order.Relation
import Mathlib.Data.Rel
import Mathlib.Tactic

namespace IATO.V7

structure TransitionSystem (State : Type _) where
  step : State → State → Prop

variable {State Domain Obs : Type _}

abbrev obsView (view : Domain → State → Obs) (d : Domain) (s : State) : Obs := view d s

abbrev lowEq (view : Domain → State → Obs) (d : Domain) (s₁ s₂ : State) : Prop :=
  obsView view d s₁ = obsView view d s₂

abbrev output_consistency (view : Domain → State → Obs) : Prop :=
  ∀ d s₁ s₂, lowEq view d s₁ s₂ → obsView view d s₁ = obsView view d s₂

abbrev step_consistency (T : TransitionSystem State) (view : Domain → State → Obs) : Prop :=
  ∀ d s₁ s₂ s₁' s₂', lowEq view d s₁ s₂ → T.step s₁ s₁' → T.step s₂ s₂' → lowEq view d s₁' s₂'

abbrev local_respect (T : TransitionSystem State) (view : Domain → State → Obs) : Prop :=
  ∀ d s s', T.step s s' → lowEq view d s s'

theorem nonInterference
    (T : TransitionSystem State)
    (view : Domain → State → Obs)
    (hOut : output_consistency view)
    (hStep : step_consistency T view)
    (hLocal : local_respect T view) :
    ∀ d s₁ s₂ s₁' s₂', Relation.TransGen T.step s₁ s₁' → Relation.TransGen T.step s₂ s₂' →
      lowEq view d s₁ s₂ → lowEq view d s₁' s₂' := by
  -- TODO(proof): nonInterference — lift unwinding obligations over Relation.TransGen closures.
  admit

end IATO.V7
