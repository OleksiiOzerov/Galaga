
#include "GraphicsScene.hpp"
#include "PixmapItem.hpp"
#include "GameStates.hpp"
#include "AnimationState.hpp"
#include "StarFighter.hpp"

#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QStateMachine>
#include <QtCore/QFinalState>
//#include <QtCore/QPauseAnimation>

#include <QGraphicsView>
#include <QKeyEventTransition>
#include <QDebug>
#include <QApplication>

GraphicsScene::GraphicsScene(int x, int y, int width, int height) :
    QGraphicsScene(x, y, width, height),
    m_StarFighter(0),
    m_textInformation(0)
{
    setGameBackground(width, height);

    createGameLogoAnimation(width, height);

    createStarfighter();
}

void GraphicsScene::setGameBackground(int sceneWidth, int sceneHeight)
{
    PixmapItem *backgroundImage = new PixmapItem(QString(":/Pictures/background"));

    backgroundImage->setZValue(1);
    backgroundImage->setPos(0,0);
    backgroundImage->resizePixmap(sceneWidth, sceneHeight);
    QGraphicsScene::addItem(backgroundImage);

    m_textInformation = new QGraphicsTextItem(backgroundImage);
    m_textInformation->setPos(sceneWidth / 2 - 200, sceneHeight / 2 + 300);
    m_textInformation->setPlainText(QString("Press space to start game"));
    m_textInformation->setFont(QFont("Comic Sans MS", 30));
    m_textInformation->setDefaultTextColor(QColor(Qt::white));
    m_textInformation->hide();
}

void GraphicsScene::createGameLogoAnimation(int sceneWidth, int sceneHeight)
{
    static const int nLetters = 6;
    static struct {
        char const *pix;
        qreal initX, initY;
        qreal destX, destY;
    } logoData[nLetters] = {
        {"g", -1000, -1000, sceneWidth / 2.0 - 125, sceneHeight / 2.0 },
        {"a",  -800, -1000, sceneWidth / 2.0 - 75,  sceneHeight / 2.0 },
        {"l",  -600, -1000, sceneWidth / 2.0 - 25,  sceneHeight / 2.0 },
        {"a",  -400, -1000, sceneWidth / 2.0 + 25,  sceneHeight / 2.0 },
        {"g",  1000,  2000, sceneWidth / 2.0 + 75,  sceneHeight / 2.0 },
        {"a",   800,  2000, sceneWidth / 2.0 + 125, sceneHeight / 2.0 }
    };

    gameLogoAnimation = new QSequentialAnimationGroup(this);
    gameLogoFadingAnimation = new QParallelAnimationGroup(this);

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
        QPropertyAnimation *fadeAnim = new QPropertyAnimation(logo, "opacity", gameLogoFadingAnimation);
        fadeAnim->setDuration(800);
        fadeAnim->setEndValue(0);
        fadeAnim->setEasingCurve(QEasingCurve::OutQuad);
    }
}

void GraphicsScene::createStarfighter()
{
    m_StarFighter = new StarFighter();

    m_StarFighter->hide();
    QGraphicsScene::addItem(m_StarFighter);
}

void GraphicsScene::setupGameStateMachine()
{
    QStateMachine *gameStateMachine = new QStateMachine(this);

    //Animation when the player enter in the game
    AnimationState *gameLogoState = new AnimationState(gameStateMachine);
    gameLogoState->setAnimation(gameLogoAnimation);

    //Animation when the welcome screen disappear
    AnimationState *logoFadingState = new AnimationState(gameStateMachine);
    logoFadingState->setAnimation(gameLogoFadingAnimation);

    QKeyEventTransition *startLogoFaidingTransition = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Space);
    startLogoFaidingTransition->setTargetState(logoFadingState);
    gameLogoState->addTransition(startLogoFaidingTransition);

    //This state is when the player is playing
    PlayingGameState *gameState = new PlayingGameState(this, gameStateMachine);
    logoFadingState->addTransition(logoFadingState, SIGNAL(animationFinished()), gameState);

    //Welcome screen is the initial state
    gameStateMachine->setInitialState(gameLogoState);

    gameStateMachine->start();
    m_textInformation->show();
    //We reach the final state, then we quit
    connect(gameStateMachine, SIGNAL(finished()), qApp, SLOT(quit()));
}

