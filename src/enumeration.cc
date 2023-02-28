#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <fstream>
#include "board.h"

int main() {
    Board::init();

    std::queue<uint64_t> que[600]; /* que[i]はi手ではじめて到達可能な状態を処理するキュー */
    std::unordered_set<uint64_t> S[600]; /* S[i]はi手ではじめて到達可能な状態の集合 */
    std::unordered_set<uint64_t> found_states;
    std::unordered_set<uint64_t> found_afterstates;
    std::unordered_set<uint64_t> terminal_states;

    for (Square sq_i = SQ_1A; sq_i < SQUARE_NB; ++sq_i) {
        for (Square sq_j = (Square)((int)sq_i+1); sq_j < SQUARE_NB; ++sq_j) {
            for (int i = 1; i <= 2; ++i) {
                for (int j = 1; j <= 2; ++j) {
                    Board initial_board(0);
                    initial_board.spawn(sq_i, i);
                    initial_board.spawn(sq_j, j);
                    uint64_t normalized_key = initial_board.normalize();
                    if (found_states.find(normalized_key) == found_states.end()) {
                        found_states.insert(normalized_key);
                        que[0].push(initial_board.normalize());
                        S[0].insert(initial_board.normalize());
                    }
                }
            }
        }
    }

    for (int move_cnt = 0; move_cnt < 600; ++move_cnt) {
        if (que[move_cnt].empty()) {
            break;
        }
        while (!que[move_cnt].empty()) {
            uint64_t k = que[move_cnt].front();
            que[move_cnt].pop();
            Board board(0);
            board.set(k);

            if (board.is_gameover()) {
                terminal_states.insert(board.normalize());
                continue;
            }

            for (Action action : ALL_ACTIONS) {
                board.set(k);
                if (!board.is_changed_by(action)) {
                    continue;
                }
                board.do_action(action);
                uint64_t afterstate_key = board.normalize();
                board.set(afterstate_key);

                if (found_afterstates.find(afterstate_key) != found_afterstates.end()) {
                    continue;
                }
                found_afterstates.insert(afterstate_key);
                for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
                    if (!board.is_empty(sq)) {
                        continue;
                    }
                    for (int num = 1; num <= 2; ++num) {
                        Board next_board(0);
                        next_board.set(afterstate_key);
                        next_board.spawn(sq, num);
                        uint64_t next_state_key = next_board.normalize();

                        if (found_states.find(next_state_key) == found_states.end()) {
                            found_states.insert(next_state_key);
                            que[move_cnt+1].push(next_state_key);
                            S[move_cnt+1].insert(next_state_key);
                        }
                    }
                }
            }
        }

        std::cout << move_cnt << " " << S[move_cnt].size() << std::endl;
    }

    std::cout << "found states: " << found_states.size() << std::endl;
    std::cout << "found afterstates: " << found_afterstates.size() << std::endl;
    std::cout << "terminal states: " << terminal_states.size() << std::endl;

    std::fstream file_found_states;
    file_found_states.open("found_states.txt", std::ios_base::out);
    for (uint64_t key : found_states) {
        file_found_states << key << std::endl;
    }

    std::fstream file_found_afterstates;
    file_found_afterstates.open("found_afterstates.txt", std::ios_base::out);
    for (uint64_t key : found_afterstates) {
        file_found_afterstates << key << std::endl;
    }

    std::fstream file_terminal_states;
    file_terminal_states.open("terminal_states.txt", std::ios_base::out);
    for (uint64_t key : terminal_states) {
        file_terminal_states << key << std::endl;
    }
}