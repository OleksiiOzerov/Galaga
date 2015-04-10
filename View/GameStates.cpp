#include "GameStates.hpp"
#include "GraphicsScene.hpp"
#include "StarFighter.hpp"
#include "Enemy.hpp"
#include "Animation.hpp"

#include <QDebug>

PlayingGameState::PlayingGameState(GraphicsScene * scene, QState *parent) :
    QState(parent),
    m_CurrentLevel(0),
    m_CurrentWave(0),
    m_scene(scene),
    m_stateMachine(0)
{
}

void PlayingGameState::startWave()
{
    if (m_scene->m_GameDescription[m_CurrentLevel].enemyWaves().size() > m_CurrentWave)
    {
        qDebug() << "Wave =  " << m_CurrentWave;
        qDebug() << "Wave enemy number " << m_scene->m_GameDescription[m_CurrentLevel].enemyWaves()[m_CurrentWave].enemyNumber;
        for (unsigned int i = 0; i < m_scene->m_GameDescription[m_CurrentLevel].enemyWaves()[m_CurrentWave].enemyNumber; ++i)
        {
            Enemy *enemy = new Enemy();
            enemy->setPos(m_scene->m_GameDescription[m_CurrentLevel].enemyWaves()[m_CurrentWave].enterPosition + QPointF(0, -100));
            enemy->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
                                m_scene->height() / m_scene->screenHeightCoefficient);
            m_scene->addItem(enemy);

            //creation of the animations for moving letters
            Animation *moveAnimation = new Animation(enemy, "pos");
            moveAnimation->setScreenResolution(m_scene->width(), m_scene->height());
            moveAnimation->setEndValue(QPointF(m_scene->height() * 0.1 + i * enemy->size().width(),
                                          m_scene->height() * 0.1));
            moveAnimation->setDuration(5000);
            moveAnimation->setEasingCurve(QEasingCurve::Linear);
            moveAnimation->addPause(i * 500);
            moveAnimation->start();
        }
    }
}

void PlayingGameState::startLevel()
{
    if (m_scene->m_GameDescription.size() > m_CurrentLevel)
    {
        qDebug() << "Level Title " << m_scene->m_GameDescription[m_CurrentLevel].levelTitle();
        qDebug() << "Level id " << m_scene->m_GameDescription[m_CurrentLevel].levelId();
        startWave();
    }
}

void PlayingGameState::onEntry(QEvent *)
{
    m_scene->m_textInformation->hide();

    m_scene->m_StarFighter->setPos(m_scene->width() / 2.0, m_scene->height() * 0.85);
    m_scene->m_StarFighter->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
                                         m_scene->height() / m_scene->screenHeightCoefficient);
    m_scene->m_StarFighter->show();
    m_scene->m_StarFighter->fly();
    m_scene->setFocusItem(m_scene->m_StarFighter, Qt::OtherFocusReason);

    connect(m_scene, SIGNAL(allEnemiesDestroyed()), this, SLOT(allEnemiesDestroyed()));

    startLevel();
}


void PlayingGameState::allEnemiesDestroyed()
{
    ++m_CurrentWave;
    if(m_scene->m_GameDescription[m_CurrentLevel].enemyWaves().size() <= m_CurrentWave)
    {
        ++m_CurrentLevel;
        m_CurrentWave = 0;
        startLevel();
    }
    else
    {
        startWave();
    }
}
