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

#ifndef GAME_TREE_SEARCH_ALGORITHMS_BOARD_H
#define GAME_TREE_SEARCH_ALGORITHMS_BOARD_H

// std
#include <array>
#include <iostream>
#include <algorithm>


namespace gtsa {

/// Board stores the data in row-major order in memory
/// \tparam N size of the board
template<size_t N>
class Board {
public:
    Board();

    void print() const;

    [[nodiscard]] bool isGameEnd() const;

    [[nodiscard]] bool isWinInColumn() const;

    [[nodiscard]] bool isWinInRow() const;

    [[nodiscard]] bool isWinInDiagonal() const;

    [[nodiscard]] char getField(int x, int y) const;

    [[nodiscard]] std::size_t getMaxDepth() const;

    [[nodiscard]] bool isFieldEmpty(int x, int y) const;

    [[nodiscard]] bool isFull() const;

    void setFieldForP1(int id);

    void setFieldForP1(int x, int y);

    void setFieldForP2(int id);

    void setFieldForP2(int x, int y);

    void clearField(int x, int y);

    void setField(int id, char value);

    void setField(int x, int y, char value);

private:
    std::array<std::array<char, N>, N> board_{};
    const std::size_t fieldsCount_{board_.size() * board_.front().size()};
    std::size_t checkedFieldsCount_{0};

    const char P1 = 'X';
    const char P2 = 'O';
    const char EMPTY = ' ';
};

template<size_t N>
Board<N>::Board() {
    std::fill(&board_[0][0], &board_[N][0], EMPTY);
}

template<size_t N>
void Board<N>::print() const {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            const auto &v = board_[j][i];
            const auto c = (v == P1 ? P1 : (v == P2 ? P2 : ' '));

            std::cout << " | " << c;
        }

        std::cout << " | " << std::endl;
    }
}

template<size_t N>
bool Board<N>::isGameEnd() const {
    return isWinInColumn() || isWinInRow() || isWinInDiagonal();
}

template<size_t N>
bool Board<N>::isWinInColumn() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (!isFieldEmpty(i, 0)) {
            if (std::all_of(std::begin(board_[i]) + 1, std::end(board_[i]),
                            [&cmp = board_[i][0]](auto item) {
                                return item == cmp;
                            })) {
                return true;
            }
        }
    }

    return false;
}

template<size_t N>
bool Board<N>::isWinInRow() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (!isFieldEmpty(0, i)) {
            if (std::all_of(std::begin(board_) + 1, std::end(board_),
                            [&cmp = board_[0][i], &i](auto item) {
                                return item[i] == cmp;
                            })) {
                return true;
            }
        }
    }

    return false;
}

template<size_t N>
bool Board<N>::isWinInDiagonal() const {
    if (!isFieldEmpty(0, 0)) {
        const auto &cmp = board_[0][0];

        bool ok = true;
        for (std::size_t i = 0; i < N; ++i) {
            ok &= board_[i][i] == cmp;
        }

        if (ok) {
            return true;
        }
    }

    if (!isFieldEmpty(0, N - 1)) {
        const auto &cmp = board_[0][N - 1];

        bool ok = true;
        for (std::size_t i = 0; i < N; ++i) {
            ok &= board_[i][N - 1 - i] == cmp;
        }

        if (ok) {
            return true;
        }
    }

    return false;
}

template<size_t N>
char Board<N>::getField(int x, int y) const {
    return board_[x][y];
}

template<size_t N>
std::size_t Board<N>::getMaxDepth() const {
    return fieldsCount_;
}

template<size_t N>
bool Board<N>::isFieldEmpty(int x, int y) const {
    const auto &v = getField(x, y);

    return v != P1 && v != P2;
}

template<size_t N>
bool Board<N>::isFull() const {
    return checkedFieldsCount_ >= fieldsCount_;
}

template<size_t N>
void Board<N>::setFieldForP1(int id) {
    setField(id, P1);
    ++checkedFieldsCount_;
}

template<size_t N>
void Board<N>::setFieldForP1(int x, int y) {
    setField(x, y, P1);
    ++checkedFieldsCount_;
}

template<size_t N>
void Board<N>::setFieldForP2(int id) {
    setField(id, P2);
    ++checkedFieldsCount_;
}

template<size_t N>
void Board<N>::setFieldForP2(int x, int y) {
    setField(x, y, P2);
    ++checkedFieldsCount_;
}

template<size_t N>
void Board<N>::clearField(int x, int y) {
    setField(x, y, EMPTY);
    --checkedFieldsCount_;
}

template<size_t N>
void Board<N>::setField(int id, char value) {
    const auto x = (id - 1) / N;
    const auto y = (id - 1) % N;

    board_[x][y] = value;
}

template<size_t N>
void Board<N>::setField(int x, int y, char value) {
    board_[x][y] = value;
}

}  // namespace game_tree_search_algorithms

#endif  // GAME_TREE_SEARCH_ALGORITHMS_BOARD_H
