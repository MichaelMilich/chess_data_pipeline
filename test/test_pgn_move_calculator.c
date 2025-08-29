#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "../include/fen_utils.h"

void free_moves(Move **moves, int count) {
    if (!moves) return;
    for (int i = 0; i < count; i++) free(moves[i]);
    free(moves);
}

void test_no_moves(){
    printf("Testing no moves...\n");
    const char *pgn = "1.";
    int move_count = get_move_numbers_from_pgn_string(pgn);
    assert(move_count == 0);
    Move **moves = get_moves_from_pgn_string(pgn);
    assert(moves == NULL);
    printf("move count: %d Test passed\n", move_count);
}

void test_one_move(){
    printf("Testing one move...\n");
    const char *pgn = "1. e4";
    int move_count = get_move_numbers_from_pgn_string(pgn);
    assert(move_count == 1);
    Move **moves = get_moves_from_pgn_string(pgn);
    assert(moves != NULL);
    assert(strcmp(moves[0]->move_data.san.notation, "e4") == 0);
    printf("move count: %d Test passed\n", move_count);
    free_moves(moves, move_count);
}

void test_two_moves(){
    printf("Testing two moves...\n");
    const char *pgn = "1. e4 e5";
    int move_count = get_move_numbers_from_pgn_string(pgn);
    assert(move_count == 2);
    Move **moves = get_moves_from_pgn_string(pgn);
    assert(moves != NULL);
    assert(strcmp(moves[0]->move_data.san.notation, "e4") == 0);
    assert(strcmp(moves[1]->move_data.san.notation, "e5") == 0);
    printf("move count: %d Test passed\n", move_count);
    free_moves(moves, move_count);
}

void test_custom_pgn() {
    printf("Testing custom PGN...\n");
    const char *pgn = "1. e4 d5 2. Nf3 Nc6 3. Nc3 Nf6 4. Bb5 Bd7 5. O-O a6 6. Ba4 e5 7. d3 Bb4 8. Bd2 O-O 9. h3";
    int move_count = get_move_numbers_from_pgn_string(pgn);
    assert(move_count == 17);
    printf("move count: %d Test passed\n", move_count);
    Move **moves = get_moves_from_pgn_string(pgn);
    assert(moves != NULL);
    assert(strcmp(moves[16]->move_data.san.notation, "h3") == 0);
    free_moves(moves, move_count);
}

int main() {
    printf("=== move count ===\n\n");

    test_no_moves();
    test_one_move();
    test_two_moves();
    test_custom_pgn();
    
    printf("🎉 All tests passed successfully!\n");
    return 0;
} 
