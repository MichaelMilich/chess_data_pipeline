import unittest

import src_python.pgn_reader as pgn_reader
import src_python.pieces as pieces


class TestPgnReader(unittest.TestCase):
    def test_get_color_from_string_accepts_short_and_long_names(self):
        self.assertEqual(pieces.get_color_from_string("white"), pieces.Color.WHITE)
        self.assertEqual(pieces.get_color_from_string("W"), pieces.Color.WHITE)
        self.assertEqual(pieces.get_color_from_string("black"), pieces.Color.BLACK)
        self.assertEqual(pieces.get_color_from_string("b"), pieces.Color.BLACK)

    def test_get_piece_from_char_accepts_short_color_names(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("e4", "w")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Pawn(pieces.Color.WHITE, pieces.Position.from_string("e4")))

    def test_get_piece_from_char_accepts_short_color_names_for_castling(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("O-O", "b")
        self.assertEqual(start_piece, pieces.King(pieces.Color.BLACK, pieces.Position(5, 8)))
        self.assertEqual(end_piece, pieces.King(pieces.Color.BLACK, pieces.Position(7, 8)))

    def test_get_piece_from_char_keeps_origin_file_hint(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Nbd2", "white")
        self.assertEqual(start_piece, pieces.Knight(pieces.Color.WHITE, pieces.VirtualPosition(file=2)))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("d2")))

    def test_get_piece_from_char_keeps_origin_rank_hint(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("R1e1", "white")
        self.assertEqual(start_piece, pieces.Rook(pieces.Color.WHITE, pieces.VirtualPosition(rank=1)))
        self.assertEqual(end_piece, pieces.Rook(pieces.Color.WHITE, pieces.Position.from_string("e1")))

    def test_get_piece_from_char_keeps_full_origin_when_present(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Qh4e1", "white")
        self.assertEqual(start_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("h4")))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("e1")))

    def test_get_piece_from_char_keeps_promotion_origin_hint(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("exd8=Q", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition(file=5)))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("d8")))


if __name__ == "__main__":
    unittest.main()
