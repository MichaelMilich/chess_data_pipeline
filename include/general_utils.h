#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
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
    char notation[10];    // e.g., "e4", "Nf3", "O-O"
} san_move;

// Move struct with union to handle both UCI and SAN formats
typedef struct {
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

char *get_termination_string(enum Termination termination);
char *get_game_result_string(enum GameResult game_result);
Move *get_move_from_uci(char *uci_move);
bool string_to_int(const char *str, int *out) ;



#endif