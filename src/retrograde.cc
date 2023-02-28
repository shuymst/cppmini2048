#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <numeric>
#include "board.h"

int main() {
    Board::init();

    std::unordered_map<uint64_t, double> state_values;
    std::unordered_map<uint64_t, double> afterstate_values;
    std::ifstream input_state_file("found_states.txt");
    std::string line;

    std::queue<uint64_t> que;
    while (std::getline(input_state_file, line)) {
        uint64_t state_key = std::stoull(line);
        que.push(state_key);
    }

    while (!que.empty()) {
        uint64_t state_key = que.front();
        que.pop();

        Board state_board(0);
        state_board.set(state_key);
        if (state_board.is_gameover()) {
            state_values[state_key] = 0.0;
            continue;
        }

        double max_q_value = -1e9;
        bool all_afterstates_calculated = true;

        for (Action action : state_board.legal_actions()) {
            Board afterstate_board(0);
            afterstate_board.set(state_key);
            double reward = afterstate_board.do_action(action);
            uint64_t afterstate_key = afterstate_board.normalize();

            if (afterstate_values.find(afterstate_key) != afterstate_values.end()) {
                max_q_value = std::max(max_q_value, reward + afterstate_values[afterstate_key]);
                continue;
            }

            afterstate_board.set(afterstate_key); // normalizedしておく
            std::vector<double> next_state_values;
            std::vector<double> next_state_probs;
            bool all_next_states_calculated = true;

            int empty_cnt = 0;
            for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
                if (afterstate_board.is_empty(sq)) {
                    empty_cnt++;
                }
            }

            for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
                if (!afterstate_board.is_empty(sq)) {
                    continue;
                }
                for (int i = 1; i <= 2; ++i) {
                    Board next_state_board(0);
                    next_state_board.set(afterstate_key);
                    next_state_board.spawn(sq, i);
                    uint64_t next_state_key = next_state_board.normalize();
                    if (state_values.find(next_state_key) == state_values.end()) {
                        all_next_states_calculated = false;
                        break;
                    }
                    //assert(state_values[normalized_key] != 0);
                    next_state_values.emplace_back(state_values[next_state_key]);
                    if (i == 1) {
                        next_state_probs.emplace_back(1.0 / (double)empty_cnt * 0.9);
                    } else {
                        next_state_probs.emplace_back(1.0 / (double)empty_cnt * 0.1);
                    }
                }
                if (!all_next_states_calculated) {
                    all_afterstates_calculated = false;
                    break;
                }
            }

            if (all_next_states_calculated) {
                double afterstate_value = std::inner_product(next_state_values.begin(), next_state_values.end(), next_state_probs.begin(), 0.0);
                afterstate_values[afterstate_key] = afterstate_value;
                max_q_value = std::max(max_q_value, reward + afterstate_value);
            }
        }

        if (all_afterstates_calculated) {
            state_values[state_key] = max_q_value;
        } else {
            que.push(state_key);
        }
    }

    std::fstream out_state_file;
    out_state_file.open("state_values_.txt", std::ios_base::out);
    for (auto it = state_values.begin(); it != state_values.end(); ++it) {
        out_state_file << it->first << " " << it->second << std::endl;
    }

    std::fstream out_afterstate_file;
    out_afterstate_file.open("afterstate_values_.txt", std::ios_base::out);
    for (auto it = afterstate_values.begin(); it != afterstate_values.end(); ++it) {
        out_afterstate_file << it->first << " " << it->second << std::endl;
    }

    return 0;
}