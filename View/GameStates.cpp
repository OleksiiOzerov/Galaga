#include "GameStates.hpp"
#include "GraphicsScene.hpp"
#include "StarFighter.hpp"
#include "Enemy.hpp"
#include "Animation.hpp"

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


    for (int i = 0; i < m_scene->m_GameDescription[0].enemyWaves()[0].enemyNumber; ++i)
    {
        Enemy *enemy = new Enemy();
        enemy->setPos(m_scene->m_GameDescription[0].enemyWaves()[0].enterPosition + QPointF(0, -100));
        enemy->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
                            m_scene->height() / m_scene->screenHeightCoefficient);
        m_scene->addItem(enemy);

        //creation of the animations for moving letters
        Animation *moveAnim = new Animation(enemy, "pos");
        moveAnim->setEndValue(QPointF(m_scene->height() * 0.1 + i * enemy->size().width(), m_scene->height() * 0.1));
        moveAnim->setDuration(5000 + i * 500);
        moveAnim->setEasingCurve(QEasingCurve::Linear);
        moveAnim->addPause(i * 500);
        moveAnim->start();

    }

}

