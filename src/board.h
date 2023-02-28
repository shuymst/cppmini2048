#pragma once
#include <vector>
#include <random>
#include <cassert>
#include "types.h"

class Board {
    private:
        const int64_t seed;
        std::mt19937_64 mt_;
        uint8_t tiles[9];
        inline static std::vector<uint16_t> right_action_;
        inline static std::vector<uint16_t> right_action_scores_;
        inline static std::vector<std::vector<uint16_t>> right_action_inv_;

    public:
        public:
        static void init();
        Board(int64_t seed);
        Board(const Board&) = delete;
        Board(Board&&) = delete;

        bool is_empty(Square sq) const;
        void spawn(Square sq, int num);
        void spawn_force(Square sq, int num); // Don't use when playing games
        int get_num(Square sq) const;
        void random_spawn();
        void reset();
        void game_start();
        void clear();
        bool is_gameover() const;

        uint32_t do_action(Action action);
        std::vector<Action> legal_actions() const;
        uint32_t compute_reward(Action action) const;
        bool is_changed_by(Action action) const;

        std::vector<uint64_t> inverse_action(Action action) const;
        std::vector<Action> invertible_actions() const;
        bool is_invertible_by(Action action) const;

        uint64_t key() const;
        void set(uint64_t key);
        uint64_t rotate90() const;
        uint64_t rotate180() const;
        uint64_t rotate270() const;
        uint64_t flip_horizontal() const;
        uint64_t flip_vertical() const;
        uint64_t flip_diagonal1() const;
        uint64_t flip_diagonal2() const;
        uint64_t normalize() const;

        std::vector<uint64_t> enum_before_spawn_normalized_keys() const; // state -> prev afterstates
        std::vector<uint64_t> enum_before_action_normalized_keys() const; // afterstate -> prev states

        void print() const;
};