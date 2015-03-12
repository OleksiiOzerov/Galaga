#include "GraphicsScene.hpp"
#include "PixmapItem.hpp"
#include "AnimationState.hpp"

#include "StarFighter.hpp"

#include <QKeyEvent>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QStateMachine>
#include <QtCore/QFinalState>
#include <QtCore/QPauseAnimation>
#include <QtWidgets/QAction>

#include <QtWidgets/QGraphicsView>
#include <QKeyEventTransition>

#include <QDebug>

GraphicsScene::GraphicsScene(int x, int y, int width, int height) :
    QGraphicsScene(x ,y, width, height)/*, m_GameStared(false), m_Timer(nullptr)*/
{
    PixmapItem *backgroundItem = new PixmapItem(QString(":/Pictures/background"));

    backgroundItem->setZValue(1);
    backgroundItem->setPos(0,0);
    backgroundItem->resizePixmap(width, height);
    QGraphicsScene::addItem(backgroundItem);

    static const int nLetters = 6;
    static struct {
        char const *pix;
        qreal initX, initY;
        qreal destX, destY;
    } logoData[nLetters] = {
        {"g", -1000, -1000, width / 2.0 - 125, height / 2.0 },
        {"a",  -800, -1000, width / 2.0 - 75,  height / 2.0 },
        {"l",  -600, -1000, width / 2.0 - 25,  height / 2.0 },
        {"a",  -400, -1000, width / 2.0 + 25,  height / 2.0 },
        {"g",  1000,  2000, width / 2.0 + 75,  height / 2.0 },
        {"a",   800,  2000, width / 2.0 + 125, height / 2.0 }
    };

    gameLogoAnimation = new QSequentialAnimationGroup(this);
    gamelogoFadingAnimation = new QParallelAnimationGroup(this);

    for (int i = 0; i < nLetters; ++i) {
        PixmapItem *logo = new PixmapItem(QLatin1String(":/Pictures/Welcome/logo-") + logoData[i].pix, this);
        logo->setPos(logoData[i].initX, logoData[i].initY);
        logo->setZValue(i + 3);
        //creation of the animations for moving letters
        QPropertyAnimation *moveAnim = new QPropertyAnimation(logo, "pos", gameLogoAnimation);
        moveAnim->setEndValue(QPointF(logoData[i].destX, logoData[i].destY));
        moveAnim->setDuration(200);
        moveAnim->setEasingCurve(QEasingCurve::OutElastic);
        gameLogoAnimation->addPause(50);
        //creation of the animations for fading out the letters
        QPropertyAnimation *fadeAnim = new QPropertyAnimation(logo, "opacity", gamelogoFadingAnimation);
        fadeAnim->setDuration(800);
        fadeAnim->setEndValue(0);
        fadeAnim->setEasingCurve(QEasingCurve::OutQuad);
    }


    m_StarFighter = new StarFighter();

    m_StarFighter->setZValue(1);
    m_StarFighter->setPos(width / 2.0, height * 0.9);
    m_StarFighter->resizePixmap(50, 50);
    m_StarFighter->hide();
    QGraphicsScene::addItem(m_StarFighter);

}

void GraphicsScene::setupScene()
{
    QStateMachine *gameStateMachine = new QStateMachine(this);


    //Animation when the player enter in the game
    AnimationState *gameLogoState = new AnimationState(gameStateMachine);
    gameLogoState->setAnimation(gameLogoAnimation);

    //Animation when the welcome screen disappear
    AnimationState *logoFadingState = new AnimationState(gameStateMachine);
    logoFadingState->setAnimation(gamelogoFadingAnimation);

    QKeyEventTransition *startLogoFaiding = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Space);
    startLogoFaiding->setTargetState(logoFadingState);

    gameLogoState->addTransition(startLogoFaiding);
            //if new game then we fade out the welcome screen and start playing
    //gameLogoState->addTransition(newAction, SIGNAL(triggered()), lettersFadingState);


    //This state is when the player is playing
    GameStartState *gameState = new GameStartState(gameStateMachine);
    logoFadingState->addTransition(logoFadingState, SIGNAL(animationFinished()), gameState);


    connect(gameState, SIGNAL(startGame()), this, SLOT(gameStarted()));
    //Welcome screen is the initial state
    gameStateMachine->setInitialState(gameLogoState);

    gameStateMachine->start();

    //We reach the final state, then we quit
    //connect(machine, SIGNAL(finished()), qApp, SLOT(quit()));
}


//void GraphicsScene::startGame()
//{
//    gameLogoAnimation->stop();

//    connect(gamelogoFadingAnimation, SIGNAL(finished()), this, SLOT(gameStarted()));

//    gamelogoFadingAnimation->start();
//}

void GraphicsScene::gameStarted()
{
    m_StarFighter->show();
    m_StarFighter->fly();
    setFocusItem(m_StarFighter, Qt::OtherFocusReason);
}

//void GraphicsScene::moveLeft()
//{
//    if (m_StarFighter)
//    {
//        qreal newXPos = m_StarFighter->x() - 5.0;

//        if (newXPos > 0)
//            m_StarFighter->setX(newXPos);
//    }
//}

//void GraphicsScene::moveRight()
//{
//    if (m_StarFighter)
//    {
//        qreal newXPos = m_StarFighter->x() + 5.0;

//        if (newXPos < width() - m_StarFighter->size().width())
//            m_StarFighter->setX(newXPos);
//    }
//}

//void GraphicsScene::fire()
//{
//    if (!m_Timer)
//    {
//        m_Timer = new QTimer(this);
//        connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateMissiles()));
//        m_Timer->start(100);
//    }

//    m_MissilesCollection.push_back(PixmapItem(":/Pictures/bomb"));
//    PixmapItem & missile = m_MissilesCollection.back();
//    missile.setZValue(2);
//    missile.setX(m_StarFighter->x() + m_StarFighter->size().rwidth() / 2 - missile.size().width() / 2);
//    missile.setY(m_StarFighter->y());
//    QGraphicsScene::addItem(&missile);
//}

//void  GraphicsScene::updateMissiles()
//{
//    for (auto it = m_MissilesCollection.begin(); it != m_MissilesCollection.end(); ++it)
//    {
//        it->setY(it->y() - 10.0);
//    }
//}

//void GraphicsScene::keyPressEvent(QKeyEvent *event)
//{
//    qDebug() << "events GraphicsScene";
//    switch (event->key())
//    {
//    case Qt::Key_Space:
//        if (!m_GameStared)
//            startGame();
//        else
//            fire();
//        break;

//    case Qt::Key_Left:
//        moveLeft();
//        break;

//    case Qt::Key_Right:
//        moveRight();
//        break;

//    }
//    event->ignore();
//}
