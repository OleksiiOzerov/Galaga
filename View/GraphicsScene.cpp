#include "GraphicsScene.hpp"
#include "PixmapItem.hpp"

#include <QKeyEvent>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QStateMachine>
#include <QtCore/QFinalState>
#include <QtCore/QPauseAnimation>
#include <QtWidgets/QAction>

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <QTimer>
#include <QDebug>

GraphicsScene::GraphicsScene(int x, int y, int width, int height) :
    QGraphicsScene(x , y, width, height), m_GameStared(false), m_Timer(nullptr)
{
    PixmapItem *backgroundItem = new PixmapItem(QString(":/Pictures/background"));

    backgroundItem->setZValue(1);
    backgroundItem->setPos(0,0);
    backgroundItem->resizePixmap(800, 600);
    QGraphicsScene::addItem(backgroundItem);

    static const int nLetters = 6;
    static struct {
        char const *pix;
        qreal initX, initY;
        qreal destX, destY;
    } logoData[nLetters] = {
        {"g", -1000, -1000, 300, 150 },
        {"a",  -800, -1000, 350, 150 },
        {"l",  -600, -1000, 400, 120 },
        {"a",  -400, -1000, 460, 150 },
        {"g",  1000,  2000, 350, 250 },
        {"a",   800,  2000, 400, 250 }
    };

    lettersGroupMoving = new QSequentialAnimationGroup(this);
    lettersGroupFading = new QParallelAnimationGroup(this);

    for (int i = 0; i < nLetters; ++i) {
        PixmapItem *logo = new PixmapItem(QLatin1String(":/Pictures/Welcome/logo-") + logoData[i].pix, this);
        logo->setPos(logoData[i].initX, logoData[i].initY);
        logo->setZValue(i + 3);
        //creation of the animations for moving letters
        QPropertyAnimation *moveAnim = new QPropertyAnimation(logo, "pos", lettersGroupMoving);
        moveAnim->setEndValue(QPointF(logoData[i].destX, logoData[i].destY));
        moveAnim->setDuration(200);
        moveAnim->setEasingCurve(QEasingCurve::OutElastic);
        lettersGroupMoving->addPause(50);
        //creation of the animations for fading out the letters
        QPropertyAnimation *fadeAnim = new QPropertyAnimation(logo, "opacity", lettersGroupFading);
        fadeAnim->setDuration(800);
        fadeAnim->setEndValue(0);
        fadeAnim->setEasingCurve(QEasingCurve::OutQuad);
    }
    lettersGroupMoving->start();

    //QStateMachine *machine = new QStateMachine(this);

    //This state is when the player is playing
    //PlayState *gameState = new PlayState(this, machine);

    //Final state
    //QFinalState *final = new QFinalState(machine);

    //Animation when the player enter in the game
    //QAnimationState *lettersMovingState = new QAnimationState(machine);
    //lettersMovingState->setAnimation(lettersGroupMoving);

//    //Animation when the welcome screen disappear
    //QAnimationState *lettersFadingState = new QAnimationState(machine);
    //lettersFadingState->setAnimation(lettersGroupFading);

    //if new game then we fade out the welcome screen and start playing
    //lettersMovingState->addTransition(newAction, SIGNAL(triggered()), lettersFadingState);
    //lettersFadingState->addTransition(lettersFadingState, SIGNAL(animationFinished()), gameState);

    //New Game is triggered then player start playing
    //gameState->addTransition(newAction, SIGNAL(triggered()), gameState);

    //Wanna quit, then connect to CTRL+Q
    //gameState->addTransition(quitAction, SIGNAL(triggered()), final);
    //lettersMovingState->addTransition(quitAction, SIGNAL(triggered()), final);

    //Welcome screen is the initial state
     //machine->setInitialState(lettersMovingState);

     //machine->start();

     //We reach the final state, then we quit
     //connect(machine, SIGNAL(finished()), qApp, SLOT(quit()));
}

void GraphicsScene::startGame()
{
    lettersGroupMoving->stop();

    connect(lettersGroupFading, SIGNAL(finished()), this, SLOT(gameStarted()));

    lettersGroupFading->start();
}

void GraphicsScene::gameStarted()
{
    lettersGroupFading->stop();

    m_StarFighter = new PixmapItem(QString(":/Pictures/starfighter"));

    m_StarFighter->setZValue(1);
    m_StarFighter->setPos(400, 500);
    m_StarFighter->resizePixmap(50, 50);
    QGraphicsScene::addItem(m_StarFighter);

    m_GameStared = true;
}

void GraphicsScene::moveLeft()
{
    if (m_StarFighter)
    {
        qreal newXPos = m_StarFighter->x() - 5.0;

        if (newXPos > 0)
            m_StarFighter->setX(newXPos);
    }
}

void GraphicsScene::moveRight()
{
    if (m_StarFighter)
    {
        qreal newXPos = m_StarFighter->x() + 5.0;

        if (newXPos < width() - m_StarFighter->size().width())
            m_StarFighter->setX(newXPos);
    }
}

void GraphicsScene::fire()
{
    if (!m_Timer)
    {
        m_Timer = new QTimer(this);
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateMissiles()));
        m_Timer->start(100);
    }

    m_MissilesCollection.push_back(PixmapItem(":/Pictures/bomb"));
    PixmapItem & missile = m_MissilesCollection.back();
    missile.setZValue(2);
    missile.setX(m_StarFighter->x() + m_StarFighter->size().rwidth() / 2 - missile.size().width() / 2);
    missile.setY(m_StarFighter->y());
    QGraphicsScene::addItem(&missile);
}

void  GraphicsScene::updateMissiles()
{
    for (auto it = m_MissilesCollection.begin(); it != m_MissilesCollection.end(); ++it)
    {
        it->setY(it->y() - 10.0);
    }
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{

    switch (event->key())
    {
    case Qt::Key_Space:
        if (!m_GameStared)
            startGame();
        else
            fire();
        break;

    case Qt::Key_Left:
        moveLeft();
        break;

    case Qt::Key_Right:
        moveRight();
        break;

    }
}
