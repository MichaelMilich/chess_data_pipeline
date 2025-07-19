
#include "fen_utils.h"



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