# Chess Data Pipeline

## Overview
This repository is part of a larger personal project to build a chess engine using C, C++, and machine learning.

The goal of this module is to process Lichess game data from `.pgn.zst` files and convert it into a machine-learning-friendly CSV dataset.

## Goal
The intended dataset format, referred to here as **FEN+**, has one row per move and is designed for training a model that predicts chess moves from positions.

Each row is intended to contain:

- **Time Format**: the game time control, for example `10` or `10+3`
- **Move Number**: the move number within the game
- **FEN**: the board state before the move in Forsyth-Edwards Notation
- **ELO**: the Lichess rating of the player making the move
- **Next Move**: the move played from that position in UCI notation, for example `e2e4`

Example output row:

```csv
time_format,move_number,fen,elo,uci_move
10+3,1,rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1,1650,e2e4
```

## Current Status
This project is still in an early implementation stage.

Implemented today:

- FEN parsing in C
- FEN serialization in C
- Basic extraction of SAN move tokens from simple PGN move strings
- Unit tests for the current C parsing utilities
- Early Python prototypes for board and piece modeling

Not implemented yet:

- Streaming and processing real `.pgn.zst` Lichess dumps
- Robust PGN parsing for full real-world game records
- SAN-to-UCI conversion
- End-to-end CSV export for the target dataset

## Development Approach
Part of the purpose of this project is to build experience with C by implementing the final pipeline in C.

The `src_python/` directory contains prototype code used to explore ideas more quickly in Python before rewriting stable logic in C.

## Input
- Lichess game dumps in `.pgn.zst` format
- PGN game records containing move text and metadata

## Intended Output
- CSV rows in the format `time_format,move_number,fen,elo,uci_move`

## Repository Structure
```text
chess_data_pipeline/
├── include/                 # C header files
├── src/                     # C source files
├── src_python/              # Python prototypes and experiments
├── test/                    # C test programs
├── src_python/test/         # Python tests and PGN test artifacts
├── README.md                # Project overview and goals
├── .cursor-config.json      # Local editor/agent configuration
└── Makefile                 # Build and test commands
```

## Build and Test
Current development is centered on the C utilities and tests.

Requirements:

- `gcc`
- `make`
- `libzstd`

Run the test suite with:

```sh
make test
```

This currently builds and runs the C test programs in `test/`.

## Dependencies
- `libzstd` for future `.zst` file support

## Roadmap
- Efficient streaming of large `.zst` files without full decompression
- Accurate PGN parsing
- SAN-to-UCI move translation
- CSV writer for the FEN+ dataset
- Additional unit tests and benchmarks

