#include "general_utils.h"

char *get_termination_string(enum Termination termination) {
    switch (termination) {
        case NORMAL:
            return "Normal";
        case TIME_FORFEIT:
            return "Time forfeit";
        case ABANDONED:
            return "Abandoned";
        default:
            return "Unknown";
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
        default:
            return "*";
    }
}


Move *get_move_from_uci(char *uci_move) {
    Move *move = (Move *)malloc(sizeof(Move));
    if (!move) return NULL;

    move->type = "uci";
    size_t len = strlen(uci_move);
    if (len != 4 && len != 5) {
        printf("Invalid UCI move: %s\n", uci_move);
        free(move);
        return NULL;
    }

    strncpy(move->move_data.uci.from_square, uci_move, 2);
    move->move_data.uci.from_square[2] = '\0';

    strncpy(move->move_data.uci.to_square, uci_move + 2, 2);
    move->move_data.uci.to_square[2] = '\0';

    if (len == 5) {
        strncpy(move->move_data.uci.promotion, uci_move + 4, 1);
        move->move_data.uci.promotion[1] = '\0';
    } else {
        move->move_data.uci.promotion[0] = '\0';  // No promotion
    }

    return move;
}

bool string_to_int(const char *str, int *out) {
    char *endptr;
    long val;

    errno = 0; // reset errno before call
    val = strtol(str, &endptr, 10); // base 10

    // Check for conversion errors
    if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
        return false; // out of int range
    }
    if (endptr == str || *endptr != '\0') {
        return false; // no digits converted or leftover characters
    }

    *out = (int)val;
    return true; // success
}

