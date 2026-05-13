import Mathlib.Data.Finset.Lattice
import Mathlib.Logic.Function.Basic
import Mathlib.Tactic
import IATO.V7.Basic

namespace IATO.V7

structure LegacyWorker (Domain α : Type _) where
  domain : Domain
  deps : DepSet α

structure ScanReport (Domain α : Type _) where
  workers : List (LegacyWorker Domain α)
  conflicts : List (DepSet α × DepSet α)

def scanLegacy {Domain α : Type _} [DecidableEq α] (w : LegacyWorker Domain α) : DepSet α := w.deps

def scanAll {Domain α : Type _} [DecidableEq α] (ws : List (LegacyWorker Domain α)) : List (DepSet α) :=
  ws.map scanLegacy

def conflicts {α : Type _} [DecidableEq α] (xs : List (DepSet α)) : List (DepSet α × DepSet α) :=
  xs.bind (fun x => xs.filterMap (fun y => if Disjoint x y then none else some (x, y)))

def parseDomain {Domain : Type _} (f : Domain → Domain) := f

theorem scanLegacy_sound {Domain α : Type _} [DecidableEq α] (w : LegacyWorker Domain α) :
    scanLegacy w = w.deps := by rfl

theorem scanAll_incompat_mono {Domain α : Type _} [DecidableEq α]
    (ws₁ ws₂ : List (LegacyWorker Domain α))
    (hsub : ws₁ <+ ws₂) :
    (scanAll ws₁).length ≤ (scanAll ws₂).length := by
  -- TODO(proof): scanAll_incompat_mono — prove scan length monotonic under list embedding.
  admit

theorem parseDomain_idem {Domain : Type _} (f : Domain → Domain)
    (h : Function.Idempotent f) : Function.Idempotent (parseDomain f) := by
  simpa [parseDomain] using h

end IATO.V7
