from pieces import Position, AbstractPiece, get_piece_from_char

class Board:
    def __init__(self, fen_string: str = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        self.board = self._board_from_fen_string(fen_string)
        split_fen = fen_string.split(" ")
        self.side_to_move = split_fen[1]
        self.castling_rights = split_fen[2]
        self.en_passant_square = split_fen[3]
        self.halfmove_clock = int(split_fen[4])
        self.fullmove_number = int(split_fen[5])

    def __str__(self):
        return f"{self.board}"

    def _board_to_fen_string(self):
        fen_string = ""
        for rank in range(8, 0, -1):
            file_index = 0
            empty_count = 0
            while file_index < 8:
                piece = self.board[rank][file_index]
                if piece is None:
                    empty_count += 1
                else:
                    if empty_count > 0:
                        fen_string += str(empty_count)
                        empty_count = 0
                    fen_string += piece
        return fen_string

    @staticmethod
    def _board_from_fen_string(fen_string: str):
        fen_string = fen_string.split(" ")[0]
        board = [[None for _ in range(8)] for _ in range(8)]
        rank = 7
        file_index = 0
        for char in fen_string:
            if char == "/":
                rank -= 1
                file_index = 0
            elif char.isdigit():
                file_index += int(char)
            else:
                board[rank][file_index] = get_piece_from_char(char, f"{Position.FILE_NAMES[file_index]}{rank}")
                file_index += 1
        return board