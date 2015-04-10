#include "GameDataParser.hpp"

#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

void GameDataParser::allLevelsDescription(QList<LevelDescription> &gameDescription,
                                          int screenWidth, int screenHeight)
{
    //parse the xml that contain all data of the game
    QFile file(":gamedata.xml");
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader reader;
    reader.setDevice(&file);

    LevelDescription currentLevel;
    LevelDescription::EnemyWave currentWave;

    while (!reader.atEnd())
    {
         reader.readNext();

         if (reader.tokenType() == QXmlStreamReader::StartElement)
         {
             if (reader.name() == "enemy")
             {
//                 qDebug() << reader.attributes().value("name").toString();
//                 qDebug() << reader.attributes().value("points").toString();
//                 qDebug() << reader.attributes().value("type").toString();
             }
             else if (reader.name() == "level")
             {
                currentLevel.setLevelId(reader.attributes().value("id").toString().toInt());
                currentLevel.setLevelTitle(reader.attributes().value("title").toString());
             }
             else if (reader.name() == "enemyWave")
             {

                 currentWave.type = reader.attributes().value("type").toString();
                 currentWave.enemyNumber = reader.attributes().value("enemyNumber").toString().toInt();

                 QString enterSide = std::move(reader.attributes().value("enterSide").toString());

                 if (enterSide == "top")
                 {
                     currentWave.enterPosition.setX(screenWidth * reader.attributes().value("attitude").toString().toDouble());
                     currentWave.enterPosition.setY(0);
                 }
                 else if (enterSide == "left")
                 {
                     currentWave.enterPosition.setX(0);
                     currentWave.enterPosition.setY(screenHeight * reader.attributes().value("attitude").toString().toDouble());
                 }
                 else if (enterSide == "right")
                 {
                     currentWave.enterPosition.setX(screenWidth);
                     currentWave.enterPosition.setY(screenHeight * reader.attributes().value("attitude").toString().toDouble());
                 }
             }
         }
         else if (reader.tokenType() == QXmlStreamReader::EndElement)
         {
            if (reader.name() == "level")
            {
                gameDescription.append(currentLevel);
                currentLevel.enemyWaves().clear();
            }

            if (reader.name() == "enemyWave")
            {
                currentLevel.enemyWaves().append(currentWave);
            }
         }
   }
}
