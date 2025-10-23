#include "Drawings.hpp"

// Función encargada de dibujar el tablero de juego, las pistas, minas, etc.
void DrawGameBoard(GameMechanics* game) {
    if (!game || !game->getMatrix()) {
        TraceLog(LOG_ERROR, "Game or matrix not initialized!");
        return;
    }

    int boardWidth = game->getMatrix()->cols * cellSize;
    int boardHeight = game->getMatrix()->rows * cellSize;

    int startX = (screenWidth - boardWidth) / 2;
    int startY = (screenHeight - boardHeight) / 2;
    // Mostrar en pantalla el número de vidas
    DrawText(TextFormat("Vidas: %d", game->getLivesLeft()), 194, 470, 20, BLACK);
    // Mostrar en pantalla la cantidad de minas
    DrawText(TextFormat("Minas: %d", game->getMinesQty()), 570, 470, 20, BLACK);
    for (int y = 0; y < game->getMatrix()->rows; y++) {
        for (int x = 0; x < game->getMatrix()->cols; x++) {
            char cellValue = game->getMatrix()->board[y * game->getMatrix()->cols + x];
            Rectangle cellRect = {
                static_cast<float>(startX + x * cellSize),
                static_cast<float>(startY + y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };

            if (game->revealedCells[y * game->getMatrix()->cols + x]) {
                DrawRectangleRec(cellRect, COLOR_REVEALED_LIGHT_BROWN);
                DrawRectangleLinesEx(cellRect, 1, COLOR_REVEALED_DARK_BROWN);
                if (cellValue != '0') {
                    if (cellValue == 'G') {
                        DrawRectangleRec(cellRect, COLOR_MINE);
                    } else {
                        DrawText(TextFormat("%c", cellValue), 
                            cellRect.x + cellSize / 2 - 5, 
                            cellRect.y + cellSize / 2 - 10, 
                            20, WHITE);
                    }
                }
            } else if (cellValue == 'G') {
                DrawRectangleRec(cellRect, COLOR_MINE);
            } else {
                DrawRectangleRec(cellRect, COLOR_UNREVEALED_LIGHT_GREEN);
                DrawRectangleLinesEx(cellRect, 1, COLOR_UNREVEALED_DARK_GREEN);

                if (cellValue == '5') {
                    DrawRectangleRec(cellRect, COLOR_FLAG);
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();
        for (int y = 0; y < game->getMatrix()->rows; y++) {
            for (int x = 0; x < game->getMatrix()->cols; x++) {
                Rectangle cellRect = {
                    static_cast<float>(startX + x * cellSize),
                    static_cast<float>(startY + y * cellSize),
                    static_cast<float>(cellSize),
                    static_cast<float>(cellSize)
                };
                if (CheckCollisionPointRec(mousePoint, cellRect)) {
                    bool isLeftClick = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                    game->cellClicked(x, y, isLeftClick);
                }
            }
        }
    }
}


// Funcion para dibujar los botones
void DrawButton(Button &btn) {
    // Modifica el color de los botones dependiendo de cual boton sea
    Color baseColor = (strcmp(btn.text, "Salir") == 0) ? RED_E30404 :
                      (strcmp(btn.text, "Difícil") == 0) ? RED_E30404 :
                      (strcmp(btn.text, "Medio") == 0) ? YELLOW_D4A323 :
                      GREEN289E34;

    // Si el botón está en estado hover, el color será gris
    Color color = btn.hover ? GRAY : (btn.active ? DARKGRAY : baseColor);

    DrawRectangleRec(btn.rec, color);
    DrawRectangleLinesEx(btn.rec, 2, BLACK);
    int textWidth = MeasureText(btn.text, 20);
    DrawText(btn.text, btn.rec.x + (btn.rec.width - textWidth) / 2, btn.rec.y + 10, 20, WHITE);
}

// Dibujar la imagen en boton
void DrawImageButton(Rectangle rec, Texture2D texture, bool hover) {
    // Dibujar la textura ajustada al tamaño del botón
    DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, rec, { 0, 0 }, 0.0f, hover ? Fade(WHITE, 0.6f) : WHITE);
    DrawRectangleLinesEx(rec, 2, BLACK);
}
