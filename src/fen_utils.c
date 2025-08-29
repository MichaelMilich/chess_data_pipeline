
#include "fen_utils.h"


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

Move *get_move_from_san(char *san_move) {
    Move *move = (Move *)malloc(sizeof(Move));
    if (!move) return NULL;

    move->type = "san";
    move->player = '?';
    size_t cap = sizeof(move->move_data.san.notation);

    strncpy(move->move_data.san.notation, san_move, cap - 1);
    move->move_data.san.notation[cap - 1] = '\0';

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

int get_move_numbers_from_pgn_string(const char *pgn_string) {
    if (!pgn_string) return 0;

    int move_count = 0;
    const unsigned char *p = (const unsigned char *)pgn_string;

    while (*p != '\0') {
        // 1) Skip move numbers, dots, and whitespace
        while (*p != '\0' && (isdigit(*p) || *p == '.' || isspace(*p))) {
            p++;
        }
        if (*p == '\0') break;

        // 2) At start of a SAN token -> count one move
        move_count++;

        // 3) Skip the token (non-space run)
        while (*p != '\0' && !isspace(*p)) {
            p++;
        }
        // loop continues; no extra p++ here
    }

    return move_count;
}

Move **get_moves_from_pgn_string(const char *pgn_string){
    if (!pgn_string) return NULL;
    
    int move_count = get_move_numbers_from_pgn_string(pgn_string);
    if (move_count == 0) return NULL;
    Move **moves = (Move **)malloc(sizeof(Move *) * move_count);
    if (!moves) return NULL;

    // Make a copy of the PGN string since strtok modifies it
    size_t pgn_size = strlen(pgn_string);
    char pgn_copy[pgn_size + 1];
    strncpy(pgn_copy, pgn_string, sizeof(pgn_copy) - 1);
    pgn_copy[sizeof(pgn_copy) - 1] = '\0';

    char *token = strtok(pgn_copy, " \t\r\n");
    int move_index = 0;
    while (token != NULL && move_index < move_count) {
        if (isdigit(token[0]) || token[0] == '.') {
            token = strtok(NULL, " \t\r\n");
            continue; // Skip move numbers and dots
        }
        moves[move_index] = get_move_from_san(token);
        move_index++;
        token = strtok(NULL, " \t\r\n");
    }

    return moves;
}

/**
 * @brief Parses a FEN (Forsyth–Edwards Notation) string and fills a FEN_Board struct.
 *
 * This function takes a FEN string describing a chess position and populates the provided
 * FEN_Board structure with the board state, side to move, castling rights, en passant target,
 * halfmove clock, and fullmove number.
 *
 * @param output Pointer to a FEN_Board struct to be filled. If NULL, memory will be allocated.
 * @param fen_string Null-terminated string containing the FEN position.
 * @return true if parsing was successful and the board was filled, false otherwise.
 *
 * The function expects the FEN string to be well-formed. If any part is missing or invalid,
 * the function returns false. If output is NULL, the function will allocate memory for it,
 * but the caller is responsible for freeing it.
 */
FEN_Board *create_fen_board(char *fen_string) {
    if (fen_string == NULL) {
        return NULL;
    }
    FEN_Board *output = (FEN_Board *)malloc(sizeof(FEN_Board));
    if (output == NULL) {
        return NULL;
    }
    
    // Initialize board with empty squares
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            output->board[rank][file] = ' ';
        }
    }
    
    // Make a copy of the FEN string since strtok modifies it
    char fen_copy[200];
    strncpy(fen_copy, fen_string, sizeof(fen_copy) - 1);
    fen_copy[sizeof(fen_copy) - 1] = '\0';
    
    char *board_part = strtok(fen_copy, " ");        // points into `fen`
    char *side = strtok(NULL, " ");             // "w"
    char *castling = strtok(NULL, " ");         // "KQkq"
    char *en_passant = strtok(NULL, " ");       // "e3"
    char *halfmove = strtok(NULL, " ");         // "0"
    char *fullmove = strtok(NULL, " ");         // "1"
    
    if (!board_part || !side || !castling || !en_passant || !halfmove || !fullmove) {
        free(output);
        return NULL;
    }
    
    // Parse the board part
    int rank_index = 7;
    int file_index = 0;
    int board_string_index = 0;
    
    while (board_part[board_string_index] != '\0' && rank_index >= 0) {
        char c = board_part[board_string_index];
        
        if (c == '/') {
            rank_index--;
            file_index = 0;
        } else if (isdigit(c)) {
            int empty_squares = c - '0';
            file_index += empty_squares;
        } else if (file_index < 8) {
            output->board[rank_index][file_index] = c;
            file_index++;
        }
        board_string_index++;
    }
    
    // Parse side to move
    output->side_to_move = (side[0] == 'w') ? 0 : 1;
    
    // Parse castling rights
    strncpy(output->castling_rights, castling, sizeof(output->castling_rights) - 1);
    output->castling_rights[sizeof(output->castling_rights) - 1] = '\0';
    
    // Parse en passant square
    if (strcmp(en_passant, "-") != 0) {
        output->en_passant_square_file = en_passant[0] - 'a';
        output->en_passant_square_rank = en_passant[1] - '1';
    } else {
        output->en_passant_square_file = -1;
        output->en_passant_square_rank = -1;
    }
    
    // Parse halfmove clock and fullmove number
    bool halfmove_convert_success = string_to_int(halfmove, &output->halfmove_clock);
    bool fullmove_convert_success = string_to_int(fullmove, &output->fullmove_number);
    
    if (!halfmove_convert_success || !fullmove_convert_success) {
        free(output);
        return NULL;
    }

    return output;
}

/**
 * @brief Converts a FEN_Board struct to a FEN string.
 *
 * This function serializes the board state and metadata from a FEN_Board struct
 * into a standard FEN string and writes it to the provided output buffer.
 *
 * @param board Pointer to the FEN_Board struct to convert.
 * @param fen_string_out Output buffer to store the resulting FEN string.
 * @return true if conversion was successful, false otherwise.
 */
bool fen_board_to_fen_string(FEN_Board *board, char *fen_string_out){
    if (board == NULL || fen_string_out == NULL) {
        return false;
    }
    fen_string_out[0] = '\0'; // Initialize output string

    // Convert board to FEN string
    for (int rank = 7; rank >= 0; rank--) {
        int empty_count = 0;
        for (int file = 0; file < 8; file++) {
            if (board->board[rank][file] == ' ') {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    sprintf(fen_string_out + strlen(fen_string_out), "%d", empty_count);
                    empty_count = 0;
                }
                strncat(fen_string_out, &board->board[rank][file], 1);
            }
        }
        if (empty_count > 0) {
            sprintf(fen_string_out + strlen(fen_string_out), "%d", empty_count);
        }
        if (rank > 0) {
            strcat(fen_string_out, "/");
        }
    }

    // Append side to move
    strcat(fen_string_out, board->side_to_move == 0 ? " w " : " b ");

    // Append castling rights
    strcat(fen_string_out, board->castling_rights);
    strcat(fen_string_out, " ");

    // Append en passant square
    if (board->en_passant_square_file != -1 && board->en_passant_square_rank != -1) {
        sprintf(fen_string_out + strlen(fen_string_out), "%c%d ",
                board->en_passant_square_file + 'a',
                board->en_passant_square_rank + 1);
    } else {
        strcat(fen_string_out, "- ");
    }

    // Append halfmove clock and fullmove number
    sprintf(fen_string_out + strlen(fen_string_out), "%d %d",
            board->halfmove_clock, board->fullmove_number);

    return true;
}

bool free_fen_plus(FEN_Plus *fen_plus){
    if (fen_plus == NULL) {
        return false;
    }
    if (fen_plus->board != NULL) {
        free(fen_plus->board);
    }
    if (fen_plus->move != NULL) {
        free(fen_plus->move);
    }
    free(fen_plus);
    return true;
}



FEN_Board *generate_starting_position_fen(void){
    char *fen_string =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    return create_fen_board(fen_string);
}

char *find_move_starting_position_of_piece(char piece, FEN_Board *board, char destination_square[2], char *starting_position){
    // TODO: fill this function
    return NULL;
}

bool translate_san_to_uci(Move *san_move, FEN_Board *board){
    if (san_move == NULL || board == NULL) {
        return false;
    }
    if (strcmp(san_move->type, "uci") == 0) {
        return true; // Already in UCI format
    }
    if (strcmp(san_move->type, "san") != 0) {
        return false; // not UCI and not SAN, something is wrong
    }

    uci_move uci;
    size_t san_length = strlen(san_move->move_data.san.notation);
    if( san_length==2){ // this happens only when we move a pawn to an empty square
        strncmp(san_move->move_data.san.notation,uci.to_square, 2);
        uci.to_square[2] = '\0';
        char from_square[3];
        find_move_starting_position_of_piece("p", board, uci.to_square, &from_square);
    }
}
