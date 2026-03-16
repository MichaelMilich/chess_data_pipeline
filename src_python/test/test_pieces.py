import src_python.pieces as pieces
import unittest

class TestPieces(unittest.TestCase):
    def _assert_piece(self, piece, expected_cls, expected_color, expected_square):
        self.assertIsInstance(piece, expected_cls)
        self.assertEqual(piece.color, expected_color)
        self.assertEqual(str(piece.position), expected_square)

    def test_get_piece_from_char(self):
        self._assert_piece(pieces.get_piece_from_char("P", "e2"), pieces.Pawn, pieces.Color.WHITE, "e2")
        self._assert_piece(pieces.get_piece_from_char("p", "e7"), pieces.Pawn, pieces.Color.BLACK, "e7")
        self._assert_piece(pieces.get_piece_from_char("N", "g1"), pieces.Knight, pieces.Color.WHITE, "g1")
        self._assert_piece(pieces.get_piece_from_char("n", "g8"), pieces.Knight, pieces.Color.BLACK, "g8")
        self._assert_piece(pieces.get_piece_from_char("B", "c1"), pieces.Bishop, pieces.Color.WHITE, "c1")
        self._assert_piece(pieces.get_piece_from_char("b", "c8"), pieces.Bishop, pieces.Color.BLACK, "c8")
        self._assert_piece(pieces.get_piece_from_char("R", "a1"), pieces.Rook, pieces.Color.WHITE, "a1")
        self._assert_piece(pieces.get_piece_from_char("r", "a8"), pieces.Rook, pieces.Color.BLACK, "a8")

if __name__ == "__main__":
    unittest.main()
