#include "matrixASM.hpp"
#include <cassert>
int main () {
  // test de exito para creacion de matriz, consulta de celda y actualizacion
  // de celda
  MatrixASM* matrixTest = new MatrixASM(2,2,2);
  assert(matrixTest->getCols() ==  2);
  assert(matrixTest->getRows() ==  2);
  int valorCeldaAnterior = matrixTest->checkCell(0, 0);
  std::cout << "Valor anterior: " << valorCeldaAnterior << std::endl;
  matrixTest->updateCell(0,0,8);
  int valorCeldaActual = matrixTest->checkCell(0, 0);
  std::cout << "Valor actual: " << valorCeldaActual << std::endl;
  assert(valorCeldaActual == 8);

  
  // test para filas o columnas invalidas en checkCell (menor a cero)
  bool errorInvalidRowsCols = false;
  try {
    matrixTest->checkCell(-1, 2);
  } catch (errorMatrix ERROR_INVALID_ROWS_OR_COLS) {
    errorInvalidRowsCols = true;
  }
  assert(errorInvalidRowsCols);

  // test para filas o columnas invalidas en checkCell (mayor a la cantidad de cols)
  errorInvalidRowsCols = false;
  try {
    matrixTest->checkCell(0,9);
  } catch (errorMatrix ERROR_INVALID_ROWS_OR_COLS) {
    errorInvalidRowsCols = true;
  }
  assert(errorInvalidRowsCols);


  // test para filas o columnas invalidas en updateCell (menor a cero)
  errorInvalidRowsCols = false;
  try {
    matrixTest->updateCell(-1, 2, 99);
  } catch (errorMatrix ERROR_INVALID_ROWS_OR_COLS) {
    errorInvalidRowsCols = true;
  }
  assert(errorInvalidRowsCols);

  // test para filas o columnas invalidas en updateCell (mayor a la cantidad de cols)
  errorInvalidRowsCols = false;
  try {
    matrixTest->updateCell(0,9, 99);
  } catch (errorMatrix ERROR_INVALID_ROWS_OR_COLS) {
    errorInvalidRowsCols = true;
  }
  assert(errorInvalidRowsCols);

  delete matrixTest;

  // Test para filas o columnas invalidas
  errorInvalidRowsCols = false;
  try {
    MatrixASM* matrixTest = new MatrixASM(0,2, 2);
  } catch (errorMatrix ERROR_INVALID_ROWS_OR_COLS) {
    errorInvalidRowsCols = true;
  }
  assert(errorInvalidRowsCols);
  
  // Test para minas invalidas (minas negativas)
  bool errorInvalidMines = false;
  try {
    MatrixASM* matrixTest = new MatrixASM(2,2, -1);
  } catch (errorMatrix ERROR_INVALID_MINE_QTY) {
    errorInvalidMines = true;
  }
  assert(errorInvalidMines);

  // Test para minas invalidas (mas minas que celdas)
  errorInvalidMines = false;
  try {
    MatrixASM* matrixTest = new MatrixASM(2,2, 5);
  } catch (errorMatrix ERROR_INVALID_MINE_QTY) {
    errorInvalidMines = true;
  }
  assert(errorInvalidMines);

  // Test para casos donde la cantidad de celdas excede los limites de INT
  bool errorOutOfBound = false;
  try {
    MatrixASM* matrixTest = new MatrixASM(999999, 999999, 2);
  } catch (errorMatrix ERROR_OUT_OF_BOUND_MUL) {
    errorOutOfBound = true;
  }
  assert(ERROR_OUT_OF_BOUND_MUL);

  // Test para casos donde no hay suficiente memoria
  bool errorMatrixNotAllocated = false;
  try {
    MatrixASM* matrixTest = new MatrixASM(46340, 46340, 2);
    delete matrixTest;
  } catch (errorMatrix ERROR_MATRIX_NOT_ALLOCATED) {
    errorMatrixNotAllocated = true;
    std::cerr << "Could not allocate" << std::endl;
  }

}