#include "GameStates.hpp"
#include "GraphicsScene.hpp"
#include "StarFighter.hpp"
#include "Enemy.hpp"

PlayingGameState::PlayingGameState(GraphicsScene * scene, QState *parent) :
    QState(parent),
    m_scene(scene),
    m_stateMachine(0)
{
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



    for (int i = 0; i < 10; ++i)
    {
        Enemy *enemy = new Enemy();
        enemy->setPos(qrand() % static_cast<int>(m_scene->width()) + 1,
                      qrand() % static_cast<int>(m_scene->height() / 2) + 1);
        enemy->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
                            m_scene->height() / m_scene->screenHeightCoefficient);
//        enemy->show();
        m_scene->addItem(enemy);

    }

//    enemy = new Enemy();
//    enemy->setPos(500 , 500);
//    enemy->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
//                                         m_scene->height() / m_scene->screenHeightCoefficient);
//    enemy->show();
//    m_scene->addItem(enemy);
}

