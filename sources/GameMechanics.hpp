/**
 * @file GameMechanics.hpp
 * @brief Archivo tipo header que contiene las funcionalidades lógicas del juego.
 *
 * En este archivo se crea el juego BuscaMinas con sus diferentes atributos y funciones.
 */

#ifndef GAMEMECHANICS_HPP
#define GAMEMECHANICS_HPP

#include "matrixASM.hpp"
#include <string>
#include <iostream>
#include <utility>
#include <map>

using namespace std;

/**
 * @brief Clase que gestiona la lógica principal del juego de buscaminas.
 * 
 * La clase GameMechanics maneja la inicialización del juego, la interacción con el tablero, 
 * y la gestión del estado del juego.
 */
class GameMechanics {
private:
    MatrixASM* matrix;                        ///< Puntero a la matriz que representa el tablero del juego.
    int cellsLeft;                            ///< Número de celdas restantes por revelar.
    int minesQty;                             ///< Cantidad total de minas en el juego.
    int livesLeft;                            ///< Número de vidas restantes del jugador.
    int time;                                 ///< Tiempo transcurrido en el juego.
    int minesMarked;                          ///< Número de minas marcadas correctamente.
    string gameMode;                          ///< Modo de juego (facil, medio, dificil).
    string flagFile;                          ///< Nombre del archivo para guardar las celdas marcadas con banderas.
    map<pair<int, int>, bool> mineInfo;       ///< Información sobre si una celda tiene una mina.

private:
    /**
     * @brief Verifica si el jugador ha ganado el juego.
     */
    void checkVictory();

    /**
     * @brief Maneja la explosión de una mina.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     * @return Número de vidas restantes después de la explosión.
     */
    int explodeMine(int x, int y);

    /**
     * @brief Muestra la mina en las coordenadas especificadas.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     */
    void showMine(int x, int y);

    /**
     * @brief Muestra todas las minas en el tablero.
     */
    void showAllMines();

public:
    /**
     * @brief Constructor de la clase GameMechanics.
     * 
     * @param gameMode Modo de juego (facil, medio, dificil).
     */
    GameMechanics(const string& gameMode);

    /**
     * @brief Destructor de la clase GameMechanics.
     */
    ~GameMechanics();

    /**
     * @brief Inicializa el juego con el modo especificado.
     * 
     * @param gameMode Modo de juego.
     */
    void gameInit(const string& gameMode);

    /**
     * @brief Vector para llevar las celdas reveladas.
     */
    std::vector<bool> revealedCells;

    /**
     * @brief Maneja el evento de clic en una celda.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     * @param isLeftClick Indica si el clic es izquierdo.
     */
    void cellClicked(int x, int y, bool isLeftClick);

    /**
     * @brief Termina el juego y muestra el resultado (victoria o derrota).
     */
    void finishGame();
    /**
     * @brief Elimina una bandera de la celda especificada.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     */
    void removeFlag(int x, int y);

    /**
     * @brief Coloca una bandera en la celda especificada.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     */
    void putFlag(int x, int y);

    /**
     * @brief Muestra una pista en la celda especificada.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     * @param result Valor de la pista (número de minas alrededor).
     */
    void showHint(int x, int y, int result);

    /**
     * @brief Guarda el valor de una celda marcada con bandera en un archivo.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     * @param value Valor de la celda a guardar.
     */
    void saveFlaggedCellValue(int x, int y, char value);

    /**
     * @brief Restaura el valor de una celda marcada con bandera desde un archivo.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     * @return Valor de la celda restaurada.
     */
    char restoreFlaggedCellValue(int x, int y);

    /**
     * @brief Elimina la entrada de una celda marcada con bandera del archivo.
     * 
     * @param x Coordenada de la fila.
     * @param y Coordenada de la columna.
     */
    void deleteFlaggedCellEntry(int x, int y);

    // Getters para obtener información del estado del juego
    /**
     * @brief Obtiene el número de celdas restantes por revelar.
     * 
     * @return Número de celdas restantes.
     */
    int getCellsLeft() const { return cellsLeft; }

    /**
     * @brief Obtiene la cantidad total de minas en el juego.
     * 
     * @return Cantidad de minas.
     */
    int getMinesQty() const { return minesQty; }

    /**
     * @brief Obtiene el número de vidas restantes del jugador.
     * 
     * @return Número de vidas restantes.
     */
    int getLivesLeft() const { return livesLeft; }

    /**
     * @brief Obtiene el tiempo transcurrido en el juego.
     * 
     * @return Tiempo transcurrido.
     */
    int getTime() const { return time; }

    /**
     * @brief Obtiene el número de minas marcadas correctamente.
     * 
     * @return Número de minas marcadas.
     */
    int getMinesMarked() const { return minesMarked; }

    /**
     * @brief Obtiene el modo de juego actual.
     * 
     * @return Modo de juego.
     */
    string getGameMode() const { return gameMode; }

    /**
     * @brief Obtiene el nombre del archivo para guardar las celdas marcadas con banderas.
     * 
     * @return Nombre del archivo de banderas.
     */
    string getFlagFile() const { return flagFile; }

    /**
     * @brief Obtiene el puntero a la matriz del juego.
     * 
     * @return Puntero a la matriz.
     */
    MatrixASM* getMatrix() const { return matrix; }
};

#endif // GAMEMECHANICS_HPP
