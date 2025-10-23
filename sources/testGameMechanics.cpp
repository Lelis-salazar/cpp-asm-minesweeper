#include "GameMechanics.hpp"
#include <cassert>

// Función principal para pruebas
int main() {

    // Caso de prueba 1: Hacer clic en una celda con mina y perder el juego
    {
        cout << "\nCaso de prueba 1: Click en celda con mina." << endl;
        GameMechanics game("facil");
        game.gameInit("facil");
        game.getMatrix()->updateCell(0, 0, -1);  // Simular una mina en la celda (0,0)
        game.cellClicked(0, 0, true);  // Clic izquierdo
        assert(game.getLivesLeft() == 2);  // Se debería perder una vida
        assert(game.getMatrix()->checkCell(0, 0) == 'G');  // La celda debería estar marcada como explotada
        cout << "\nCaso de prueba 1: Click en celda con mina\n****************\nPASADO" << endl;
    }

    // Caso de prueba 2: Colocar y remover bandera en una celda
    {
        cout << "\nCaso de prueba 2: Colocar y remover bandera." << endl;
        GameMechanics game("facil");
        game.gameInit("facil");
        game.putFlag(1, 1);  // Colocar bandera en la celda (1, 1)
        assert(game.getMatrix()->checkCell(1, 1) == '5');  // La celda debería estar marcada como con bandera
        game.removeFlag(1, 1);  // Remover bandera
        assert(game.getMatrix()->checkCell(1, 1) != '5');  // La celda ya no debería estar marcada como con bandera
        cout << "\nCaso de prueba 2: Colocar y remover bandera:\n****************\nPASADO" << endl;
    }

    // Caso de prueba 3: Clic en coordenadas fuera de los límites
    {
        cout << "\nCaso de prueba 3: Clic fuera de los límites." << endl;
        GameMechanics game("facil");
        game.gameInit("facil");
        game.cellClicked(-1, 0, true);  // Clic fuera de los límites
        game.cellClicked(0, -1, true);  // Clic fuera de los límites
        game.cellClicked(8, 0, true);  // Clic fuera de los límites
        game.cellClicked(0, 8, true);  // Clic fuera de los límites
        // No debería caerse el programa
        cout << "\nCaso de prueba 3: Clic fuera de los límites\n****************\nPASADO" << endl;
    }

    cout << "Todos los casos de prueba pasaron." << endl;
    return 0;
}

