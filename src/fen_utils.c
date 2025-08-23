
#include "fen_utils.h"


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
bool create_fen_board(FEN_Board *output, char *fen_string) {
    if (fen_string == NULL) {
        return false;
    }
    if (output == NULL) {
        output = (FEN_Board *)malloc(sizeof(FEN_Board));
        if (output == NULL) return false;
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
        return false;
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
        return false;
    }
    
    return true;
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