/**
 * @file Drawings.hpp
 * @brief Archivo tipo header que contiene funcionalidades de la interfaz gráfica.
 *
 * En este archivo se definen los colores, las pantallas, funciones de dibujo
 * y demás partes importantes de la interfaz gráfica del juego BuscaMinas
 */

#ifndef SCREEN_DRAWINGS_HPP
#define SCREEN_DRAWINGS_HPP

#include "raylib.h"
#include <cstring>
#include <iostream>

#include "matrixASM.hpp"
#include "GameMechanics.hpp"

// Definir los colores que se escogieron para los botones y la interfaz
#define GREEN289E34 (Color){ 40, 158, 52, 255 }
#define RED_E30404 (Color){ 227, 4, 4, 255 }
#define YELLOW_D4A323 (Color){ 212, 163, 35, 255 }
#define BROWN613710 (Color){ 97, 55, 16, 255 }
#define COLOR_UNREVEALED_DARK_GREEN (Color){46, 163, 27, 255}
#define COLOR_UNREVEALED_LIGHT_GREEN (Color){52, 204, 27, 255}
#define COLOR_REVEALED_DARK_BROWN (Color){209, 145, 50, 255}
#define COLOR_REVEALED_LIGHT_BROWN (Color){204, 133, 27, 255}
#define COLOR_MINE DARKGRAY
#define COLOR_FLAG RED

// Valores para el tamaño de la ventana
const int screenWidth = 800;
const int screenHeight = 500;
const int cellSize = 20;

// Valores para centrar el temporizador
#define TIMER_POSITION_X (screenWidth / 2 - 50)
#define TIMER_POSITION_Y (screenHeight - 30)

/**
 * @brief Estructura que representa los botones con su posición, estado y texto.
 */
struct Button {
    Rectangle rec;
    const char *text;
    bool hover;
    bool active;
};

/**
 * @brief Enum definiendo las múltiples pantallas
 */
enum GameScreen {
    SCREEN_MENU,         ///< Pantalla principal
    SCREEN_DIFFICULTY,   ///< Pantalla para escoger dificultad
    SCREEN_GAME,         ///< Pantalla de partida
    SCREEN_HELP,         ///< Pantalla con instrucciones de cómo jugar
    SCREEN_CREDITS,      ///< Créditos del juego
    SCREEN_RECORDS,      ///< Pantalla con los records
    SCREEM_ASK_NAME,     ///< Pantalla para solicitar un nombre al jugador
    SCREEN_SHOW_RECORD,  ///< Pantalla para mostrar records
    SCREEN_RESULT,       ///< Resultado de la partida
    SCREEN_WIN,          ///< Pantalla al ganar
    SCREEN_LOST,         ///< Pantalla al perder
    SCREEN_ERROR         ///< Pantalla al recibir un error con el juego
};

/**
 * @brief Dibuja el tablero de juego.
 *
 * @param game Puntero al juego en GameMechanics.
 */
void DrawGameBoard(GameMechanics* game);

/**
 * @brief Dibuja los botones sencillos del juego.
 *
 * @param btn Boton estructura que representa los botones con su posición, estado y texto.
 */
void DrawButton(Button &btn);

/**
 * @brief Dibuja los botones que se representan con imágenes.
 *
 * @param rec Rectangulo que define la posición y el tamaño de la imagen.
 * @param texture Textrura para mostrar en la imagen del botón.
 * @param hover Indica si el mouse está encima del botón.
 */
void DrawImageButton(Rectangle rec, Texture2D texture, bool hover);

#endif // SCREEN_DRAWINGS_HPP