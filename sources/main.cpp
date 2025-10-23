/**
 * @file main.cpp
 * @brief Programa principal del juego BuscaMinas.
 *
 * Este archivo contiene la implementación principal del juego BuscaMinas,
 * incluyendo la inicialización de la ventana, manejo de botones y pantallas,
 * gestión del tiempo, y lógica del juego utilizando las clases GameMechanics
 * y RankingHandler para el manejo de la mecánica y el ranking respectivamente.
 */

#include "matrixASM.hpp"
#include "GameMechanics.hpp"
#include "RankingHandler.hpp"
#include "Drawings.hpp"

// Variables globales para el tiempo
float startTime = 0.0f;
float elapsedTime = 0.0f;
bool gameStarted = false;

int main() {
    InitWindow(screenWidth, screenHeight, "BuscaMinas");
    // Iniciar la música
    InitAudioDevice();
    Music music = LoadMusicStream("assets/flat-8-bit-gaming-music-instrumental-211547.wav");
    PlayMusicStream(music);

    GameScreen previousScreen = SCREEN_MENU;
    GameScreen currentScreen = SCREEN_MENU;
    // Asignaciones de botones del menú
    Button menuButtons[4] = {
        {{ screenWidth / 2 - 100, 100, 200, 40 }, "JUGAR", false, false},
        {{ screenWidth / 2 - 100, 150, 200, 40 }, "Records", false, false},
        {{ screenWidth / 2 - 100, 200, 200, 40 }, "Créditos", false, false},
        {{ screenWidth / 2 - 100, 250, 200, 40 }, "Salir", false, false}
    };
    // Asignaciones de botones de pantalla al perder
    Button lostButtons[3] = {
        {{ screenWidth / 2 - 100, 100, 200, 40 }, "Jugar de nuevo", false, false},
        {{ screenWidth / 2 - 100, 150, 200, 40 }, "Ir al Menu", false, false},
        {{ screenWidth / 2 - 100, 200, 200, 40 }, "Salir", false, false}
    };
    // Asignaciones de botones de dificultad
    Button difficultyButtons[3] = {
        {{ screenWidth / 2 - 100, 100, 200, 40 }, "Fácil", false, false},
        {{ screenWidth / 2 - 100, 150, 200, 40 }, "Medio", false, false},
        {{ screenWidth / 2 - 100, 200, 200, 40 }, "Difícil", false, false}
    };

    Button backButton = {{ screenWidth / 2 - 100, screenHeight - 60, 200, 40 }, "Atrás", false, false};

    // Cargar la imagen para el botón de ayuda
    Texture2D helpButtonTexture = LoadTexture("assets/help.png");
    if (helpButtonTexture.id == 0) {
        TraceLog(LOG_ERROR, "Error: Help Button not created correctly");
    }
    Rectangle helpButton = { 10, screenHeight - 60, 50, 50 }; // Esquina inferior izquierda

    // Botón de pausa de la música
    Rectangle pauseButton = { screenWidth - 50, screenHeight - 50, 40, 40 }; // Esquina inferior derecha
    Texture2D pauseTexture = LoadTexture("assets/music.png");
    if (pauseTexture.id == 0) {
        TraceLog(LOG_ERROR, "Error: Music Button not created correctly");
    }
    bool isMusicPaused = false;

    GameMechanics* game = nullptr;
    SetTargetFPS(30);
    // Inicialización de RankingHandler (solo para pruebas, esto se elimina luego)
    RankingHandler rankingHandler;
    rankingHandler.openFileOfRankings();
    rankingHandler.loadRankings();
    int score;
    string gameMode;
    int totalInputChars = 3;
    char inputText[totalInputChars + 1] = ""; // Arreglo para almacenar los caracteres + el carácter nulo
    int currentCharCount = 0; // Contador de caracteres actuales

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        bool helpButtonHover = CheckCollisionPointRec(mousePoint, helpButton);
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        bool pauseButtonHover = CheckCollisionPointRec(mousePoint, pauseButton);
        if (pauseButtonHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isMusicPaused = !isMusicPaused;
            if (isMusicPaused) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }

        switch (currentScreen) {
            // Menu principal
            case SCREEN_MENU:
                gameStarted = false;
                elapsedTime = 0.0f;
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("BuscaMinas", screenWidth / 2 - MeasureText("BuscaMinas", 40) / 2, 20, 40, WHITE);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);
                for (int i = 0; i < 4; i++) {
                    menuButtons[i].hover = CheckCollisionPointRec(mousePoint, menuButtons[i].rec);
                    menuButtons[i].active = menuButtons[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                    DrawButton(menuButtons[i]);
                }
                if (menuButtons[0].active) {
                    currentScreen = SCREEN_DIFFICULTY;
                } else if (menuButtons[1].active) {
                    currentScreen = SCREEN_RECORDS;
                } else if (menuButtons[2].active) {
                    currentScreen = SCREEN_CREDITS;
                } else if (menuButtons[3].active) {
                    UnloadTexture(helpButtonTexture);
                    UnloadTexture(pauseTexture);
                    UnloadMusicStream(music);
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }
                break;
            // Pantalla de puntajes obtenidos
            case SCREEN_RECORDS:
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Records", screenWidth / 2 - MeasureText("Records", 40) / 2, 20, 40, WHITE);

                for (int i = 0; i < 3; i++) {
                    const char* name = rankingHandler.getRankingName(i);
                    int score = rankingHandler.getRankingScore(i);

                    // Calcular el ancho del texto
                    int textWidth = MeasureText(TextFormat("%s: %d", name, score), 30);

                    // Ajustar la posición X para centrar el texto
                    int posX = screenWidth / 2 - textWidth / 2;
                    if (score != 0) {
                        DrawText(TextFormat("%s: %d", name, score), posX, 100 + 50 * i, 30, BLACK);
                    } else {
                        DrawText("Record vacío", screenWidth / 2 - MeasureText("Record vacío", 30) / 2, 100 + 50 * i, 30, BLACK);
                    }

                    
                }

                backButton.hover = CheckCollisionPointRec(mousePoint, backButton.rec);
                backButton.active = backButton.hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                DrawButton(backButton);

                if (backButton.active) {
                    currentScreen = SCREEN_MENU;
                }
                break;
            // Pantalla de creditos
            case SCREEN_CREDITS:
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Créditos", screenWidth / 2 - MeasureText("Créditos", 40) / 2, 20, 40, WHITE);
                DrawText("Desarrolladores:", 20, 100, 20, BLACK);
                DrawText("Alejandro Jiménez R. C04079", 40, 130, 20, BLACK);
                DrawText("Lelis Salazar S. C17168", 40, 160, 20, BLACK);
                DrawText("Alejandro Solano B. C27534", 40, 190, 20, BLACK);
                DrawText("Manuel Navarro M. B85599", 40, 220, 20, BLACK);

                backButton.hover = CheckCollisionPointRec(mousePoint, backButton.rec);
                backButton.active = backButton.hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                DrawButton(backButton);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);
                if (backButton.active) {
                    currentScreen = SCREEN_MENU;
                }
                break;

            // Pantalla en caso de error con el juego
            case SCREEN_ERROR:{
                int titleFontSize = 60;
                int messageFontSize = 30;

                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);

                const char* titleText = "ERROR";
                int titleTextWidth = MeasureText(titleText, titleFontSize);
                DrawText(titleText, screenWidth / 2 - titleTextWidth / 2, 20, titleFontSize, WHITE);

                const char* messageText = "Ha ocurrido un error inesperado con el juego";
                int messageTextWidth = MeasureText(messageText, messageFontSize);
                DrawText(messageText, screenWidth / 2 - messageTextWidth / 2, 130, messageFontSize, BLACK);

                backButton.hover = CheckCollisionPointRec(mousePoint, backButton.rec);
                backButton.active = backButton.hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                DrawButton(backButton);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);

                if (backButton.active) {
                    currentScreen = SCREEN_MENU;
                }
                break;

            }
            // Pantalla de seleccion de dificultad
            case SCREEN_DIFFICULTY:
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Dificultad", screenWidth / 2 - MeasureText("Dificultad", 40) / 2, 20, 40, WHITE);

                for (int i = 0; i < 3; i++) {
                    difficultyButtons[i].hover = CheckCollisionPointRec(mousePoint, difficultyButtons[i].rec);
                    difficultyButtons[i].active = difficultyButtons[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                    DrawButton(difficultyButtons[i]);
                }
                if (game) {
                    delete game;
                    game = nullptr;
                }

                game = new GameMechanics("");
                if (difficultyButtons[0].active) {
                    game->gameInit("facil");
                    gameMode = "facil";
                    previousScreen = SCREEN_GAME;
                    currentScreen = SCREEN_GAME;
                    //currentScreen = SCREEN_ERROR;
                } else if (difficultyButtons[1].active) {
                    game->gameInit("medio");
                    gameMode = "medio";
                    previousScreen = SCREEN_GAME;
                    currentScreen = SCREEN_GAME;
                } else if (difficultyButtons[2].active) {
                    game->gameInit("dificil");
                    gameMode = "dificil";
                    previousScreen = SCREEN_GAME;
                    currentScreen = SCREEN_GAME;
                }

                if (helpButtonHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    previousScreen = SCREEN_DIFFICULTY;
                    currentScreen = SCREEN_HELP;
                }
                DrawImageButton(helpButton, helpButtonTexture, helpButtonHover);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);

                backButton.hover = CheckCollisionPointRec(mousePoint, backButton.rec);
                backButton.active = backButton.hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                DrawButton(backButton);
                if (backButton.active) {
                    currentScreen = SCREEN_MENU;
                }
                break;
            // Pantalla al presionar el boton de ayuda
            case SCREEN_HELP:
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Cómo Jugar", screenWidth / 2 - MeasureText("Cómo Jugar", 40) / 2, 20, 40, WHITE);
                DrawText("Controles:", 20, 100, 20, BLACK);
                DrawText("- Click izquierdo: Busca si hay una mina en el espacio seleccionado", 40, 130, 20, BLACK);
                DrawText("- Click derecho: Dependiendo de la dificultad, puede marcar los espacios", 40, 160, 20, BLACK);
                DrawText("Guía de dificultades:", 20, 200, 20, BLACK);
                DrawText("- Fácil: 3 vidas, contador de minas, y banderas. Reduce el puntaje final", 40, 230, 20, BLACK);
                DrawText("- Medio: 2 vidas y banderas", 40, 260, 20, BLACK);
                DrawText("- Difícil: Vida única, sin habilidades. Aumenta el puntaje final.", 40, 290, 20, BLACK);

                backButton.hover = CheckCollisionPointRec(mousePoint, backButton.rec);
                backButton.active = backButton.hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                DrawButton(backButton);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);
                if (backButton.active) {
                    if (previousScreen == SCREEN_DIFFICULTY){
                        currentScreen = SCREEN_DIFFICULTY;
                    }
                    else {
                        currentScreen = SCREEN_GAME;
                    }
                }
                break;
            // Pantalla al ganar una partida que muestra resultados
            case SCREEN_RESULT:
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Ganaste!", screenWidth / 2 - MeasureText("Ganaste!", 40) / 2, 20, 40, WHITE);

                if (currentCharCount < totalInputChars) {
                    // Leer entrada del teclado (teclas alfabéticas y numéricas)
                    int key = GetKeyPressed();
                    if (key >= 32 && key <= 125) { // Rango de caracteres ASCII imprimibles
                        inputText[currentCharCount] = (char)key;
                        currentCharCount++;
                        inputText[currentCharCount] = '\0'; // Asegurar que el string esté terminado con '\0'
                    }
                }
                DrawText("Introduza su nombre!", screenWidth / 2 - MeasureText("Introduza su nombre!", 40) / 2, 80, 40, BLACK);
                DrawText(inputText, screenWidth / 2 - MeasureText(inputText, 40) / 2, 140, 40, GREEN);
               
                DrawLine(screenWidth / 2 - MeasureText(inputText, 40) / 2, 140 + 40 + 5, screenWidth / 2 + MeasureText(inputText, 40) / 2, 140 + 40 + 5, BLACK);
    
                if (currentCharCount == totalInputChars) {
                    std::string str(inputText);
                    score = rankingHandler.calculateScore(score, gameMode, game->getLivesLeft());
                    rankingHandler.addScore(score, inputText);
                    rankingHandler.orderScores();
                    rankingHandler.loadRankings();
                    currentCharCount = 0;
                    memset(inputText, 0, sizeof(inputText));
                    currentScreen = SCREEN_WIN;
                }
                break;

            case SCREEN_WIN:
                gameStarted = false;
                elapsedTime = 0.0f;
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("Fin del juego", screenWidth / 2 - MeasureText("Fin del juego", 40) / 2, 20, 40, WHITE);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);
                for (int i = 0; i < 3; i++) {
                    lostButtons[i].hover = CheckCollisionPointRec(mousePoint, lostButtons[i].rec);
                    lostButtons[i].active = lostButtons[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                    DrawButton(lostButtons[i]);
                }
                if (lostButtons[0].active) {
                    currentScreen = SCREEN_DIFFICULTY;
                } else if (lostButtons[1].active) {
                    currentScreen = SCREEN_MENU;
                } else if (lostButtons[2].active) {
                    UnloadTexture(helpButtonTexture);
                    UnloadTexture(pauseTexture);
                    UnloadMusicStream(music);
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }
                break;
            // Pantalla al perder
            case SCREEN_LOST:
                gameStarted = false;
                elapsedTime = 0.0f;
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, 20, 40, WHITE);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);
                for (int i = 0; i < 3; i++) {
                    lostButtons[i].hover = CheckCollisionPointRec(mousePoint, lostButtons[i].rec);
                    lostButtons[i].active = lostButtons[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                    DrawButton(lostButtons[i]);
                }
                if (lostButtons[0].active) {
                    currentScreen = SCREEN_DIFFICULTY;
                } else if (lostButtons[1].active) {
                    currentScreen = SCREEN_MENU;
                } else if (lostButtons[2].active) {
                    UnloadTexture(helpButtonTexture);
                    UnloadTexture(pauseTexture);
                    UnloadMusicStream(music);
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }
                break;
            // Pantalla del juego
            case SCREEN_GAME:
                // Inicio del tiempo
                if (!gameStarted) {
                    startTime = GetTime();
                    gameStarted = true;
                }
                DrawRectangle(0, 0, screenWidth, 80, BROWN613710);
                DrawText("BuscaMinas", screenWidth / 2 - MeasureText("BuscaMinas", 40) / 2, 20, 40, WHITE);
                DrawImageButton(helpButton, helpButtonTexture, helpButtonHover);
                if (helpButtonHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = SCREEN_HELP;
                }
                DrawImageButton(helpButton, helpButtonTexture, helpButtonHover);
                DrawImageButton(pauseButton, pauseTexture, pauseButtonHover);

                // Acceder a la matriz del juego y otros datos
                // Para probar la pantalla de error, comentar este if, y descomentar el inferior
                if (game && game->getMatrix()) {
                // if (1 == 0) {    
                    elapsedTime = GetTime() - startTime;
                    int minutes = (int)(elapsedTime / 60);
                    int seconds = (int)(elapsedTime) % 60;
                    DrawGameBoard(game);
                    DrawText(TextFormat("Tiempo: %02d:%02d", minutes, seconds), TIMER_POSITION_X, TIMER_POSITION_Y, 20, BLACK);
                    // Verificar condiciones de fin de juego
                    if (game->getCellsLeft() == 0) {
                        remove("flagged_cells.txt");
                        score = (int) elapsedTime;
                        currentScreen = SCREEN_RESULT;
                        
                        gameStarted = false;
                    }
                    if (game->getLivesLeft() == 0) {
                        remove("flagged_cells.txt");
                        currentScreen = SCREEN_LOST;
                        gameStarted = false;
                    }
                } else { // Se pasa a la pantalla de error en caso de que el juego no funcione
                    TraceLog(LOG_ERROR, "Game object is not initialized!");
                    currentScreen = SCREEN_ERROR;
                }
                break;

            default:
                break;
        }

        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadTexture(helpButtonTexture);
    UnloadTexture(pauseTexture);
    CloseWindow();

    // Liberar la memoria de game si se inicializó
    if (game) {
        delete game;
    }
    remove("flagged_cells.txt");
    return 0;
}
