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

    std::unordered_set<uint64_t> legal_states;
    std::unordered_set<uint64_t> legal_afterstates;
    std::unordered_map<uint64_t, double> state_values;
    std::unordered_map<uint64_t, double> afterstate_values;
    std::unordered_set<uint64_t> tracking_states;
    std::unordered_set<uint64_t> tracking_afterstates;

    std::ifstream input_legal_state_file("found_states.txt");
    std::ifstream input_legal_afterstate_file("found_afterstates.txt");
    std::ifstream input_terminal_states_file("terminal_states.txt");
    std::string line;

    /* 入力読み込み */
    while (std::getline(input_legal_state_file, line)) {
        uint64_t state_key = std::stoull(line);
        legal_states.insert(state_key);
    }

    while (std::getline(input_legal_afterstate_file, line)) {
        uint64_t afterstate_key = std::stoull(line);
        legal_afterstates.insert(afterstate_key);
    }

    while (std::getline(input_terminal_states_file, line)) {
        uint64_t terminal_state_key = std::stoull(line);
        state_values[terminal_state_key] = 0.0;
        tracking_states.insert(terminal_state_key);
    }

    /* 後退解析 */
    while (tracking_states.size() > 0 || tracking_afterstates.size() > 0) {
        std::cout << state_values.size() << " " << afterstate_values.size() << " " << tracking_states.size() << " " << tracking_afterstates.size() << std::endl;

        /* calculated states -> previous afterstates */
        Board state_board(0);
        std::vector<uint64_t> finished_state_keys;
        for (uint64_t state_key : tracking_states) {
            state_board.set(state_key);
            std::vector<uint64_t> before_spawn_normalized_keys = state_board.enum_before_spawn_normalized_keys();
            bool all_prev_afterstates_calculated = true;

            for (uint64_t afterstate_key : before_spawn_normalized_keys) {

                /* legalなafterstateでない場合 */
                if (legal_afterstates.find(afterstate_key) == legal_afterstates.end()) {
                    continue;
                }
                /* すでに計算済みのafterstate */
                if (afterstate_values.find(afterstate_key) != afterstate_values.end()) {
                    continue;
                }

                Board afterstate_board(0);
                afterstate_board.set(afterstate_key);
                int empty_cnt = 0;
                for (Square sq = SQ_1A; sq < SQUARE_NB; ++sq) {
                    if (afterstate_board.is_empty(sq)) {
                        empty_cnt++;
                    }
                }

                bool all_next_states_calculated = true;
                std::vector<double> next_state_values;
                std::vector<double> next_state_probs;
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
                        break;
                    }
                }

                if (all_next_states_calculated) {
                    afterstate_values[afterstate_key] = std::inner_product(next_state_values.begin(), next_state_values.end(), next_state_probs.begin(), 0.0);
                    tracking_afterstates.insert(afterstate_key);
                } else {
                    all_prev_afterstates_calculated = false;
                }
            }
        
            if (all_prev_afterstates_calculated) {
                finished_state_keys.push_back(state_key);
            }
        }

        for (uint64_t finished_state_key : finished_state_keys) {
            tracking_states.erase(finished_state_key);
        }

        /* calculated afterstates -> previous states */
        Board afterstate_board(0);
        std::vector<uint64_t> finished_afterstate_keys;
        for (uint64_t afterstate_key : tracking_afterstates) {
            afterstate_board.set(afterstate_key);
            std::vector<uint64_t> before_action_normalized_keys = afterstate_board.enum_before_action_normalized_keys();
            bool all_prev_states_calculated = true;

            for (uint64_t state_key : before_action_normalized_keys) {

                /* legalなstateでない場合 */
                if (legal_states.find(state_key) == legal_states.end()) {
                    continue;
                }
                /* すでに計算済みのstate */
                if (state_values.find(state_key) != state_values.end()) {
                    continue;
                }

                Board state_board(0);
                state_board.set(state_key);
                double max_q_value = -1e9;
                bool all_afterstates_calculated = true;

                for (Action action : state_board.legal_actions()) {
                    Board afterstate_board(0);
                    afterstate_board.set(state_key);
                    double reward = afterstate_board.do_action(action);
                    uint64_t one_afterstate_key = afterstate_board.normalize();
                    if (afterstate_values.find(one_afterstate_key) != afterstate_values.end()) {
                        max_q_value = std::max(max_q_value, reward + afterstate_values[one_afterstate_key]);
                    } else {
                        all_afterstates_calculated = false;
                        break;
                    }
                }

                if (all_afterstates_calculated) {
                    state_values[state_key] = max_q_value;
                    tracking_states.insert(state_key);
                } else {
                    all_prev_states_calculated = false;
                }
            }

            if (all_prev_states_calculated) {
                finished_afterstate_keys.push_back(afterstate_key);
            }
        }

        for (uint64_t finished_afterstate_key : finished_afterstate_keys) {
            tracking_afterstates.erase(finished_afterstate_key);
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