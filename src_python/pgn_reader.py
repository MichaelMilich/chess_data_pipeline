import os
import src_python.test.test_artifacts as test_artifacts
import src_python.pieces as pieces

SAN_PIECE_DICT = {
    "K": pieces.King,
    "Q": pieces.Queen,
    "R": pieces.Rook,
    "B": pieces.Bishop,
    "N": pieces.Knight,
}


def read_pgn_file(file_path: str):
    with open(file_path, 'r') as file:
        return file.read()

def _get_moves_line_from_pgn_file(pgn_file: str):
    """
    Returns the line that contains the moves of the game.
    """
    for line in pgn_file.split('\n'):
        if line.startswith('1.'):
            return line

def _get_moves_list_from_pgn_file(pgn_file: str):
    """
    Returns a list of moves from the game.
    """
    moves_line = _get_moves_line_from_pgn_file(pgn_file)
    raw_move_list = moves_line.split(' ')
    move_list = []
    white_move = ""
    black_move = ""
    i = 0
    while i < len(raw_move_list):
        word = raw_move_list[i]
        if word == "1-0" or word == "0-1" or word == "1/2-1/2":
            move_list.append(( white_move, black_move))
            break
        if "." in word:
            if white_move!="":
                move_list.append(( white_move, black_move))
            white_move = ""
            black_move = ""
        elif white_move=="":
            white_move = word
        elif black_move=="":
            black_move = word
        i += 1
    return move_list

def get_moves_list_from_pgn_file(pgn_file_path: str):
    pgn_file = read_pgn_file(pgn_file_path)
    return _get_moves_list_from_pgn_file(pgn_file)

def get_piece_from_char(char: str, color: str):
    """
    Returns the piece type and its designated position from a character and a position string.
    """
    piece_type = None
    if char[0] not in SAN_PIECE_DICT:
        piece_type = pieces.Pawn
    else:
        piece_type = SAN_PIECE_DICT[char[0]]
    return piece_type(color, pieces.Position.from_string(char[-2:])) # this is wrong by many points, read https://en.wikipedia.org/wiki/Algebraic_notation_(chess)



def main():
    pgn_file_path = os.path.join(test_artifacts.__path__[0], "game_1.pgn")
    move_list = get_moves_list_from_pgn_file(pgn_file_path)
    print(move_list)

if __name__ == "__main__":
    main()