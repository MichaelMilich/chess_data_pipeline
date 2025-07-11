#include "general_utils.h"

char *get_termination_string(enum Termination termination) {
    switch (termination) {
        case NORMAL:
            return "Normal";
        case TIME_FORFEIT:
            return "Time forfeit";
        case ABANDONED:
            return "Abandoned";
    }
}

char *get_game_result_string(enum GameResult game_result) {
    switch (game_result) {
        case WHITE_WINS:
            return "1-0";
        case BLACK_WINS:
            return "0-1";
        case DRAW:
            return "1/2-1/2";
    }
}


