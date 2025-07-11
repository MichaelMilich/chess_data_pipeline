#ifndef FEN_UTILS_H
#define FEN_UTILS_H

#include <string>
#include "general_utils.h"

typedef struct {
    char time_control[20];  // Fixed size for time format (e.g., "10", "10+3", "600+0")
    int move_number;
    char fen[100];         // Fixed size for FEN string (standard FEN is ~90 chars max)
    int elo;
    struct Move move;
} FEN_Plus;


FEN_Plus *starting_position_fen_plus(char *time_control, int elo, Move *uci_move);
FEN_Plus *next_fen_plus(FEN_Plus *fen_plus);
FEN_Plus **generate_list_of_fen_plus(FEN_Plus *starting_fen_plus, char *pgn_string);
void free_fen_plus(FEN_Plus *fen_plus);





#endif