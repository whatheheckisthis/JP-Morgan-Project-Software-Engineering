import Mathlib.Order.Monotone.Basic
import Mathlib.Order.Relation
import Mathlib.Data.Rel
import Mathlib.Tactic
import IATO.V7.Basic

namespace IATO.V7

structure Worker (Domain α : Type _) where
  domain : Domain
  deps : DepSet α

structure Step (σ : Type _) where
  run : σ → σ → Prop

abbrev lowEqW {σ Domain Obs : Type _} (view : Domain → σ → Obs) (d : Domain) (s₁ s₂ : σ) : Prop :=
  view d s₁ = view d s₂

def locallyRespecting {σ Domain Obs α : Type _} (w : Worker Domain α)
    (st : Step σ) (view : Domain → σ → Obs) : Prop :=
  ∀ s s', st.run s s' → lowEqW view w.domain s s'

def compose {α : Type _} [SemilatticeSup α] [OrderBot α] (ws : List (Worker Unit α)) : α :=
  ws.foldl (fun acc w => acc ⊔ (w.deps.sup id)) ⊥

theorem compose_deps_upper_bound {α : Type _} [SemilatticeSup α] [OrderBot α]
    (ws : List (Worker Unit α)) (x : α)
    (hx : ∀ w ∈ ws, w.deps.sup id ≤ x) : compose ws ≤ x := by
  -- TODO(proof): compose_deps_upper_bound — establish foldl sup upper-bound induction.
  admit

end IATO.V7
