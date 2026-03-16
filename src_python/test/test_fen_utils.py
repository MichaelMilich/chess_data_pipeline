import unittest

import src_python.fen_utils as fen_utils
import src_python.pieces as pieces


class TestFenUtils(unittest.TestCase):
    def test_square_access_matches_internal_board_indices(self):
        board = fen_utils.Board()
        self.assertEqual(board["a1"], board._board[0][0])
        self.assertEqual(board["h8"], board._board[7][7])

    def test_square_assignment_updates_internal_board(self):
        board = fen_utils.Board()
        board["a1"] = None
        self.assertIsNone(board._board[0][0])
        board["a1"] = pieces.Rook(pieces.Color.WHITE, pieces.Position(1, 1))
        self.assertIsInstance(board._board[0][0], pieces.Rook)
        self.assertEqual(board._board[0][0].color, pieces.Color.WHITE)
        self.assertEqual(str(board._board[0][0].position), "a1")

    def test_invalid_square_key_raises_key_error(self):
        board = fen_utils.Board()
        with self.assertRaises(KeyError):
            _ = board["z9"]


if __name__ == "__main__":
    unittest.main()
