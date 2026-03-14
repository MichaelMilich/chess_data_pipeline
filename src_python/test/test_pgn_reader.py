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

    def test_get_piece_from_char_parses_normal_piece_move_without_disambiguation(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Nf3", "white")
        self.assertEqual(start_piece, pieces.Knight(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("f3")))

    def test_get_piece_from_char_parses_pawn_capture(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("exd5", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition(file=5)))
        self.assertEqual(end_piece, pieces.Pawn(pieces.Color.WHITE, pieces.Position.from_string("d5")))

    def test_get_piece_from_char_parses_piece_capture(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Qxe5", "black")
        self.assertEqual(start_piece, pieces.Queen(pieces.Color.BLACK, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.BLACK, pieces.Position.from_string("e5")))

    def test_get_piece_from_char_ignores_check_suffix(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Qh5+", "white")
        self.assertEqual(start_piece, pieces.Queen(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("h5")))

    def test_get_piece_from_char_ignores_mate_suffix(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Qh7#", "white")
        self.assertEqual(start_piece, pieces.Queen(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("h7")))

    def test_get_piece_from_char_ignores_annotation_suffix(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Bb5!", "white")
        self.assertEqual(start_piece, pieces.Bishop(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Bishop(pieces.Color.WHITE, pieces.Position.from_string("b5")))

    def test_get_piece_from_char_ignores_combined_capture_check_and_annotation(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Nxf7+?", "white")
        self.assertEqual(start_piece, pieces.Knight(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("f7")))

    def test_get_piece_from_char_parses_white_queenside_castling(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("O-O-O", "white")
        self.assertEqual(start_piece, pieces.King(pieces.Color.WHITE, pieces.Position(5, 1)))
        self.assertEqual(end_piece, pieces.King(pieces.Color.WHITE, pieces.Position(3, 1)))

    def test_get_piece_from_char_parses_black_kingside_castling_with_zeroes(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("0-0", "black")
        self.assertEqual(start_piece, pieces.King(pieces.Color.BLACK, pieces.Position(5, 8)))
        self.assertEqual(end_piece, pieces.King(pieces.Color.BLACK, pieces.Position(7, 8)))

    def test_get_piece_from_char_parses_black_queenside_castling_with_lowercase_o(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("o-o-o", "black")
        self.assertEqual(start_piece, pieces.King(pieces.Color.BLACK, pieces.Position(5, 8)))
        self.assertEqual(end_piece, pieces.King(pieces.Color.BLACK, pieces.Position(3, 8)))

    def test_get_piece_from_char_parses_simple_promotion(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("e8=Q", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("e8")))

    def test_get_piece_from_char_parses_promotion_to_knight_after_capture(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("exd8=N", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition(file=5)))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("d8")))

    def test_get_piece_from_char_parses_promotion_with_check_suffix(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("e8=Q+", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Queen(pieces.Color.WHITE, pieces.Position.from_string("e8")))

    def test_get_piece_from_char_parses_promotion_with_mate_suffix(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("fxg8=R#", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition(file=6)))
        self.assertEqual(end_piece, pieces.Rook(pieces.Color.WHITE, pieces.Position.from_string("g8")))

    def test_get_piece_from_char_keeps_full_origin_for_knight_move(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Nb1d2", "white")
        self.assertEqual(start_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("b1")))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("d2")))

    def test_get_piece_from_char_returns_virtual_origin_for_simple_pawn_move(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("a4", "white")
        self.assertEqual(start_piece, pieces.Pawn(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Pawn(pieces.Color.WHITE, pieces.Position.from_string("a4")))

    def test_get_piece_from_char_rejects_unsupported_color_string(self):
        with self.assertRaises(ValueError):
            pgn_reader.get_piece_from_char("e4", "green")

    def test_get_piece_from_char_rejects_invalid_promotion_piece(self):
        with self.assertRaises(KeyError):
            pgn_reader.get_piece_from_char("e8=X", "white")

    def test_get_piece_from_char_rejects_invalid_destination_square(self):
        with self.assertRaises(AssertionError):
            pgn_reader.get_piece_from_char("Ne9", "white")

    def test_get_piece_from_char_rejects_empty_move_string(self):
        with self.assertRaises(IndexError):
            pgn_reader.get_piece_from_char("", "white")

    def test_get_piece_from_char_rejects_incomplete_piece_move(self):
        with self.assertRaises(AssertionError):
            pgn_reader.get_piece_from_char("Q", "white")

    def test_get_piece_from_char_currently_accepts_out_of_range_rank_hint_nine(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("R9e1", "white")
        self.assertEqual(start_piece, pieces.Rook(pieces.Color.WHITE, pieces.VirtualPosition(rank=9)))
        self.assertEqual(end_piece, pieces.Rook(pieces.Color.WHITE, pieces.Position.from_string("e1")))

    def test_get_piece_from_char_currently_accepts_out_of_range_rank_hint_zero(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("R0e1", "white")
        self.assertEqual(start_piece, pieces.Rook(pieces.Color.WHITE, pieces.VirtualPosition(rank=0)))
        self.assertEqual(end_piece, pieces.Rook(pieces.Color.WHITE, pieces.Position.from_string("e1")))

    def test_get_piece_from_char_currently_falls_back_for_invalid_file_hint(self):
        start_piece, end_piece = pgn_reader.get_piece_from_char("Nid2", "white")
        self.assertEqual(start_piece, pieces.Knight(pieces.Color.WHITE, pieces.VirtualPosition()))
        self.assertEqual(end_piece, pieces.Knight(pieces.Color.WHITE, pieces.Position.from_string("d2")))


if __name__ == "__main__":
    unittest.main()
