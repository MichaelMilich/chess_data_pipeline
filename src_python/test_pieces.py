import pieces
import unittest

class TestPieces(unittest.TestCase):
    def test_get_piece_from_char(self):
        self.assertEqual(pieces.get_piece_from_char("P", "e2"), pieces.Pawn("white", pieces.Position(5, 2)))
        self.assertEqual(pieces.get_piece_from_char("p", "e7"), pieces.Pawn("black", pieces.Position(5, 7)))
        self.assertEqual(pieces.get_piece_from_char("N", "g1"), pieces.Knight("white", pieces.Position(7, 1)))
        self.assertEqual(pieces.get_piece_from_char("n", "g8"), pieces.Knight("black", pieces.Position(7, 8)))
        self.assertEqual(pieces.get_piece_from_char("B", "c1"), pieces.Bishop("white", pieces.Position(3, 1)))
        self.assertEqual(pieces.get_piece_from_char("b", "c8"), pieces.Bishop("black", pieces.Position(3, 8)))
        self.assertEqual(pieces.get_piece_from_char("R", "a1"), pieces.Rook("white", pieces.Position(1, 1)))
        self.assertEqual(pieces.get_piece_from_char("r", "a8"), pieces.Rook("black", pieces.Position(1, 8)))

if __name__ == "__main__":
    unittest.main()