#include "matrixASM.hpp"
// tomado de https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
using u32 = uint_least32_t; 
using engine = std::mt19937;

MatrixASM::MatrixASM(int rowsQty, int colsQty, int mineQty) {
    // verifica que las columnas y filas tengan un valor valido       
    if (rowsQty < 1 || colsQty < 1) {
        throw ERROR_INVALID_ROWS_OR_COLS;
    }
    // verifica que la cantidad de minas sea valida
    if (mineQty < 1 || mineQty >= rowsQty*colsQty) {
        throw ERROR_INVALID_MINE_QTY;
    }
    if (rowsQty > std::numeric_limits<int>::max() / colsQty 
        || colsQty > std::numeric_limits<int>::max() / rowsQty) {
        throw ERROR_OUT_OF_BOUND_MUL;
    }
    // constructor en ensamblador
    int error = matrix_constructor(this, rowsQty, colsQty, mineQty);
    if (error == ERROR_MATRIX_NOT_ALLOCATED) {
        throw ERROR_MATRIX_NOT_ALLOCATED;
    }
    createMines(mineQty);
}

MatrixASM::~MatrixASM() {
    // destructor en ensamblador
   matrix_destructor(this);
}

void MatrixASM::createMines(int mineQty) {
    // crea un generador de numeros random
    std::srand(std::time(NULL));
    int minesPlaced = 0;
    int x = 0;
    int y = 0;
    int random_value = 0;
    // coloca minas, se detiene cuando ya se haya colocado el total de mians
    while (minesPlaced < mineQty) {
        // genera un numero random entre 0 y el total de celdas
        random_value = std::rand() % (this->cols*this->rows);
        // obtiene el equivalente de x del numero generado
        x = random_value/(this->cols);
        // obtiene el equivalente de y del numero generado
        y = random_value%(this->cols);
        // llama a placeMineAsm para colocar la mina, si devuelve cero 
        // es porque la celda ya contenia una mina. 
        // si devuelve 1 es porque si se coloco
        minesPlaced += placeMineAsm(this, x, y);
    }
}

int MatrixASM::checkCell(int x, int y) {
    if (x < 0 || x >= this->rows || y < 0 || y >= this->cols) {
        throw ERROR_INVALID_CELL_VALUES;
    }
    // checkCell en ensamblador
    int result = checkCellAsm(this, x, y);
    return result;
}
void MatrixASM::updateCell(int x, int y, int newValue) {
    if (x < 0 || x >= this->rows || y < 0 || y >= this->cols) {
        throw ERROR_INVALID_CELL_VALUES;
    }
    // updateCell en ensamblador
    updateCellAsm(this, x, y, newValue);
}
int MatrixASM::getRows () {
    int rows = this->rows;
    return rows;
}
int MatrixASM::getCols () {
    int cols = this->cols;
    return cols;
}
