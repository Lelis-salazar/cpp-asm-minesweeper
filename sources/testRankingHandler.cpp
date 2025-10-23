#include <cassert>
#include <iostream>
#include <string>
#include "RankingHandler.hpp"


using namespace std;

void testMoreThanThreeChars() {

    RankingHandler rankingHandler;
    rankingHandler.addScore(1500, "MasDeTresCaracteres");
    rankingHandler.loadRankings();    
    const char* name1 = rankingHandler.getRankingName(0);
    int score1 = rankingHandler.getRankingScore(0);
    assert(name1 != nullptr && "Nombre no permitido");
    std::cout << "Nombre: " << name1 << endl;
    std::cout << "Record: " << score1 << endl;
    
}

void testWithoutLoading() {

    RankingHandler rankingHandler;
    rankingHandler.addScore(1500, "MasDeTresCaracteres");
    // rankingHandler.loadRankings();    
    const char* name1 = rankingHandler.getRankingName(0);
    int score1 = rankingHandler.getRankingScore(0);
    assert(name1 != nullptr && "Nombre y record no encontrados");
    std::cout << "Nombre: " << name1 << endl;
    std::cout << "Record: " << score1 << endl;
}

int main() {
    testMoreThanThreeChars();
    testWithoutLoading();

    printf("Test correctos");

    return 0;
}