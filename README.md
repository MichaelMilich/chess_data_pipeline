# Chess Data Pipeline

## Overview
This project is part of a larger personal project to build a chess engine using C, C++, and Machine Learning.  
The goal of this module is to process the Lichess game database (in `.pgn.zst` format) and transform it into a machine-learning-friendly CSV format.

## Purpose
Extract relevant training data for a machine learning model that predicts chess moves.  
The CSV format, referred to as **FEN+**, will include one row per move and contain:

- **Time Format** - String representing the game time limit and if there are any increments. (e.g. `10` for 10 minutes or `10+3` for a 10 minute game where each move buys 3 more seconds to the clock)
- **Move Number** — integer indicating which move in the game this is.
- **FEN** — board state before the move (standard Forsyth–Edwards Notation).
- **ELO** — Lichess rating of the player who made the move.
- **Next Move** — the move taken from the FEN position, represented in **UCI** notation (e.g. `e2e4`).

## Input
- File format: `.pgn.zst` (Lichess compressed PGN dump)
- Each file contains multiple games in PGN format.

## Output
- CSV file containing rows in the format: time_format,move_number,fen,elo,uci_move

## Project Structure
chess_data_pipeline/
├── src/ # Core C source files
├── include/ # Header files
├── lichess_database/ # Input PGN and output CSV files, the whole directory is in gitignore
├── scripts/ # Helper scripts (decompression, test runners, etc.)
├── README.md # Project description and goals
├── .cursor-config.json # Custom config for Cursor context
└── Makefile # Build system

## Dependencies
- `libzstd`: for reading `.zst` files inline

## Development Goals
- 🚧 Efficient streaming of large `.zst` files without full decompression
- 🚧 Accurate PGN parsing
- 🚧 Clean and modular C implementation
- 🚧 FEN state generator
- 🚧 CSV writer with UCI move annotations
- 🚧 Unit tests and benchmarks


