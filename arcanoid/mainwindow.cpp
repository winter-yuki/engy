#include "mainwindow.hpp"

#include "entities.hpp"
#include "components.hpp"
#include "engy/game.hpp"
#include "engy/controllers/ecarrowkeys.hpp"
#include "engy/components/move.hpp"
#include "engy/controllers/eccollisions.hpp"
#include "engy/controllers/collision_handlers.hpp"
#include "engy/controllers/ecscenebounds.hpp"
#include "engy/components/mass.hpp"


MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
{
    createGame();
}


void MainWindow::restartGame()
{
    assert(game_);
    delete game_;
    createGame();
}


void MainWindow::updateScore(int delta)
{
    score_ += delta;
    setWindowTitle(QString("SCORE: %1").arg(score_));
}


void MainWindow::createGame()
{
    assert(!game_);
    score_ = 0;
    updateScore(0);

    // Create game
    auto game_ = new Engy::Game(this);
    game_->setSceneSize({1000, 900});
    game_->setBg(QPixmap(":/images/hydra.jpg").scaled(game_->sceneSize()));
    setFixedSize(game_->size());

    // Create player
    double borderWidth = 5;
    auto player = Engy::Entity::create<Player>(game_);
    auto keyController = Engy::Controller::create<Engy::ECArrowKeys>(player);
    keyController->setDx(25);
    keyController->setDy(std::nullopt);
    keyController->setLBorder(borderWidth);
    keyController->setRBorder(borderWidth);

    // Add bounds
    auto borders = makeBorders(game_, borderWidth);

    // Create ball
    auto ball = Engy::Entity::create<Ball>(game_);
    ball->form()->moveBy(100, 100);

    auto move = Engy::Component::create<Engy::Move>();
    move->setV({.3f, 0.4f});
    ball->addComponent(move);

    auto collisions = Engy::Controller::create<Engy::ECCollisions>(ball);
    collisions->setHandler(Engy::basicCollisionHandler);

    auto outOfScene = Engy::Controller::create<Engy::ECSceneBounds>(ball);
    QObject::connect(outOfScene, &Engy::ECSceneBounds::isOut, [outOfScene] {
        delete outOfScene->entity();
    });

    // Launch
    game_->launch();
}













