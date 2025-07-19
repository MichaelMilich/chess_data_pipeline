#ifndef FEN_UTILS_H
#define FEN_UTILS_H

#include "general_utils.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

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
    FEN_Board board;
    int elo;
    Move move;
} FEN_Plus;


FEN_Plus *starting_position_fen_plus(char *time_control, int elo, Move *uci_move);
void next_fen_plus(FEN_Plus *fen_plus, FEN_Plus *next_fen_plus);
FEN_Plus **generate_list_of_fen_plus(FEN_Plus *starting_fen_plus, char *pgn_string);
void free_fen_plus(FEN_Plus *fen_plus);

bool create_fen_board(FEN_Board *output, char *fen_string);
bool fen_board_to_fen_string(FEN_Board *board, char *fen_string_out); // TODO: Implement this

#endif