// MIT License
//
// Copyright (c) 2022 Adrian Brzozowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created on 28.05.2022.
//

#ifndef GAME_TREE_SEARCH_ALGORITHMS_TIC_TAC_TOE_H
#define GAME_TREE_SEARCH_ALGORITHMS_TIC_TAC_TOE_H

// project
#include "game_tree_search_algorithms/board.h"


namespace gtsa {

template<size_t N>
class TicTacToe {
public:
    void run(int p1_x, int p1_y);

    void run(bool is_p1_turn = true);

    [[nodiscard]] std::pair<int, int> getBestMove(bool is_p1_turn);

    int minimax(int depth, bool is_p1_turn);

private:
    Board<N> board_;
};

template<size_t N>
void TicTacToe<N>::run(int p1_x, int p1_y) {
    board_.setFieldForP1(p1_x, p1_y);

    if (p1_x < 0 || p1_x >= N || p1_y < 0 || p1_y >= N) {
        throw std::range_error("invalid position");
    }

    run(false);
}

template<size_t N>
void TicTacToe<N>::run(bool is_p1_turn) {
    board_.print();

    while (!board_.isGameEnd() && !board_.isFull()) {
        const auto[x, y] = getBestMove(is_p1_turn);

        if (is_p1_turn) {
            board_.setFieldForP1(x, y);
        }
        else {
            board_.setFieldForP2(x, y);
        }

        std::cout << std::endl;
        board_.print();

        is_p1_turn = !is_p1_turn;
    }

    std::cout << "\nGame over" << std::endl;
}

template<size_t N>
std::pair<int, int> TicTacToe<N>::getBestMove(bool is_p1_turn) {
    int best_x = -1;
    int best_y = -1;
    int score = 0;
    int best_score = std::numeric_limits<int>::min();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (board_.isFieldEmpty(i, j)) {
                if (is_p1_turn) {
                    board_.setFieldForP1(i, j);
                }
                else {
                    board_.setFieldForP2(i, j);
                }

                score = minimax(0, !is_p1_turn);

                board_.clearField(i, j);

                if (score > best_score) {
                    best_score = score;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    return {best_x, best_y};
}

template<size_t N>
int TicTacToe<N>::minimax(int depth, bool is_p1_turn) {
    if (board_.isGameEnd()) {
        if (is_p1_turn) {
            // p2 wins
            return (board_.getMaxDepth() - depth);
        }
        else {
            // p1 wins
            return -(board_.getMaxDepth() - depth);
        }
    }
    else if (board_.isFull()) {
        // draw
        return 0;
    }

    if (is_p1_turn) {
        auto best_score = std::numeric_limits<int>::max();
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                if (board_.isFieldEmpty(i, j)) {
                    board_.setFieldForP1(i, j);

                    best_score = std::min(best_score, minimax(depth + 1, !is_p1_turn));

                    board_.clearField(i, j);
                }
            }
        }

        return best_score;
    }
    else {
        auto best_score = std::numeric_limits<int>::min();
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                if (board_.isFieldEmpty(i, j)) {
                    board_.setFieldForP2(i, j);

                    best_score = std::max(best_score, minimax(depth + 1, !is_p1_turn));

                    board_.clearField(i, j);
                }
            }
        }

        return best_score;
    }
}

}  // namespace game_tree_search_algorithms

#endif  // GAME_TREE_SEARCH_ALGORITHMS_TIC_TAC_TOE_H
