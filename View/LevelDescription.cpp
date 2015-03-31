#include "LevelDescription.hpp"

LevelDescription::LevelDescription() : m_LevelId(-1),
    m_LevelTitle("Not initialized level"),
    m_EnemyWaveCollection()
{
}

void LevelDescription::setLevelId(int id)
{
    m_LevelId = id;
}

int LevelDescription::levelId() const
{
    return m_LevelId;
}

void LevelDescription::setLevelTitle(const QString& levelTitle)
{
    m_LevelTitle = levelTitle;
}

const QString& LevelDescription::levelTitle() const
{
    return m_LevelTitle;
}

QList<LevelDescription::EnemyWave>& LevelDescription::enemyWaves()
{
    return m_EnemyWaveCollection;
}
