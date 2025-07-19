#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "../include/fen_utils.h"

// Helper function to print the board
void print_board(char board[8][8]) {
    printf("Board representation:\n");
    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++) {
            printf("%c ", board[rank][file]);
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

// Helper function to print FEN_Board structure
void print_fen_board(FEN_Board *board) {
    printf("\n=== FEN_Board Structure Details ===\n");
    
    // Print board
    print_board(board->board);
    
    // Print other members
    printf("Move number: %d\n", board->move_number);
    printf("En passant square file: %d (0=a, 1=b, ..., 7=h, -1=none)\n", board->en_passant_square_file);
    printf("En passant square rank: %d (0=1, 1=2, ..., 7=8, -1=none)\n", board->en_passant_square_rank);
    printf("Side to move: %d (0=white, 1=black)\n", board->side_to_move);
    printf("Castling rights: '%s'\n", board->castling_rights);
    printf("Halfmove clock: %d\n", board->halfmove_clock);
    printf("Fullmove number: %d\n", board->fullmove_number);
    printf("=====================================\n\n");
}

// Test function for starting position
void test_starting_position() {
    printf("Testing starting position FEN parsing...\n");
    
    // Standard chess starting position FEN
    char *starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    FEN_Board board;
    bool success = create_fen_board(&board, starting_fen);
    
    assert(success == true);
    printf("✓ create_fen_board returned true\n");
    
    // Test side to move (should be white's turn)
    assert(board.side_to_move == 0);
    printf("✓ Side to move is correct (white's turn)\n");
    
    // Test castling rights (should be "KQkq")
    assert(strcmp(board.castling_rights, "KQkq") == 0);
    printf("✓ Castling rights are correct\n");
    
    // Test en passant square (should be none)
    assert(board.en_passant_square_file == -1);
    assert(board.en_passant_square_rank == -1);
    printf("✓ En passant square is correct (none)\n");
    
    // Test halfmove clock (should be 0)
    assert(board.halfmove_clock == 0);
    printf("✓ Halfmove clock is correct\n");
    
    // Test fullmove number (should be 1)
    assert(board.fullmove_number == 1);
    printf("✓ Fullmove number is correct\n");
    
    // Test specific board positions
    // White pieces on first rank
    assert(board.board[0][0] == 'R'); // a1
    assert(board.board[0][1] == 'N'); // b1
    assert(board.board[0][2] == 'B'); // c1
    assert(board.board[0][3] == 'Q'); // d1
    assert(board.board[0][4] == 'K'); // e1
    assert(board.board[0][5] == 'B'); // f1
    assert(board.board[0][6] == 'N'); // g1
    assert(board.board[0][7] == 'R'); // h1
    printf("✓ White back rank pieces are correct\n");
    
    // White pawns on second rank
    for (int file = 0; file < 8; file++) {
        assert(board.board[1][file] == 'P');
    }
    printf("✓ White pawns are correct\n");
    
    // Empty squares in middle
    for (int rank = 2; rank < 6; rank++) {
        for (int file = 0; file < 8; file++) {
            assert(board.board[rank][file] == ' ');
        }
    }
    printf("✓ Empty middle squares are correct\n");
    
    // Black pawns on seventh rank
    for (int file = 0; file < 8; file++) {
        assert(board.board[6][file] == 'p');
    }
    printf("✓ Black pawns are correct\n");
    
    // Black pieces on eighth rank
    assert(board.board[7][0] == 'r'); // a8
    assert(board.board[7][1] == 'n'); // b8
    assert(board.board[7][2] == 'b'); // c8
    assert(board.board[7][3] == 'q'); // d8
    assert(board.board[7][4] == 'k'); // e8
    assert(board.board[7][5] == 'b'); // f8
    assert(board.board[7][6] == 'n'); // g8
    assert(board.board[7][7] == 'r'); // h8
    printf("✓ Black back rank pieces are correct\n");
    
    // Print the structure for visual verification
    print_fen_board(&board);
    
    printf("✓ All starting position tests passed!\n\n");
}

// Test function for a position with en passant
void test_en_passant_position() {
    printf("Testing position with en passant...\n");
    
    // Position after 1. e4 e5 2. f4 h6 3. f5 g5
    char *en_passant_fen = "rnbqkbnr/pppp1p2/7p/4pPp1/4P3/8/PPPP2PP/RNBQKBNR w KQkq g6 0 4";
    
    FEN_Board board;
    bool success = create_fen_board(&board, en_passant_fen);
    
    assert(success == true);
    printf("✓ create_fen_board returned true\n");
    
    // Test side to move (should be white's turn)
    assert(board.side_to_move == 0);
    printf("✓ Side to move is correct (white's turn)\n");
    
    // Test en passant square (should be g6)
    assert(board.en_passant_square_file == 6); // g = 6 (0=a, 1=b, 2=c, 3=d)
    assert(board.en_passant_square_rank == 5); // 6 = 5 (0=1, 1=2, ..., 5=6)
    printf("✓ En passant square is correct (g6)\n");
    
    // Test fullmove number (should be 4)
    assert(board.fullmove_number == 4);
    printf("✓ Fullmove number is correct\n");
    
    // Test specific board positions
    assert(board.board[5][6] == ' '); // g6 is empty and can be captured by a pawn
    printf("✓ En passant position is correct\n");
    
    print_fen_board(&board);
    
    printf("✓ All en passant tests passed!\n\n");
}

// Test function for castling rights
void test_castling_rights() {
    printf("Testing castling rights...\n");
    
    // Position where white rook has moved
    char *no_castling_fen = "rnbqkbnr/pppp1ppp/4p3/8/8/5N2/PPPPPPPP/RNBQKBR1 b Qkq - 1 2";
    
    FEN_Board board;
    bool success = create_fen_board(&board, no_castling_fen);
    
    assert(success == true);
    printf("✓ create_fen_board returned true\n");
    
    // Test castling rights (should be "Qkq" - no white kingside castling)
    assert(strcmp(board.castling_rights, "Qkq") == 0);
    printf("✓ Castling rights are correct (no white kingside)\n");
    
    print_fen_board(&board);
    
    printf("✓ All castling rights tests passed!\n\n");
}

int main() {
    printf("=== FEN Board Structure Test Suite ===\n\n");
    
    test_starting_position();
    test_en_passant_position();
    test_castling_rights();
    
    printf("🎉 All tests passed successfully!\n");
    return 0;
} 