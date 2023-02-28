#include "../src/board.h"
#include <algorithm>
#include <gtest/gtest.h>

TEST(spawn_empty, Test001) {
    Board::init();
    Board board(0);
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        EXPECT_EQ(true, board.is_empty(sq));
        board.spawn(sq, 1);
        EXPECT_EQ(false, board.is_empty(sq));
    }
}

TEST(right, Test002) {
    /* 
    1 0 1
    0 0 1
    0 1 1
    */
    Board board(0);
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2C, 1);
    board.spawn(SQ_3B, 1);
    board.spawn(SQ_3C, 1);
    EXPECT_EQ(board.do_action(RIGHT_ACTION), 8);
    EXPECT_EQ(board.get_num(SQ_1A), 0); EXPECT_EQ(board.get_num(SQ_1B), 0); EXPECT_EQ(board.get_num(SQ_1C), 2);
    EXPECT_EQ(board.get_num(SQ_2A), 0); EXPECT_EQ(board.get_num(SQ_2B), 0); EXPECT_EQ(board.get_num(SQ_2C), 1);
    EXPECT_EQ(board.get_num(SQ_3A), 0); EXPECT_EQ(board.get_num(SQ_3B), 0); EXPECT_EQ(board.get_num(SQ_3C), 2);
}

TEST(down, Test003) {
    /* 
    2 2 1
    0 4 3
    0 1 3
    */
    Board board(0);
    board.spawn(SQ_1A, 2);
    board.spawn(SQ_1B, 2);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 0);
    board.spawn(SQ_2B, 4);
    board.spawn(SQ_2C, 3);
    board.spawn(SQ_3A, 0);
    board.spawn(SQ_3B, 1);
    board.spawn(SQ_3C, 3);
    EXPECT_EQ(board.do_action(DOWN_ACTION), 16);
    EXPECT_EQ(board.get_num(SQ_1A), 0); EXPECT_EQ(board.get_num(SQ_1B), 2); EXPECT_EQ(board.get_num(SQ_1C), 0);
    EXPECT_EQ(board.get_num(SQ_2A), 0); EXPECT_EQ(board.get_num(SQ_2B), 4); EXPECT_EQ(board.get_num(SQ_2C), 1);
    EXPECT_EQ(board.get_num(SQ_3A), 2); EXPECT_EQ(board.get_num(SQ_3B), 1); EXPECT_EQ(board.get_num(SQ_3C), 4);
}

TEST(left, Test004) {
    /* 
    2 0 1
    0 3 1
    1 1 1
    */
    Board board(0);
    board.spawn(SQ_1A, 2);
    board.spawn(SQ_1B, 0);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 0);
    board.spawn(SQ_2B, 3);
    board.spawn(SQ_2C, 1);
    board.spawn(SQ_3A, 1);
    board.spawn(SQ_3B, 1);
    board.spawn(SQ_3C, 1);
    EXPECT_EQ(board.do_action(LEFT_ACTION), 4);
    EXPECT_EQ(board.get_num(SQ_1A), 2); EXPECT_EQ(board.get_num(SQ_1B), 1); EXPECT_EQ(board.get_num(SQ_1C), 0);
    EXPECT_EQ(board.get_num(SQ_2A), 3); EXPECT_EQ(board.get_num(SQ_2B), 1); EXPECT_EQ(board.get_num(SQ_2C), 0);
    EXPECT_EQ(board.get_num(SQ_3A), 2); EXPECT_EQ(board.get_num(SQ_3B), 1); EXPECT_EQ(board.get_num(SQ_3C), 0);
}

TEST(up, Test005) {
    /* 
    1 0 1
    2 2 1
    1 2 1
    */
    Board board(0);
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 0);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 2);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 1);
    board.spawn(SQ_3A, 1);
    board.spawn(SQ_3B, 2);
    board.spawn(SQ_3C, 1);
    EXPECT_EQ(board.do_action(UP_ACTION), 8+4);
    EXPECT_EQ(board.get_num(SQ_1A), 1); EXPECT_EQ(board.get_num(SQ_1B), 3); EXPECT_EQ(board.get_num(SQ_1C), 2);
    EXPECT_EQ(board.get_num(SQ_2A), 2); EXPECT_EQ(board.get_num(SQ_2B), 0); EXPECT_EQ(board.get_num(SQ_2C), 1);
    EXPECT_EQ(board.get_num(SQ_3A), 1); EXPECT_EQ(board.get_num(SQ_3B), 0); EXPECT_EQ(board.get_num(SQ_3C), 0);
}

TEST(is_game_over1, Test006) {
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    Board board(0);
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    EXPECT_TRUE(board.is_gameover());
}

TEST(is_game_over2, Test007) {
    Board board(0);
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, 1);
    }
    EXPECT_FALSE(board.is_gameover());
}

TEST(key, Test008) {
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    Board board(0);
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    uint64_t key = board.key();
    Board another_board(0);
    another_board.set(key);
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        EXPECT_EQ(board.get_num(sq), another_board.get_num(sq));
    }
}

TEST(rotate90, Test009) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board rotated_board(0);
    /*
    3 6 9
    2 5 8
    1 4 7
    */
    rotated_board.spawn(SQ_1A, 3); rotated_board.spawn(SQ_1B, 6); rotated_board.spawn(SQ_1C, 9);
    rotated_board.spawn(SQ_2A, 2); rotated_board.spawn(SQ_2B, 5); rotated_board.spawn(SQ_2C, 8);
    rotated_board.spawn(SQ_3A, 1); rotated_board.spawn(SQ_3B, 4); rotated_board.spawn(SQ_3C, 7);

    EXPECT_EQ(board.rotate90(), rotated_board.key());
}

TEST(rotate180, Test010) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board rotated_board(0);
    /*
    9 8 7
    6 5 4
    3 2 1
    */
    rotated_board.spawn(SQ_1A, 9); rotated_board.spawn(SQ_1B, 8); rotated_board.spawn(SQ_1C, 7);
    rotated_board.spawn(SQ_2A, 6); rotated_board.spawn(SQ_2B, 5); rotated_board.spawn(SQ_2C, 4);
    rotated_board.spawn(SQ_3A, 3); rotated_board.spawn(SQ_3B, 2); rotated_board.spawn(SQ_3C, 1);

    EXPECT_EQ(board.rotate180(), rotated_board.key());
}

TEST(rotate270, Test011) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board rotated_board(0);
    /*
    7 4 1
    8 5 2
    9 6 3
    */
    rotated_board.spawn(SQ_1A, 7); rotated_board.spawn(SQ_1B, 4); rotated_board.spawn(SQ_1C, 1);
    rotated_board.spawn(SQ_2A, 8); rotated_board.spawn(SQ_2B, 5); rotated_board.spawn(SQ_2C, 2);
    rotated_board.spawn(SQ_3A, 9); rotated_board.spawn(SQ_3B, 6); rotated_board.spawn(SQ_3C, 3);

    EXPECT_EQ(board.rotate270(), rotated_board.key());
}

TEST(flip_horizontal, Test012) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board flipped_board(0);
    /*
    3 2 1
    6 5 4
    9 8 7
    */
    flipped_board.spawn(SQ_1A, 3); flipped_board.spawn(SQ_1B, 2); flipped_board.spawn(SQ_1C, 1);
    flipped_board.spawn(SQ_2A, 6); flipped_board.spawn(SQ_2B, 5); flipped_board.spawn(SQ_2C, 4);
    flipped_board.spawn(SQ_3A, 9); flipped_board.spawn(SQ_3B, 8); flipped_board.spawn(SQ_3C, 7);

    EXPECT_EQ(board.flip_horizontal(), flipped_board.key());
}

TEST(flip_vertical, Test013) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board flipped_board(0);
    /*
    7 8 9
    4 5 6
    1 2 3
    */
    flipped_board.spawn(SQ_1A, 7); flipped_board.spawn(SQ_1B, 8); flipped_board.spawn(SQ_1C, 9);
    flipped_board.spawn(SQ_2A, 4); flipped_board.spawn(SQ_2B, 5); flipped_board.spawn(SQ_2C, 6);
    flipped_board.spawn(SQ_3A, 1); flipped_board.spawn(SQ_3B, 2); flipped_board.spawn(SQ_3C, 3);

    EXPECT_EQ(board.flip_vertical(), flipped_board.key());
}

TEST(flip_diagonal1, Test014) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board flipped_board(0);
    /*
    1 4 7
    2 5 8
    3 6 9
    */
    flipped_board.spawn(SQ_1A, 1); flipped_board.spawn(SQ_1B, 4); flipped_board.spawn(SQ_1C, 7);
    flipped_board.spawn(SQ_2A, 2); flipped_board.spawn(SQ_2B, 5); flipped_board.spawn(SQ_2C, 8);
    flipped_board.spawn(SQ_3A, 3); flipped_board.spawn(SQ_3B, 6); flipped_board.spawn(SQ_3C, 9);

    EXPECT_EQ(board.flip_diagonal1(), flipped_board.key());
}

TEST(flip_diagonal2, Test014) {
    Board board(0);
    /*
    1 2 3
    4 5 6
    7 8 9
    */
    for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
        board.spawn(sq, (int)sq+1);
    }
    Board flipped_board(0);
    /*
    9 6 3
    8 5 2
    7 4 1
    */
    flipped_board.spawn(SQ_1A, 9); flipped_board.spawn(SQ_1B, 6); flipped_board.spawn(SQ_1C, 3);
    flipped_board.spawn(SQ_2A, 8); flipped_board.spawn(SQ_2B, 5); flipped_board.spawn(SQ_2C, 2);
    flipped_board.spawn(SQ_3A, 7); flipped_board.spawn(SQ_3B, 4); flipped_board.spawn(SQ_3C, 1);

    EXPECT_EQ(board.flip_diagonal2(), flipped_board.key());
}

TEST(invertible, Test015) {
    Board board(0);
    /*
    1 2 3
    0 2 4
    0 0 2
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 2);
    board.spawn(SQ_1C, 3);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3C, 2);

    EXPECT_TRUE(board.is_invertible_by(RIGHT_ACTION));
    EXPECT_FALSE(board.is_invertible_by(DOWN_ACTION));
    EXPECT_FALSE(board.is_invertible_by(LEFT_ACTION));
    EXPECT_TRUE(board.is_invertible_by(UP_ACTION));
}

TEST(inverse_right_action, Test016) {
    Board board(0);
    /*
    1 2 3
    0 2 4
    0 0 2
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 2);
    board.spawn(SQ_1C, 3);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3C, 2);

    std::vector<uint64_t> possible_prev_state_keys = board.inverse_action(RIGHT_ACTION);

    for (auto afterstate_key : possible_prev_state_keys) {
        Board afterstate_board(0);
        afterstate_board.set(afterstate_key);
        afterstate_board.do_action(RIGHT_ACTION);
        EXPECT_EQ(afterstate_board.key(), board.key());
    }

    Board board_1(0);
    board_1.spawn(SQ_1A, 1); board_1.spawn(SQ_1B, 2); board_1.spawn(SQ_1C, 3);
    board_1.spawn(SQ_2A, 1); board_1.spawn(SQ_2B, 1); board_1.spawn(SQ_2C, 4);
    board_1.spawn(SQ_3A, 0); board_1.spawn(SQ_3B, 0); board_1.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board_1.key()), possible_prev_state_keys.end());

    EXPECT_EQ(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board.key()), possible_prev_state_keys.end());
}

TEST(inverse_down_action, Test017) {
    Board board(0);
    /*
    0 0 1
    1 2 4
    5 2 2
    */
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 1);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3A, 5);
    board.spawn(SQ_3B, 2);
    board.spawn(SQ_3C, 2);

    std::vector<uint64_t> possible_prev_state_keys = board.inverse_action(DOWN_ACTION);

    for (auto afterstate_key : possible_prev_state_keys) {
        Board afterstate_board(0);
        afterstate_board.set(afterstate_key);
        afterstate_board.do_action(DOWN_ACTION);
        EXPECT_EQ(afterstate_board.key(), board.key());
    }

    Board board_1(0);
    board_1.spawn(SQ_1A, 1); board_1.spawn(SQ_1B, 2); board_1.spawn(SQ_1C, 1);
    board_1.spawn(SQ_2A, 4); board_1.spawn(SQ_2B, 1); board_1.spawn(SQ_2C, 4);
    board_1.spawn(SQ_3A, 4); board_1.spawn(SQ_3B, 1); board_1.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board_1.key()), possible_prev_state_keys.end());

    EXPECT_EQ(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board.key()), possible_prev_state_keys.end());
}

TEST(inverse_left_action, Test018) {
    Board board(0);
    /*
    1 0 0
    1 2 4
    5 2 0
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_2A, 1);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3A, 5);
    board.spawn(SQ_3B, 2);

    std::vector<uint64_t> possible_prev_state_keys = board.inverse_action(LEFT_ACTION);

    for (auto afterstate_key : possible_prev_state_keys) {
        Board afterstate_board(0);
        afterstate_board.set(afterstate_key);
        afterstate_board.do_action(LEFT_ACTION);
        EXPECT_EQ(afterstate_board.key(), board.key());
    }

    Board board_1(0);
    board_1.spawn(SQ_1A, 0); board_1.spawn(SQ_1B, 1); board_1.spawn(SQ_1C, 0);
    board_1.spawn(SQ_2A, 1); board_1.spawn(SQ_2B, 2); board_1.spawn(SQ_2C, 4);
    board_1.spawn(SQ_3A, 5); board_1.spawn(SQ_3B, 1); board_1.spawn(SQ_3C, 1);
    EXPECT_NE(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board_1.key()), possible_prev_state_keys.end());

    EXPECT_EQ(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board.key()), possible_prev_state_keys.end());
}

TEST(inverse_up_action, Test019) {
    Board board(0);
    /*
    1 4 1
    6 2 4
    0 0 2
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 4);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 6);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3C, 2);

    std::vector<uint64_t> possible_prev_state_keys = board.inverse_action(UP_ACTION);

    for (auto afterstate_key : possible_prev_state_keys) {
        Board afterstate_board(0);
        afterstate_board.set(afterstate_key);
        afterstate_board.do_action(UP_ACTION);
        EXPECT_EQ(afterstate_board.key(), board.key());
    }

    Board board_1(0);
    board_1.spawn(SQ_1A, 1); board_1.spawn(SQ_1B, 4); board_1.spawn(SQ_1C, 1);
    board_1.spawn(SQ_2A, 6); board_1.spawn(SQ_2B, 1); board_1.spawn(SQ_2C, 4);
    board_1.spawn(SQ_3A, 0); board_1.spawn(SQ_3B, 1); board_1.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board_1.key()), possible_prev_state_keys.end());

    EXPECT_EQ(std::find(possible_prev_state_keys.begin(), possible_prev_state_keys.end(), board.key()), possible_prev_state_keys.end());
}

TEST(enum_before_spawn_normalized_keys, Test020) {
    Board board(0);
    /*
    1 4 1
    6 2 4
    0 0 2
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 4);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 6);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3C, 2);

    std::vector<uint64_t> before_spawn_normalized_keys = board.enum_before_spawn_normalized_keys();
    EXPECT_EQ(before_spawn_normalized_keys.size(), 4);

    Board before_spawn_board_1(0);
    before_spawn_board_1.spawn(SQ_1B, 4);
    before_spawn_board_1.spawn(SQ_1C, 1);
    before_spawn_board_1.spawn(SQ_2A, 6);
    before_spawn_board_1.spawn(SQ_2B, 2);
    before_spawn_board_1.spawn(SQ_2C, 4);
    before_spawn_board_1.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(before_spawn_normalized_keys.begin(), before_spawn_normalized_keys.end(), before_spawn_board_1.normalize()), before_spawn_normalized_keys.end());

    Board before_spawn_board_2(0);
    before_spawn_board_2.spawn(SQ_1A, 1);
    before_spawn_board_2.spawn(SQ_1B, 4);
    before_spawn_board_2.spawn(SQ_2A, 6);
    before_spawn_board_2.spawn(SQ_2B, 2);
    before_spawn_board_2.spawn(SQ_2C, 4);
    before_spawn_board_2.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(before_spawn_normalized_keys.begin(), before_spawn_normalized_keys.end(), before_spawn_board_2.normalize()), before_spawn_normalized_keys.end());

    Board before_spawn_board_3(0);
    before_spawn_board_3.spawn(SQ_1A, 1);
    before_spawn_board_3.spawn(SQ_1B, 4);
    before_spawn_board_3.spawn(SQ_1C, 1);
    before_spawn_board_3.spawn(SQ_2A, 6);
    before_spawn_board_3.spawn(SQ_2C, 4);
    before_spawn_board_3.spawn(SQ_3C, 2);
    EXPECT_NE(std::find(before_spawn_normalized_keys.begin(), before_spawn_normalized_keys.end(), before_spawn_board_3.normalize()), before_spawn_normalized_keys.end());

    Board before_spawn_board_4(0);
    before_spawn_board_4.spawn(SQ_1A, 1);
    before_spawn_board_4.spawn(SQ_1B, 4);
    before_spawn_board_4.spawn(SQ_1C, 1);
    before_spawn_board_4.spawn(SQ_2A, 6);
    before_spawn_board_4.spawn(SQ_2B, 2);
    before_spawn_board_4.spawn(SQ_2C, 4);
    EXPECT_NE(std::find(before_spawn_normalized_keys.begin(), before_spawn_normalized_keys.end(), before_spawn_board_4.normalize()), before_spawn_normalized_keys.end());

    Board before_spawn_board_5(0);
    before_spawn_board_5.spawn(SQ_1B, 4);
    before_spawn_board_5.spawn(SQ_1C, 1);
    before_spawn_board_5.spawn(SQ_2A, 6);
    before_spawn_board_5.spawn(SQ_2B, 2);
    before_spawn_board_5.spawn(SQ_2C, 4);
    EXPECT_EQ(std::find(before_spawn_normalized_keys.begin(), before_spawn_normalized_keys.end(), before_spawn_board_5.normalize()), before_spawn_normalized_keys.end());
}

TEST(enum_before_action_normalized_keys, Test020) {
    Board board(0);
    /*
    1 4 1
    6 2 4
    0 0 2
    */
    board.spawn(SQ_1A, 1);
    board.spawn(SQ_1B, 4);
    board.spawn(SQ_1C, 1);
    board.spawn(SQ_2A, 6);
    board.spawn(SQ_2B, 2);
    board.spawn(SQ_2C, 4);
    board.spawn(SQ_3C, 2);

    std::vector<uint64_t> before_action_normalized_keys = board.enum_before_action_normalized_keys();

    for (auto state_key : before_action_normalized_keys) {
        Board state_board(0);
        state_board.set(state_key);
        bool flag = false;
        auto legal_actions = state_board.legal_actions();
        for (Action action : legal_actions) {
            state_board.set(state_key);
            state_board.do_action(action);
            if (state_board.normalize() == board.normalize()) {
                flag = true;
            }
        }
        EXPECT_TRUE(flag);
    }
}

