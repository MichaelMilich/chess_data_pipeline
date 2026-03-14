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
POSSIBLE_CASTLING_MOVES = {"O-O", "O-O-O", "0-0", "0-0-0", "o-o", "o-o-o"}


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

def get_piece_from_char(move_string: str, color: str):
    """
    Returns the piece type and its designated position from a character and a position string.
    """
    color = pieces.get_color_from_string(color)

    # step 1: normalize the move string by removing any annotations or symbols that are not promotions
    move_string = _normalize_san(move_string)

    #step 2: handle castling moves first, since they don't follow the normal SAN format and can be easily identified
    if _is_castling_move(move_string):
        return _parse_castling_move(move_string, color)
    
    # step 3: handle promotion moves, since they also have a unique format that can be easily identified
    if _is_promotion_move(move_string):
        return _parse_promotion_move(move_string, color)
    
    # Now we know the move is a normal move, so we can parse it according to the standard SAN format
    piece_type = None
    if move_string[0] not in SAN_PIECE_DICT:
        piece_type = pieces.Pawn
    else:
        piece_type = SAN_PIECE_DICT[move_string[0]]
    start_position = _get_start_position_from_san(move_string)
    return piece_type(color, position=start_position), piece_type(color, pieces.Position.from_string(move_string[-2:]))

def _normalize_san(move_string: str):
    """
    Normalizes a move string in SAN format by removing any annotations or symbols that are not promotions
    """
    move_string = move_string.replace("+", "") # remove check symbol
    move_string = move_string.replace("#", "") # remove checkmate symbol
    move_string = move_string.replace("x", "") # remove capture symbol
    move_string = move_string.replace("!", "") # remove exclamation mark
    move_string = move_string.replace("?", "") # remove question mark
    return move_string

def _is_castling_move(move_string: str):
    return move_string in POSSIBLE_CASTLING_MOVES

def _parse_castling_move(move_string: str, color: str):
    home_rank = 1 if color == pieces.Color.WHITE else 8
    if move_string in ["O-O", "0-0", "o-o"]:
        return pieces.King(color, pieces.Position(5, home_rank)), pieces.King(color, pieces.Position(7, home_rank))
    elif move_string in ["O-O-O", "0-0-0", "o-o-o"]:
        return pieces.King(color, pieces.Position(5, home_rank)), pieces.King(color, pieces.Position(3, home_rank))

def _is_promotion_move(move_string: str):
    return "=" in move_string

def _parse_promotion_move(move_string: str, color: str):
    start_position = _get_start_position_from_san(move_string)
    promotion_piece_type = SAN_PIECE_DICT[move_string[-1]]
    return pieces.Pawn(color, position=start_position), promotion_piece_type(color, pieces.Position.from_string(move_string[-4:-2]))

def _get_start_position_from_san(move_string: str):
    move_core = move_string[:-4] if _is_promotion_move(move_string) else move_string[:-2]
    if not move_core:
        return pieces.VirtualPosition()

    if move_core[0] in SAN_PIECE_DICT:
        origin_hint = move_core[1:]
    else:
        origin_hint = move_core

    if len(origin_hint) == 2:
        file_char, rank_char = origin_hint[0], origin_hint[1]
        if file_char in pieces.Position.FILE_NAMES and rank_char.isdigit():
            return pieces.Position.from_string(origin_hint)

    if len(origin_hint) == 1:
        hint_char = origin_hint[0]
        if hint_char in pieces.Position.FILE_NAMES:
            file_index = pieces.Position.FILE_NAMES.index(hint_char) + 1
            return pieces.VirtualPosition(file=file_index)
        if hint_char.isdigit():
            return pieces.VirtualPosition(rank=int(hint_char))

    return pieces.VirtualPosition()

def main():
    pgn_file_path = os.path.join(test_artifacts.__path__[0], "game_1.pgn")
    move_list = get_moves_list_from_pgn_file(pgn_file_path)
    print(move_list)

if __name__ == "__main__":
    main()
