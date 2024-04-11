#include "game.h"
/*
Game::Game() {
    // 게임 초기화
    currentTetromino_ = Tetromino::randomTetromino();
    nextTetromino_ = Tetromino::randomTetromino();
    holdTetromino_ = Tetromino();
    shadowTetromino_ = currentTetromino_;
    dropDelayCounter_ = 0;
    linesCleared_ = 0;
    gameOver_ = false;
    hardDrop_ = false;
}

void Game::update() {
    // 게임 업데이트
    handleInput();
    if (!gameOver_) {
        if (dropDelayCounter_ >= DROP_DELAY || hardDrop_) {
            if (!moveTetrominoDown()) {
                // 테트로미노가 더이상 움직일 수 없음
                lockTetromino();
                clearLines();
                spawnNewTetromino();
                if (!isValidPosition(currentTetromino_, currentX_, currentY_)) {
                    // 새로운 테트로미노가 생성되지 않을 경우, 게임 종료
                    gameOver_ = true;
                }
            }
            dropDelayCounter_ = 0;
            hardDrop_ = false;
        } else {
            ++dropDelayCounter_;
        }
    }
}

void Game::draw() {
    // 게임 화면 그리기
    console::clear();
    drawBoard();
    drawTetromino(currentTetromino_, currentX_, currentY_);
    drawNextTetromino();
    drawHoldTetromino();
    drawShadowTetromino();
    drawStats();
}

void Game::handleInput() {
    // 입력 처리
    if (console::key(console::K_LEFT)) {
        moveTetrominoLeft();
    }
    if (console::key(console::K_RIGHT)) {
        moveTetrominoRight();
    }
    if (console::key(console::K_DOWN)) {
        softDropTetromino();
    }
    if (console::key(console::K_UP)) {
        rotateTetrominoCW();
    }
    if (console::key(console::K_Z)) {
        rotateTetrominoCCW();
    }
    if (console::key(console::K_SPACE)) {
        hardDrop_ = true;
    }
    if (console::key(console::K_ESC)) {
        gameOver_ = true;
    }
    if (console::key(console::K_X)) {
        holdTetromino();
    }
}

void Game::drawBoard() {
    // 게임 보드 그리기
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board_[x][y]) {
                console::draw(x, y, BLOCK_STRING);
            }
        }
    }
}

void Game::drawTetromino(const Tetromino& tetromino, int x, int y) {
    // 테트로미노 그리기
    tetromino.drawAt(SNAKE_STRING, x, y);
}

void Game::drawNextTetromino() {
    // 넥스트 테트로미노 그리기
    console::draw(12, 1, "NEXT:");
    nextTetromino_.drawAt(SNAKE_STRING, 12, 3);
}

void Game::drawHoldTetromino() {
    // 홀드 테트로미노 그리기
    console::draw(12, 8, "HOLD:");
    holdTetromino_.drawAt(SNAKE_STRING, 12, 10);
}

void Game::drawShadowTetromino() {
    // 그림자 테트로미노 그리기
    int shadowY = currentY_;
    while (isValidPosition(currentTetromino_, currentX_, shadowY + 1)) {
        ++shadowY;
    }
    shadowTetromino_.drawAt(SHADOW_STRING, currentX_, shadowY);
}

void Game::drawStats() {
    // 게임 상태 표시
    console::draw(12, 16, "LINES:");
    console::draw(12, 18, std::to_string(linesCleared_));
    console::draw(12, 20, "TIME:");
    console::draw(12, 22, std::to_string(gameTime_));
}

bool Game::isValidPosition(const Tetromino& tetromino, int x, int y) {
    // 위치가 유효한지 확인
    for (int j = 0; j < tetromino.size(); ++j) {
        for (int i = 0; i < tetromino.size(); ++i) {
            if (tetromino.check(i, j)) {
                int boardX = x + i;
                int boardY = y + j;
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY < 0 || boardY >= BOARD_HEIGHT || board_[boardX][boardY]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Game::moveTetrominoLeft() {
    // 테트로미노를 왼쪽으로 이동
    if (isValidPosition(currentTetromino_, currentX_ - 1, currentY_)) {
        --currentX_;
        return true;
    }
    return false;
}

bool Game::moveTetrominoRight() {
    // 테트로미노를 오른쪽으로 이동
    if (isValidPosition(currentTetromino_, currentX_ + 1, currentY_)) {
        ++currentX_;
        return true;
    }
    return false;
}

bool Game::moveTetrominoDown() {
    // 테트로미노를 아래로 이동
    if (isValidPosition(currentTetromino_, currentX_, currentY_ + 1)) {
        ++currentY_;
        return true;
    }
    return false;
}

void Game::softDropTetromino() {
    // 소프트 드롭
    while (moveTetrominoDown());
}

void Game::rotateTetrominoCW() {
    // 시계 방향 회전
    Tetromino rotated = currentTetromino_.rotatedCW();
    if (isValidPosition(rotated, currentX_, currentY_)) {
        currentTetromino_ = rotated;
    }
}

void Game::rotateTetrominoCCW() {
    // 반시계 방향 회전
    Tetromino rotated = currentTetromino_.rotatedCCW();
    if (isValidPosition(rotated, currentX_, currentY_)) {
        currentTetromino_ = rotated;
    }
}

void Game::lockTetromino() {
    // 테트로미노 고정
    for (int j = 0; j < currentTetromino_.size(); ++j) {
        for (int i = 0; i < currentTetromino_.size(); ++i) {
            if (currentTetromino_.check(i, j)) {
                int boardX = currentX_ + i;
                int boardY = currentY_ + j;
                if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
                    board_[boardX][boardY] = true;
                }
            }
        }
    }
}

void Game::clearLines() {
    // 줄 삭제
    int linesClearedThisRound = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
        bool lineFull = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (!board_[x][y]) {
                lineFull = false;
                break;
            }
        }
        if (lineFull) {
            ++linesClearedThisRound;
            for (int j = y; j > 0; --j) {
                for (int i = 0; i < BOARD_WIDTH; ++i) {
                    board_[i][j] = board_[i][j - 1];
                }
            }
            for (int i = 0; i < BOARD_WIDTH; ++i) {
                board_[i][0] = false;
            }
            ++y; // 다음 줄도 확인해야 함
        }
    }
    linesCleared_ += linesClearedThisRound;
}

void Game::spawnNewTetromino() {
    // 새로운 테트로미노 생성
    currentTetromino_ = nextTetromino_;
    nextTetromino_ = Tetromino::randomTetromino();
    currentX_ = BOARD_WIDTH / 2 - currentTetromino_.size() / 2;
    currentY_ = 0;
    shadowTetromino_ = currentTetromino_;
}

void Game::holdTetromino() {
    // 테트로미노 홀드
    if (!holdUsed_) {
        Tetromino temp = currentTetromino_;
        if (holdTetromino_.name() != "") {
            currentTetromino_ = holdTetromino_;
        } else {
            spawnNewTetromino();
        }
        holdTetromino_ = temp;
        holdUsed_ = true;
    }
}
*/