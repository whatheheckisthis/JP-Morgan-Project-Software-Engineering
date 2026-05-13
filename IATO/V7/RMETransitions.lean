import IATO.V7.NonInterference
import Mathlib.Order.Hom.CompleteLattice
import Mathlib.Tactic

namespace IATO.V7

inductive EL where | EL0 | EL1 | EL2 | EL3 deriving DecidableEq

structure MachineWorld where
  el : EL
  realm_id : Nat
  secure_state : Bool

inductive RMEStep : MachineWorld → MachineWorld → Prop
  | realm_entry (s : MachineWorld) : RMEStep s { s with el := EL.EL3, secure_state := true }
  | realm_exit (s : MachineWorld) : RMEStep s { s with el := EL.EL1, secure_state := false }
  | switch_realm (s : MachineWorld) (rid : Nat) : RMEStep s { s with realm_id := rid }
  | stay (s : MachineWorld) : RMEStep s s

def realmObserverView (d : Nat) (s : MachineWorld) : EL × Bool :=
  if s.realm_id = d then (s.el, s.secure_state) else (EL.EL0, false)

theorem realm_entry_preserves_lowEq (d : Nat) (s : MachineWorld) :
    lowEq realmObserverView d s { s with el := EL.EL3, secure_state := true } := by
  -- TODO(proof): realm_entry_preserves_lowEq — prove observer-equivalence side conditions for matching realm.
  admit

def secureLift : MachineWorld →o MachineWorld where
  toFun s := { s with secure_state := true }
  monotone' := by
    intro a b hab
    -- TODO(proof): secureLift.monotone — instantiate order on MachineWorld and show monotonicity.
    admit

end IATO.V7
