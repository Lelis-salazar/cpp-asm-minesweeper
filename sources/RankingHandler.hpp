/**
 * @file RankingHandler.hpp
 * @brief Archivo tipo header que contiene el manejo de puntajes en el juego.
 *
 * En este archivo se calcula el puntaje al ganar una partida en base al tiempo,
 * vidas restantes y dificultad.
 */
#ifndef RANKINGHANDLER_HPP
#define RANKINGHANDLER_HPP

#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

/**
 * @class RankingHandler
 * @brief Maneja las clasificaciones del juego, incluyendo puntajes y nombres de jugadores.
 */
class RankingHandler {

private:
    int livesLeft;              ///< Número de vidas restantes.
    int time;                   ///< Tiempo tomado para completar el juego.
    string gameMode;            ///< Modo del juego (por ejemplo, fácil, medio, difícil).
    fstream file;               ///< Flujo de archivo para manejar el archivo de clasificaciones.
    vector<pair<string, int>> rankings;  ///< Vector para almacenar puntajes y nombres de jugadores.

public:
    /**
     * @brief Calcula el puntaje basado en el tiempo, modo de juego y vidas restantes.
     * @param time Tiempo tomado para completar el juego.
     * @param gameMode Modo del juego.
     * @param livesLeft Vidas restantes del jugador.
     * @return Puntaje calculado.
     */
    int calculateScore(int time, const string &gameMode, int livesLeft);

    /**
     * @brief Obtiene el nombre del jugador.
     * @return Nombre del jugador.
     */
    string getPlayerName();

    /**
     * @brief Abre el archivo de clasificaciones.
     */
    void openFileOfRankings();

    /**
     * @brief Agrega un puntaje al archivo de clasificaciones.
     * @param score Puntaje del jugador.
     * @param name Nombre del jugador.
     */
    void addScore(int score, string name);

    /**
     * @brief Ordena los puntajes en orden descendente.
     */
    void orderScores();

    /**
     * @brief Obtiene el puntaje en una posición específica de la clasificación.
     * @param position Posición en la clasificación (índice basado en 1).
     * @return Puntaje en la posición especificada.
     */
    int getRankingScore(int position);

    /**
     * @brief Obtiene el nombre en una posición específica de la clasificación.
     * @param position Posición en la clasificación (índice basado en 1).
     * @return Nombre en la posición especificada.
     */
    const char* getRankingName(int position);

    /**
     * @brief Carga las clasificaciones desde el archivo.
     */
    void loadRankings();

};

#endif



