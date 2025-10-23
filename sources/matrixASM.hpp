#ifndef MatrixASM_HPP
#define MatrixASM_HPP

// Copyright 2024 <Lelis Stefano Salazar Sanchez>
/**
 * @file matrixASM.hpp
 * @brief Archivo tipo header que contiene funcionalidades de la matriz elaborada
 * en ensamblador.
 *
 * En este archivo se definen las funciones de la matriz tanto constructores, 
 * destructores como consulta a las celdas y llenado de las celdas
 */

#include <iostream>
#include <random>
#include <cstdlib> 
#include <ctime>  
#include <limits> 


enum errorMatrix {
    ERROR_SUCCESS = 0,
    ERROR_MATRIX_NOT_ALLOCATED = 1,
    ERROR_INVALID_ROWS_OR_COLS = 2,
    ERROR_INVALID_MINE_QTY = 3,
    ERROR_INVALID_CELL_VALUES = 4,
    ERROR_OUT_OF_BOUND_MUL = 5
};

extern "C" {
    /**
     * @brief constructor de la matriz en ensamblador.
     *
     * @param self Puntero This del objeto.
     * @param rowsQty cantidad de filas que tendra la matriz.
     * @param colsQty cantidad de columnas que tendra la matriz.
     * @param mineQty cantidad de minas presente en la matriz
     */
    int matrix_constructor(void* self, int rowsQty, int colsQty, int mineQty);
    /**
     * @brief destructor de la matriz en ensamblador.
     *
     * @param self Puntero This del objeto.
     */
    void matrix_destructor(void* self);
    /**
     * @brief consulta y devuelve el contenido de la celda (x,y) en ensamblador.
     *
     * @param self Puntero This del objeto.
     * @param x coordenada x de la celda (fila).
     * @param y coordenada y de la celda (columna).
     * @return retorna el valor almacenado en la celda consultada
     */
    int checkCellAsm(void* self, int x, int y);
    /**
     * @brief actualiza el valor de la celda (x,y)
     *
     * @param self Puntero This del objeto.
     * @param x coordenada x de la celda (fila).
     * @param y coordenada y de la celda (columna).
     * @param retorna nuevo valor para la celda
     */
    void updateCellAsm(void* self, int x, int y, int newValue);
    /**
     * @brief coloca una mina en la matriz en ensamblador.
     *
     * @param self Puntero This del objeto.
     * @param x coordenada x de la celda (fila).
     * @param y coordenada y de la celda (columna).
     * @return retorna 1 si la mina fue colocada, retorna 0 si no fue colacada
     */
    int placeMineAsm(void* self, int x, int y);
} 

class MatrixASM {
public:
    // attributes
    int rows; ///< cantidad de filas de la matriz
    int cols; ///< cantidad de columnas de la matriz
    int* board; ///< puntero de la matriz
    int boardBytes; ///< cantidad de bytes que tiene la matriz
public:
    /**
     * @brief constructor de la matriz en C++ que llama al constructor en 
     * ensamblador.
     *
     * @param rowsQty cantidad de filas que tendra la matriz.
     * @param colsQty cantidad de columnas que tendra la matriz.
     * @param mineQty cantidad de minas presente en la matriz
     */
    MatrixASM(int rowsQty, int colsQty, int mineQty);
    
    /**
     * @brief destructor de la matriz en C++ que llama al destructor en 
     * ensamblador.
     */
    ~MatrixASM();
    /**
     * @brief llama a checkCellAsm.
     *
     * @param x coordenada x de la celda (fila).
     * @param y coordenada y de la celda (columna).
     * @return retorna el valor almacenado en la celda consultada
     */
    int checkCell(int x, int y);
    /**
     * @brief llama a updateCellASM.
     *
     * @param x coordenada x de la celda (fila).
     * @param y coordenada y de la celda (columna).
     * @param retorna nuevo valor para la celda
     */
    void updateCell(int x, int y, int newValue);
    /**
     * @brief Retorna la cantidad de filas en la matrix
     * 
     * @return Retorna la cantidad de filas en la matrix 
     */
    int getRows();
    /**
     * @brief Retorna la cantidad de columnas en la matrix
     * 
     * @return Retorna la cantidad de columnas en la matrix 
     */
    int getCols();

private:
    /**
     * @brief genera numeros random para luego llamar a placeMineAsm para colocar
     * la mina en esas coordenadas random
     * 
     * @param mineQty cantidad de minas a colocar 
     */
    void createMines(int mineQty);

};

#endif // MATRIX_HPP