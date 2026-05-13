import Mathlib.Order.GaloisConnection
import Mathlib.Tactic

namespace IATO.V7

structure ExecState where
  ctl : Nat
  ok : Bool

structure ControlEvidence where
  reqCtl : Nat


def α : ExecState → ControlEvidence := fun s => ⟨s.ctl⟩

def γ : ControlEvidence → Set ExecState := fun e => {s | e.reqCtl ≤ s.ctl}

theorem compliance_gc : GaloisConnection α γ := by
  intro s e
  -- TODO(proof): compliance_gc — prove α/γ adjunction between control abstraction and concretization.
  admit

end IATO.V7
