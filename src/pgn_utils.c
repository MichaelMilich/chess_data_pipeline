
#include "pgn_utils.h"

PGN *parse_pgn(char *pgn_string) {
    PGN *pgn = (PGN *)malloc(sizeof(PGN));
    pgn->event = "Unknown";
    pgn->date = NULL;
    pgn->white_player = "Unknown";
    pgn->black_player = "Unknown";
    pgn->time_control = "Unknown";
    pgn->pgn_string = pgn_string;
    return pgn;
}
