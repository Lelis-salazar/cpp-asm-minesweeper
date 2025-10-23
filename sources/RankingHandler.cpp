#include "RankingHandler.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int RankingHandler::calculateScore(int time, const string &gameMode, int livesLeft) {
    int score = 1000;
    if (gameMode == "facil") {
        score = std::max(0, score - time);
        score = std::max(0, score - 100);
        score = score + (livesLeft * 50);
    } else if (gameMode == "medio") {
        score = std::max(0, score - time);
        score = score + (livesLeft * 500);
    } else if (gameMode == "dificil") {
        score = std::max(0, score - time);
        score = score + 1000;
    }
    return score;
}

void RankingHandler::openFileOfRankings() {
    fstream ReadFile("rankings.csv", ios::in);
    if(!ReadFile) {
        fstream CreateFile("rankings.csv", ios::out);
        CreateFile<<"Nombre,Puntaje"<<endl;
    }
}

void RankingHandler::addScore(int score, string name) {
    ofstream file("rankings.csv", ios::app);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo rankings.csv para escritura" << endl;
        return;
    }
    string playerName = name;
    file << playerName << "," << score << endl;
    file.close(); // Cerrar el archivo después de escribir
}


void RankingHandler::orderScores() {
    ifstream readFile("rankings.csv");
    if (!readFile.is_open()) {
        cerr << "Error al abrir el archivo rankings.csv para lectura" << endl;
        return;
    }

    vector<pair<string, int>> rankings;
    string line, name;
    int score;

    getline(readFile, line); // Ignorar encabezado
    while (getline(readFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            name = line.substr(0, commaPos);
            score = stoi(line.substr(commaPos + 1));
            rankings.push_back(make_pair(name, score));
        }
    }
    readFile.close();

    // Ordenar los datos
    sort(rankings.begin(), rankings.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        return b.second < a.second;
    });

    // Reescribir el ranking ordenado
    ofstream writeFile("rankings.csv");
    if (!writeFile.is_open()) {
        cerr << "Error al abrir el archivo rankings.csv para escritura" << endl;
        return;
    }

    writeFile << "Nombre,Puntaje" << endl;
    for (const auto &entry : rankings) {
        writeFile << entry.first << "," << entry.second << endl;
    }
    writeFile.close();
}

void RankingHandler::loadRankings() {
    ifstream readFile("rankings.csv");
    if (!readFile.is_open()) {
        cerr << "Error al abrir el archivo rankings.csv para lectura" << endl;
        return;
    }

    rankings.clear();
    string line, name;
    int score;

    getline(readFile, line); // Ignorar encabezado
    while (getline(readFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            name = line.substr(0, commaPos);
            score = stoi(line.substr(commaPos + 1));
            rankings.push_back(make_pair(name, score));
        }
    }
    readFile.close();
}

const char* RankingHandler::getRankingName(int position) {
    if (position < 0 || position >= rankings.size()) {
        return nullptr; // Fuera de límites
    }
    return rankings[position].first.c_str();
}

int RankingHandler::getRankingScore(int position) {
    if (position < 0 || position >= rankings.size()) {
        return 0; // Fuera de límites
    }
    return rankings[position].second;
}

/*
int main() {
    RankingHandler rankingHandler;
    string gameMode = "medio";
    int score = rankingHandler.calculateScore(100, gameMode, 2);
    printf("El puntaje es de: %d\n", score);
    rankingHandler.openFileOfRankings();
    rankingHandler.addScore(score);
    rankingHandler.orderScores();
    rankingHandler.loadRankings();
    string firstName = rankingHandler.getRankingName(0);
    int firstScore = rankingHandler.getRankingScore(0);
    cout << "Nombre en " << 0 << ": " << firstName << endl;
    printf("Puntaje en %d: %d", 0, firstScore);
    return 0;
}
*/



