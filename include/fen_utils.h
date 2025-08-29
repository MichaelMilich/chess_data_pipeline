#ifndef FEN_UTILS_H
#define FEN_UTILS_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

// UCI move representation (e.g., "e2e4", "g8f6")
typedef struct {
    char from_square[3];  // e.g., "e2"
    char to_square[3];    // e.g., "e4"
    char promotion[2];    // e.g., "q" for queen promotion, "" if no promotion
} uci_move;

// SAN move representation (e.g., "e4", "Nf3", "O-O")
typedef struct {
    char notation[32];    // e.g., "e4", "Nf3", "O-O"
} san_move;

// Move struct with union to handle both UCI and SAN formats
typedef struct {
    char player;        // 'w' for white, 'b' for black
    char *type;          // "uci" or "san" to distinguish between formats
    union {
        uci_move uci;
        san_move san;
    } move_data;
} Move;

enum Termination {
    NORMAL,
    TIME_FORFEIT,
    ABANDONED
};

enum GameResult {
    WHITE_WINS,
    BLACK_WINS,
    DRAW
};

typedef struct {
    char board[8][8];
    int move_number;
    int en_passant_square_file; // a-h
    int en_passant_square_rank; // 1-8
    int side_to_move; // 0-white, 1-black
    char castling_rights[5]; //  max as "KQkq", min as "-"
    int halfmove_clock;
    int fullmove_number;
} FEN_Board;

typedef struct {
    char time_control[20];  // Fixed size for time format (e.g., "10", "10+3", "600+0")
    FEN_Board* board;
    int elo;
    Move* move;
} FEN_Plus;


char *get_termination_string(enum Termination termination);
char *get_game_result_string(enum GameResult game_result);
Move *get_move_from_uci(char *uci_move);
bool string_to_int(const char *str, int *out);


int get_move_numbers_from_pgn_string(const char *pgn_string);
Move **get_moves_from_pgn_string(const char *pgn_string);

FEN_Board *create_fen_board(char *fen_string);
bool fen_board_to_fen_string(FEN_Board *board, char *fen_string_out);
bool free_fen_plus(FEN_Plus *fen_plus);
FEN_Board *generate_starting_position_fen();

bool translate_san_to_uci(Move *san_move, FEN_Board *board);

#endif