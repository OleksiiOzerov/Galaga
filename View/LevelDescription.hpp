#ifndef LEVELDESCRIPTION_HPP
#define LEVELDESCRIPTION_HPP

#include <QList>
#include <QString>
#include <QPointF>

class LevelDescription
{
public:
    class EnemyWave
    {
    public:
        unsigned int enemyNumber;
        QPointF enterPosition;
        QString type;
    };

    LevelDescription();

    void setLevelId(int id);
    int levelId() const;

    void setLevelTitle(const QString& levelTitle);
    const QString& levelTitle() const;

    QList<LevelDescription::EnemyWave>& enemyWaves();

private:
    int m_LevelId;
    QString m_LevelTitle;
    QList<EnemyWave> m_EnemyWaveCollection;
};

#endif // LEVELDESCRIPTION_HPP
