#include "GameStates.hpp"
#include "GraphicsScene.hpp"
#include "StarFighter.hpp"
#include "Enemy.hpp"

#include <QPropertyAnimation>
//#include <QSequentialAnimationGroup>
//#include <QParallelAnimationGroup>

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

    //QParallelAnimationGroup * entryAnimation = new QParallelAnimationGroup(this);

//    for (int i = 0; i < m_scene->m_GameDescription[0].enemyWaves()[0].enemyNumber; ++i)
//    {
//        Enemy *enemy = new Enemy();
//        enemy->setPos(m_scene->m_GameDescription[0].enemyWaves()[0].enterPosition + QPointF(0, -100));
//        enemy->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
//                            m_scene->height() / m_scene->screenHeightCoefficient);
//        m_scene->addItem(enemy);

//        //creation of the animations for moving letters
//        QPropertyAnimation *moveAnim = new QPropertyAnimation(enemy, "pos");
//        moveAnim->setEndValue(QPointF(100 + i * enemy->size().width(), 100  + i * enemy->size().width()));
//        moveAnim->setDuration(100 + i * 1000);
//        moveAnim->setEasingCurve(QEasingCurve::Linear);
//    }
//    entryAnimation->start();

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

}

