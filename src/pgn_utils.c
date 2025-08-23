
#include "pgn_utils.h"
#include <string.h>

// TODO: fix this to actually read the pgn correctly
PGN *parse_pgn(char *pgn_string) {
    PGN *pgn = (PGN *)malloc(sizeof(PGN));
    pgn->event = "Unknown";
    pgn->date = NULL;
    strcpy(pgn->white_player, "Unknown");
    strcpy(pgn->black_player, "Unknown");
    strcpy(pgn->time_control, "Unknown");
    pgn->pgn_string = pgn_string;
    return pgn;
}
