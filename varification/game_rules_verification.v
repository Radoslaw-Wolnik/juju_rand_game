(* File: verification/game_rules_verification.v *)

Require Import Coq.Init.Nat.
Require Import Coq.Lists.List.
Import ListNotations.

(* Define the game state *)
Record GameState := mkGameState {
  board: nat;
  score: nat;
  round: nat
}.

(* Define a move *)
Definition Move := nat.

(* Define the rules for a valid move *)
Definition is_valid_move (state: GameState) (move: Move) : Prop :=
  move <= 12 /\
  exists k, 2^(move-1) = k /\ (board state) mod (2^move) >= k.

(* Define the effect of a move on the game state *)
Definition apply_move (state: GameState) (move: Move) : GameState :=
  mkGameState
    ((board state) - 2^(move-1))
    (score state)
    (round state).

(* Theorem: A valid move always reduces the board state *)
Theorem valid_move_reduces_board:
  forall state move,
    is_valid_move state move ->
    board (apply_move state move) < board state.
Proof.
  intros state move H_valid.
  unfold is_valid_move in H_valid.
  destruct H_valid as [H_bound [k [H_k H_board]]].
  unfold apply_move, board.
  rewrite <- H_k.
  apply Nat.sub_lt.
  - assumption.
  - apply Nat.pow_pos_nonneg; auto with arith.
Qed.

(* More theorems and proofs would follow... *)