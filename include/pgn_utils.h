#ifndef PGN_UTILS_H
#define PGN_UTILS_H

#include <time.h>
#include "general_utils.h"

typedef struct {
    char *event; // some events have extremely long names
    struct tm *date;
    char white_player[100]; // some people on the internet have long names
    char black_player[100];
    char time_control[20];  // Fixed size for time format (e.g., "10", "10+3", "600+0")
    char *pgn_string;
    int white_elo;
    int black_elo;
    enum GameResult game_result; // can be "1-0", "0-1", "1/2-1/2"
    enum Termination termination; // can be "Normal", "Time forfeit" , "Abandoned"
    char opening_name[100];
} PGN;

#endif