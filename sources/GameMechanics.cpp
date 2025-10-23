#include "GameMechanics.hpp"
#include <fstream>
#include <cstdio>
#include <iostream>
using namespace std;

// Constructor
GameMechanics::GameMechanics(const string& mode) {
    matrix = nullptr;
    cellsLeft = 0;
    minesQty = 0;
    livesLeft = 0;
    time = 0;
    gameMode = mode;
    flagFile = "flagged_cells.txt";
    minesMarked = 0;
}

// Destructor
GameMechanics::~GameMechanics() {
    if (matrix) {
        delete matrix;
        matrix = nullptr;
        cout << "Deleted it successfully" << endl;
    }
}

// Métodos del juego
void GameMechanics::gameInit(const string& mode) {
    gameMode = mode;
    if (matrix) {
        delete matrix;
        matrix = nullptr;
    }

    // Configurar el tamaño de la matriz y la cantidad de minas según el modo de juego
    int rows, cols;
    if (mode == "facil") {
        rows = 8;
        cols = 8;
        minesQty = 10;
        livesLeft = 3;
        cellsLeft = 54;
    } else if (mode == "medio") {
        rows = 16;
        cols = 16;
        minesQty = 40;
        livesLeft = 2;
        cellsLeft = 216;
    } else if (mode == "dificil") {
        rows = 16;
        cols = 30;
        minesQty = 99;
        livesLeft = 1;
        cellsLeft = 381;
    } else {
        cerr << "Modo de juego no válido." << endl;
        return;
    }

    cout << "Initializing game with mode: " << mode << endl;
    matrix = new MatrixASM(rows, cols, minesQty);
    revealedCells = vector<bool>(rows * cols, false);
    cout << "Game initialized successfully." << endl;
}

void GameMechanics::cellClicked(int x, int y, bool isLeftClick) {
    // Verificar primero si las coordenadas están fuera de los límites
    if (x < 0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in cellClicked" << endl;
        return;
    }

    int checkCellX = y;
    int checkCellY = x;
    int cellValue = matrix->checkCell(checkCellX, checkCellY);

    // Verificar si la celda ya está revelada
    if (revealedCells[y * matrix->cols + x]) {
        cout << "Cell already revealed: (" << x << ", " << y << ")" << endl;
        return;
    }

    // Si la celda tiene una bandera y se hace clic izquierdo, no hacer nada
    if (cellValue == '5' && isLeftClick) {
        cout << "Cell has a flag, no action taken: (" << x << ", " << y << ")" << endl;
        return;
    }

    // Si la celda ya explotó, no hacer nada
    if (cellValue == 'G') {
        cout << "Cell already exploded: (" << x << ", " << y << ")" << endl;
        return;
    }

    if (isLeftClick) {
        cout << "checkCell result: " << cellValue << endl;

        if (cellValue == -1) {
            livesLeft = explodeMine(x, y);
            if (livesLeft == 0) {
                finishGame();
            }
            matrix->updateCell(checkCellX, checkCellY, 'G');  // Marcada como explotada
        } else {
            // Almacenar el resultado como un carácter en la matriz
            matrix->updateCell(checkCellX, checkCellY, '0' + cellValue);
            cellsLeft--;
            if (cellsLeft == 0) {
                finishGame();
            }
        }
        // Marcar la celda como revelada
        revealedCells[y * matrix->cols + x] = true;

    } else {
        if (cellValue != '5') {
            putFlag(x, y);
        } else {
            removeFlag(x, y);
        }
    }

    cout << "Exiting cellClicked." << endl;
}

void GameMechanics::finishGame() {
    if (!matrix) {
        cerr << "Error: Matrix is null in finishGame" << endl;
        return;
    }

    cout << "Finishing game, livesLeft: " << livesLeft << endl;
    showAllMines();
    if (livesLeft <= 0) {
        // Mostrar ventana de pérdida
        cout << "You lost!" << endl;
    } else {
        // Mostrar ventana de victoria 
        cout << "You won!" << endl;
    }
}

void GameMechanics::removeFlag(int x, int y) {
    if (!matrix) {
        cerr << "Error: Matrix is null in removeFlag" << endl;
        return;
    }

    if (x < 0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in removeFlag" << endl;
        return;
    }

    if (gameMode == "dificil") {
        cout << "Flagging is disabled in hard mode." << endl;
        return;
    }

    if (gameMode == "facil" || gameMode == "medio") {
        // Verificar si la celda contiene una mina
        if (mineInfo.find({x, y}) == mineInfo.end()) {
            int result = matrix->checkCell(x, y);
            mineInfo[{x, y}] = (result == -1);
        }

        if (matrix->board[y * matrix->cols + x] == '5' && mineInfo[{x, y}]) {
            minesMarked--;
        }
    }

    cout << "Removing flag at: (" << x << ", " << y << ")" << endl;

    char originalValue = restoreFlaggedCellValue(x, y);
    matrix->board[y * matrix->cols + x] = originalValue;
    deleteFlaggedCellEntry(x, y);
}

void GameMechanics::putFlag(int x, int y) {
    if (!matrix) {
        cerr << "Error: Matrix is null in putFlag" << endl;
        return;
    }

    if (x < 0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in putFlag" << endl;
        return;
    }

    if (gameMode == "dificil") {
        cout << "Flagging is disabled in hard mode." << endl;
        return;
    }

    cout << "Putting flag at: (" << x << ", " << y << ")" << endl;

    if (gameMode == "facil" || gameMode == "medio") {
        // Verificar si la celda contiene una mina
        if (mineInfo.find({x, y}) == mineInfo.end()) {
            int result = matrix->checkCell(x, y);
            mineInfo[{x, y}] = (result == -1);
        }

        if (mineInfo[{x, y}]) {
            minesMarked++;
            if (minesMarked == minesQty) {
                // Si todas las minas están marcadas correctamente, se gana
                finishGame();
            }
        }
    }

    char originalValue = matrix->board[y * matrix->cols + x];
    saveFlaggedCellValue(x, y, originalValue);
    matrix->board[y * matrix->cols + x] = '5'; // Asumiendo que '5' representa una celda con bandera
}

void GameMechanics::showHint(int x, int y, int hintValue) {
    if (!matrix) {
        cerr << "Error: Matrix is null in showHint" << endl;
        return;
    }

    if (x < 0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in showHint" << endl;
        return;
    }

    if (hintValue >= 0 && hintValue <= 8) {
        cout << "There are " << hintValue << " mines around: (" << x << ", " << y << "): " << endl;
    } else {
        cerr << "Invalid hint value: " << hintValue << " at (" << x << ", " << y << ")" << endl;
    }
}

int GameMechanics::explodeMine(int x, int y) {
    if (!matrix) {
        cerr << "Error: Matrix is null in explodeMine" << endl;
        return livesLeft;
    }

    if (x < 0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in explodeMine" << endl;
        return livesLeft;
    }

    cout << "Mine exploded at (" << x << ", " << y << ")" << endl;
    cout << "Lives left:" << livesLeft - 1 << endl;
    return livesLeft - 1;
}

void GameMechanics::showMine(int x, int y) {
    if (!matrix) {
        cerr << "Error: Matrix is null in showMine" << endl;
        return;
    }

    if (x <     0 || x >= matrix->cols || y < 0 || y >= matrix->rows) {
        cerr << "Error: Coordinates out of bounds in showMine" << endl;
        return;
    }

    if (matrix->board[y * matrix->cols + x ] == -1) {
        cout << "Mine at (" << x << ", " << y << ")" << endl;
    }
}

void GameMechanics::showAllMines() {
    if (!matrix) {
        cerr << "Error: Matrix is null in showAllMines" << endl;
        return;
    }

    for (int y = 0; y < matrix->rows; ++y) {
        for (int x = 0; x < matrix->cols; ++x) {
            if (matrix->board[y * matrix->cols + x] == -1) {
                cout << "Mine at (" << x << ", " << y << ")" << endl;
            }
        }
    }
}

void GameMechanics::saveFlaggedCellValue(int x, int y, char value) {
    ofstream outFile(flagFile, ios::app); // Abrir archivo en modo append

    if (!outFile) {
        cerr << "Error: Failed to open " << flagFile << " for writing" << endl;
        return;
    }

    outFile << x << " " << y << " " << value << endl;

    outFile.close();
}

char GameMechanics::restoreFlaggedCellValue(int x, int y) {
    ifstream inFile(flagFile);

    if (!inFile) {
        cerr << "Error: Failed to open " << flagFile << " for reading" << endl;
        return '0'; // Valor predeterminado si falla la lectura
    }

    int savedX, savedY;
    char savedValue;
    char originalValue = matrix->board[y * matrix->cols + x];

    while (inFile >> savedX >> savedY >> savedValue) {
        if (savedX == x && savedY == y) {
            inFile.close();
            return savedValue;
        }
    }

    inFile.close();
    return originalValue; // Devolver el valor original si no se encontró una entrada para esas coordenadas
}

void GameMechanics::deleteFlaggedCellEntry(int x, int y) {
    ifstream inFile(flagFile);
    ofstream outFile("temp.txt");

    if (!inFile || !outFile) {
        cerr << "Error: Failed to open files for deleting entry" << endl;
        return;
    }

    int savedX, savedY;
    char savedValue;

    while (inFile >> savedX >> savedY >> savedValue) {
        if (savedX != x || savedY != y) {
            outFile << savedX << " " << savedY << " " << savedValue << endl;
        }
    }

    inFile.close();
    outFile.close();

    // Renombrar el archivo temporal al original
    if (remove(flagFile.c_str()) != 0) {
        cerr << "Error deleting original flag file" << endl;
    }
    if (rename("temp.txt", flagFile.c_str()) != 0) {
        cerr << "Error renaming temporary file to original flag file" << endl;
    }
}

void GameMechanics::checkVictory() {
    if (cellsLeft == 0) {
        cout << "Congratulations! You've revealed all cells that are not mines." << endl;
        finishGame();
    }
}