# AGENTS.md

## Purpose

This file defines rules and constraints for AI coding agents (Codex, Copilot, Cursor, etc.) working in this repository.

Agents must follow these rules strictly to protect privacy, maintain code quality, and avoid unintended changes.

---

# 1. Workspace Boundaries (Privacy Rules)

The agent is **strictly limited to this repository**, except for the
specific Codex configuration path listed below.

Allowed paths:

* This repository directory
* Subdirectories inside this repository

The agent **must never access or attempt to access**:

* Parent directories (`..`)
* Other directories under `~/`
* System directories (`/etc`, `/usr`, `/home`, etc.)
* SSH keys
* git credentials
* browser data
* environment secrets
* other repositories

The only allowed external directory is:

```
~/.codex
```

This is used only for Codex configuration.

If a task would require accessing files outside the repository or
`~/.codex`, the agent must **refuse and explain why**.

---

# 2. Repository Scope

This project implements a **chess data pipeline**.

Primary goals:

* Read chess games from PGN files
* Parse game metadata and moves
* Transform data into structured representations
* Export processed data for analysis or ML pipelines

The repository contains:

* PGN parsing utilities
* chess move processing
* board reconstruction
* dataset generation

The system must remain **deterministic and reproducible**.

---

# 3. General Development Rules

When modifying code, agents must follow these principles.

## Minimal Changes

Make the **smallest possible patch** required to solve the problem.

Do not refactor unrelated code.

Do not rename files or move modules unless explicitly requested.

If another rule requires related updates, such as tests or
documentation, those updates are still considered part of the minimal
acceptable patch.

---

## Preserve Existing APIs

Existing interfaces should remain stable unless the user explicitly asks for a change.

Breaking changes must be avoided.

---

## Readability First

Prefer:

* clear code
* simple functions
* explicit logic

Avoid:

* clever one-liners
* unnecessary abstraction
* micro-optimizations that reduce readability.

---

## Function Size

Functions should ideally remain under **50–60 lines**.

This is a guideline, not a strict limit.

If logic becomes complex, prefer splitting into helper functions, but do
not perform unrelated refactors solely to satisfy this guideline.

---

# 4. Testing Requirements

Any change that affects behavior must include tests.

Agents should:

* add unit tests
* extend existing tests
* ensure edge cases are covered

Typical edge cases in this project include:

* malformed PGN
* illegal moves
* missing metadata
* incomplete games
* unusual notation

Tests should live in:

```
tests/
```

Adding or updating tests required by a behavioral change does not count
as an unnecessary multi-file refactor.

---

# 5. Chess Domain Rules

The chess logic must remain correct and deterministic.

Important invariants:

* Board state must always represent a valid chess position.
* Move application must be deterministic.
* PGN parsing must not mutate global state.
* Board reconstruction must follow move order strictly.
* Illegal moves should raise clear errors.

Agents must never silently ignore parsing errors.

---

# 6. Performance Considerations

This project may process **large PGN datasets**.

Agents should avoid:

* loading entire large files unnecessarily
* excessive memory allocation
* repeated parsing passes

Prefer:

* streaming approaches
* iterators/generators
* incremental parsing

---

# 7. File Modification Policy

Agents must follow this workflow before editing:

1. Inspect the relevant files
2. Explain the intended change
3. Apply a minimal patch
4. Summarize the modifications

Agents should not perform large multi-file refactors without explicit approval.

Required companion edits in tests or documentation are allowed when they
are necessary to keep the repository correct and consistent.

---

# 8. Security Rules

Agents must never:

* execute arbitrary shell commands unrelated to the task
* install packages without approval
* access network resources without approval
* modify system configuration
* modify git credentials

Shell commands must be limited to development tasks such as:

* running tests
* formatting code
* building the project

---

# 9. Documentation Updates

When behavior changes, the agent should update:

* README.md
* relevant docstrings
* comments where necessary

Documentation should always match code behavior.

Documentation updates that are necessary to reflect a behavioral change
are part of the expected patch, even when this touches more than one
file.

---

# 10. When Unsure

If instructions are ambiguous or risky, the agent must:

* stop
* explain the uncertainty
* ask for clarification

Never guess when the change could break correctness.

Routine implementation choices that do not create a meaningful risk to
correctness do not require clarification.

When rules appear to compete, agents should use this priority order:

1. Privacy and security rules
2. Correctness and determinism
3. Explicit user instructions
4. Required tests and documentation consistency
5. Minimal changes and readability guidelines

---

# Summary

Agents working in this repository must prioritize:

* privacy
* correctness
* minimal changes
* deterministic behavior
* clear and readable code
