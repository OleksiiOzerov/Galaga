#include "GameStates.hpp"
#include "GraphicsScene.hpp"
#include "StarFighter.hpp"

PlayingGameState::PlayingGameState(GraphicsScene * scene, QState *parent) :
    QState(parent),
    m_scene(scene),
    m_stateMachine(0)
{
}

void PlayingGameState::onEntry(QEvent *)
{
    m_scene->m_textInformation->hide();
    m_scene->m_StarFighter->setZValue(2);
    m_scene->m_StarFighter->setPos(m_scene->width() / 2.0, m_scene->height() * 0.85);
    m_scene->m_StarFighter->resizePixmap(m_scene->height() / m_scene->screenHeightCoefficient,
                                         m_scene->height() / m_scene->screenHeightCoefficient);
    m_scene->m_StarFighter->show();
    m_scene->m_StarFighter->fly();
    m_scene->setFocusItem(m_scene->m_StarFighter, Qt::OtherFocusReason);
}

