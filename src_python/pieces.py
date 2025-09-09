class Position:
    FILE_NAMES = "abcdefgh"
    def __init__(self, file: int, rank: int):
        assert file >= 1 and file <= 8
        assert rank >= 1 and rank <= 8
        self.file = file
        self.rank = rank
    
    def __str__(self):
        return f"{self.FILE_NAMES[self.file - 1]}{self.rank}"

    def __eq__(self, other):
        return self.file == other.file and self.rank == other.rank


    @classmethod
    def from_string(cls, position_string: str):
        assert len(position_string) == 2
        file = cls.FILE_NAMES.index(position_string[0]) + 1
        rank = int(position_string[1])
        return cls(file, rank)


class AbstractPiece:
    def __init__(self, color: str, type: str, position: Position = None):
        self.color = color
        self.type = type
        self.position = position
        self._moveset = set()
        self._attack_moveset = set()

    def get_moveset(self):
        if self.position is None:
            return Exception("Piece has no position")
        final_moveset = set()
        for move in self._moveset:
            if self.position.file + move[0] <1 or self.position.file + move[0] >8:
                continue
            if self.position.rank + move[1] <1 or self.position.rank + move[1] >8:
                continue
            final_moveset.add( Position(self.position.file + move[0], self.position.rank + move[1]) )
        return final_moveset

    def get_attack_moveset(self):
        if self.position is None:
            return Exception("Piece has no position")
        final_moveset = set()
        for move in self._attack_moveset:
            if self.position.file + move[0] <1 or self.position.file + move[0] >8:
                continue
            if self.position.rank + move[1] <1 or self.position.rank + move[1] >8:
                continue
            final_moveset.add( Position(self.position.file + move[0], self.position.rank + move[1]) )
        return final_moveset
    
    def get_all_moveset(self):
        return self.get_moveset().union(self.get_attack_moveset())

    def __str__(self):
        return f"{self.color} {self.type}"

    def __eq__(self, other):
        return self.color == other.color and self.type == other.type and self.position == other.position

class Pawn(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "Pawn", position)
        self._moveset.add( (0,1) if self.color == "white" else (0,-1) )
        self._attack_moveset.add( (1,1) if self.color == "white" else (-1,-1) )
        self._attack_moveset.add( (-1,1) if self.color == "white" else (1,-1) )

    def get_moveset(self):
        starting_rank = 2 if self.color == "white" else 7
        if self.position.rank == starting_rank:
            self._moveset.add( (0,2) if self.color == "white" else (0,-2) )
        elif (0,2) in self._moveset:
            self._moveset.remove( (0,2) )
        elif (0,-2) in self._moveset:
            self._moveset.remove( (0,-2) )
        return super().get_moveset()
    
    def __str__(self):
        return "P" if self.color == "white" else "p"

    def __repr__(self):
        return self.__str__()

class Knight(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "Knight", position)
        for file in (1,2,-1,-2):
            for rank in (1,2,-1,-2):
                if abs(file) == abs(rank):
                    continue
                self._moveset.add( (file,rank) )

    def get_attack_moveset(self):
        return self.get_moveset()

    def __str__(self):
        return "N" if self.color == "white" else "n"

    def __repr__(self):
        return self.__str__()

class Bishop(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "Bishop", position)
        for i in range(-8,9):
            self._moveset.add( (i,i) )
            self._moveset.add( (i,-i) )

    def get_attack_moveset(self):
        return self.get_moveset()

    def __str__(self):
        return "B" if self.color == "white" else "b"

    def __repr__(self):
        return self.__str__()

class Rook(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "Rook", position)
        for i in range(-8,9):
            self._moveset.add( (i,0) )
            self._moveset.add( (0,i) )

    def get_attack_moveset(self):
        return self.get_moveset()

    def __str__(self):
        return "R" if self.color == "white" else "r"

    def __repr__(self):
        return self.__str__()

class Queen(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "Queen", position)
        for i in range(-8,9):
            self._moveset.add( (i,i) )
            self._moveset.add( (i,-i) )
            self._moveset.add( (i,0) )
            self._moveset.add( (0,i) )

    def get_attack_moveset(self):
        return self.get_moveset()

    def __str__(self):
        return "Q" if self.color == "white" else "q"

    def __repr__(self):
        return self.__str__()

class King(AbstractPiece):
    def __init__(self, color: str, position: Position = None):
        super().__init__(color, "King", position)
        for i in range(-1,2):
            for j in range(-1,2):
                self._moveset.add( (i,j) )

    def get_attack_moveset(self):
        return self.get_moveset()

    def __str__(self):
        return "K" if self.color == "white" else "k"

    def __repr__(self):
        return self.__str__()

def get_piece_from_char(char: str, position_string: str):
    char_lower = char.lower()
    is_white = char.isupper()
    if char_lower == "p":
        return Pawn(color="white" if is_white else "black", position=Position.from_string(position_string))
    elif char_lower == "n":
        return Knight(color="white" if is_white else "black", position=Position.from_string(position_string))
    elif char_lower == "b":
        return Bishop(color="white" if is_white else "black", position=Position.from_string(position_string))
    elif char_lower == "r":
        return Rook(color="white" if is_white else "black", position=Position.from_string(position_string))
    elif char_lower == "q":
        return Queen(color="white" if is_white else "black", position=Position.from_string(position_string))
    elif char_lower == "k":
        return King(color="white" if is_white else "black", position=Position.from_string(position_string))