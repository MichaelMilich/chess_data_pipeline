try:
    from src_python.pieces import Position, AbstractPiece, get_piece_from_char as fen_get_piece_from_char, Color
    from src_python.pgn_reader import get_piece_from_char as san_get_piece_from_char
except ImportError:
    from pieces import Position, AbstractPiece, get_piece_from_char as fen_get_piece_from_char, Color
    from pgn_reader import get_piece_from_char as san_get_piece_from_char

class Board(dict):
    def __init__(self, fen_string: str = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        super().__init__()
        self._board = self._board_from_fen_string(fen_string)
        self._fill_self_dict()
        split_fen = fen_string.split(" ")
        self.side_to_move = split_fen[1]
        self.castling_rights = split_fen[2]
        self.en_passant_square = split_fen[3]
        self.halfmove_clock = int(split_fen[4])
        self.fullmove_number = int(split_fen[5])
        self._white_pieces = set()
        self._black_pieces = set()

    def _fill_self_dict(self):
        for rank_index in range(len(self._board)):
            for file_index in range(len(self._board[0])):
                key = f"{Position.FILE_NAMES[file_index]}{rank_index + 1}"
                dict.__setitem__(self, key, self._board[rank_index][file_index])

    @staticmethod
    def _square_to_indices(square: str):
        if len(square) != 2:
            raise KeyError(square)
        file_name = square[0].lower()
        rank_name = square[1]
        if file_name not in Position.FILE_NAMES or not rank_name.isdigit():
            raise KeyError(square)
        rank_index = int(rank_name) - 1
        if rank_index < 0 or rank_index >= 8:
            raise KeyError(square)
        file_index = Position.FILE_NAMES.index(file_name)
        return rank_index, file_index

    def __getitem__(self, key):
        if isinstance(key, str):
            rank_index, file_index = self._square_to_indices(key)
            return self._board[rank_index][file_index]
        return dict.__getitem__(self, key)

    def __setitem__(self, key, value):
        if isinstance(key, str):
            rank_index, file_index = self._square_to_indices(key)
            self._board[rank_index][file_index] = value
            dict.__setitem__(self, key, value)
            return
        dict.__setitem__(self, key, value)

    def __str__(self):
        return self._board_to_fen_string()
    
    @property
    def white_pieces(self):
        self._fill_player_pieces(Color.WHITE)
        return self._white_pieces
    
    @property
    def black_pieces(self):
        self._fill_player_pieces(Color.BLACK)
        return self._black_pieces
    
    def _fill_player_pieces(self, color: str):
        pieces_set = self._white_pieces if color == Color.WHITE else self._black_pieces
        if len(pieces_set) == 0:
            for rank in self._board:
                for piece in rank:
                    if piece is not None and piece.color == color:
                        pieces_set.add(piece)
        return pieces_set
    
    def _update_piece_in_player_set(self, piece: AbstractPiece, color: str, remove: bool = False):
        pieces_set = self._white_pieces if color == Color.WHITE else self._black_pieces
        if remove and piece in pieces_set:
            pieces_set.remove(piece)
        elif not remove:
            pieces_set.add(piece)

    def _move_single_piece_on_the_board(self,start_position : Position, end_position: Position):
        """
        This function is only responsible for the moving of the pieces. it should affect self._board and the self dict.
        It is not responsible for checking if a move is legal or not (except for positions being within the board)
        """
        if not start_position.real_position or not end_position.real_position:
            raise ValueError("provided wrong positions to move a piece")
        
        moved_piece = self[str(start_position)] 
        if moved_piece is None:
            raise ValueError("Trying to move a piece that is not there")
        
        end_piece = self[str(end_position)]
        if end_piece:
            end_piece.position = None
            self._update_piece_in_player_set(piece=end_piece, color=end_piece.color, remove=True)

        self[str(end_position)] = self[str(start_position)]
        self[str(start_position)] = None

        moved_piece.position = Position.from_string(str(end_position))


    def _board_to_fen_string(self):
        fen_string = ""
        for rank in range(8, 0, -1):
            file_index = 0
            empty_count = 0
            while file_index < 8:
                piece = self._board[rank][file_index]
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
                board[rank][file_index] = fen_get_piece_from_char(char, f"{Position.FILE_NAMES[file_index]}{rank + 1}")
                file_index += 1
        return board
    
