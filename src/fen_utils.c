
#include "fen_utils.h"

FEN_Plus *starting_position_fen_plus(char *time_control, int elo, Move *uci_move) {
    FEN_Plus *fen_plus = (FEN_Plus *)malloc(sizeof(FEN_Plus));
    fen_plus->fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    fen_plus->time_control=time_control;
    fen_plus->elo=elo;
    fen_plus->move=uci_move;
    return fen_plus;
}
