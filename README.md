# Qurio: WIP Chess Engine

Qurio is a work-in-progress chess engine built from scratch. The goal is to learn, experiment, and create a competitive engine while exploring algorithms, heuristics, and neural evaluation techniques. 

---

## Planned search features

Once the fundamentals are implemented (negamax + alpha-beta pruning), the engine will progressively add the following search enhancements:

- [x] Iterative Deepening
- [x] Basic Move Ordering (captures via MVV-LVA)
- [x] Quiescence Search
- [x] Transposition Table (sort TT move first)
- [x] Butterfly history heuristic
- [ ] PVS
- [ ] Aspiration windows
- [ ] RFP
- [ ] Null Move Pruning (NMP)
- [ ] Late Move Reductions (LMR)
- [ ] Killer moves
- [ ] Late Move Pruning (LMP)
- [ ] Futility pruning
- [ ] Internal Iterative Reduction (IIR)
- [ ] Improving heuristic
- [ ] QS SEE pruning
- [ ] PVS SEE pruning (captures and quiets)
- [ ] Continuation history (CMH + FMH)
- [ ] Capture history heuristic
- [ ] History pruning
- [ ] Singular extensions
- [ ] Multicut (using singular search results)
- [ ] Double/triple/negative extensions
- [ ] Cutnode (as part of negative extensions, LMR, etc)
- [ ] Static eval correction history
- [ ] QS futility pruning

---

## Time management

After iterative deepening is in place, time management improvements will be added:

- Hard bound (applied to the entire search)
- Soft bound (checked at each new depth in the iterative deepening loop)
  - Node-based scaling
  - Best move stability
  - Eval stability

---

## Notes

- Parameter tweaking and testing will be applied incrementally after each feature addition.
- Minor features found in top engines may be added later.
- The focus is on learning, experimentation, and building a solid foundation for further improvements.
