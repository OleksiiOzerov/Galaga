#ifndef GAMEDATAPARSER_HPP
#define GAMEDATAPARSER_HPP

#include "LevelDescription.hpp"

class GameDataParser
{
private:
    GameDataParser();
    ~GameDataParser();

public:
    static void allLevelsDescription(QList<LevelDescription> &gameLevels, int screenWidth, int screenHeight);
};

#endif // GAMEDATAPARSER_HPP
