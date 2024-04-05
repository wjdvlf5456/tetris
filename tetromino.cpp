#include "tetromino.h"

Tetromino Tetromino::I("I", 4, "XXXX");
Tetromino Tetromino::O("O", 2, "XX"
                                  "XX");
Tetromino Tetromino::T("T", 3, " X "
                                  "XXX");
Tetromino Tetromino::S("S", 3, " XX"
                                  "XX ");
Tetromino Tetromino::Z("Z", 3, "XX "
                                  " XX");
Tetromino Tetromino::J("J", 3, "X  "
                                  "XXX");
Tetromino Tetromino::L("L", 3, "  X"
                                  "XXX");

Tetromino::Tetromino(std::string name, int size, std::string shape) : name_(name), size_(size) {
    // 초기화
    int index = 0;
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            shape_[x][y] = (shape[index] == 'X');
            ++index;
        }
    }
    original_ = this;
}

Tetromino Tetromino::rotatedCW() {
    // 시계 방향 회전
    Tetromino rotated(name_, size_, "");
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            rotated.shape_[y][size_ - 1 - x] = shape_[x][y];
        }
    }
    return rotated;
}

Tetromino Tetromino::rotatedCCW() {
    // 반시계 방향 회전
    Tetromino rotated(name_, size_, "");
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            rotated.shape_[size_ - 1 - y][x] = shape_[x][y];
        }
    }
    return rotated;
}

void Tetromino::drawAt(std::string s, int x, int y) {
    // 테트로미노를 그린다.
    for (int j = 0; j < size_; ++j) {
        for (int i = 0; i < size_; ++i) {
            if (check(i, j)) {
                console::draw(x + i, y + j, s);
            }
        }
    }
}
