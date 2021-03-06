#include "entities.hpp"

#include <random>

#include "components.hpp"
#include "gamewidget.hpp"
#include "engy/components/move.hpp"
#include "engy/components/collisions.hpp"
#include "engy/controllers/ecscenebounds.hpp"


using namespace std::placeholders;


Player::Player(Engy::Game * game)
    : Engy::Entity(game)
{
    const auto gsw = game->sceneSize().width();
    const auto gsh = game->sceneSize().height();
    const double width = 200;
    const double height = 15;

    auto platform = createForm({gsw / 2 - width / 2,
                                gsh - height}, {width, height});
    addForm(platform);

    setName(NAME);
}


QGraphicsItem * Player::createForm(QPointF pos, QSizeF size)
{
    auto * platform = new QGraphicsRectItem(QRectF({0, 0}, size));
    platform->setPos(pos);

    QColor color(0xAA00BB);
    platform->setPen({color});
    platform->setBrush({color});

    return platform;
}


Ball::Ball(Engy::Game * game)
    : Engy::Entity(game)
{
    QColor ballColor(QRgb(0x00AADD));

    auto * ball = new QGraphicsEllipseItem;
    ball->setRect({0, 0, 30, 30});

    ball->setBrush(QBrush(ballColor));
    ball->setPen(QPen(ballColor));

    addForm(ball);
    setName(NAME);
}


Block::Block(Engy::Game * game, QRectF rect)
    : Engy::Entity(game)
{
    auto form = new QGraphicsRectItem(rect);

    QColor color(0xAA00DD);
    form->setPen(QPen(color));
    form->setBrush(QBrush(color));

    addForm(form);
    setName(NAME);
}


MovingBlock::MovingBlock(Engy::Game * game, QRectF rect)
    : Engy::Entity(game)
{
    auto form = new QGraphicsRectItem(rect);

    QColor color(0xAA00DD);
    form->setPen(QPen(color));
    form->setBrush(QBrush(color));

    addForm(form);
    setName(NAME);
}


Bonus::Bonus(Engy::Game * game, Engy::Entity * ancestor)
    : Engy::Entity(game)
{
    assert(ancestor);

    setName(NAME);

    auto awidth  = ancestor->form()->boundingRect().width();
    auto aheight = ancestor->form()->boundingRect().height();
    auto rect = new QGraphicsRectItem(QRectF{0, 0, awidth / 2, aheight / 2});
    QColor color(0x00FF00);
    rect->setPen(color);
    rect->setBrush(color);
    addForm(rect);
    form()->setPos(ancestor->form()->pos() + QPointF{awidth / 4, aheight / 4});

    auto move = Engy::Component::create<Engy::Move>();
    move->setV({0, 0.2f});
    addComponent(move);

    auto bounds = Engy::Controller::create<Engy::ECSceneBounds>(this);
    connect(bounds, &Engy::ECSceneBounds::isOut, [this] {
        deleteLater();
    });

    auto collisions = Engy::Component::create<Engy::Collisions>();
    addComponent(collisions);
    addComponent(Engy::Component::create<Engy::DisReflector>());

    auto applyF = [this](Engy::Entity * a, Engy::Entity * b) {
        assert(a->name() == NAME);
        if (b->name() == recepient_) {
            if (applier_) {
                applier_(b);
            }
            a->deleteLater();
        }
    };
    auto apply = Engy::Component::create<Engy::FunctionHandler
            <decltype (applyF)>>(applyF);
    collisions->addHandler(apply);
}


void Bonus::setApplier(Applier a)
{
    applier_ = a;
}


void Bonus::setRecepient(QString name)
{
    recepient_ = std::move(name);
}


Trampoline::Trampoline(Engy::Game * game)
    : Engy::Entity(game)
{
    setName(NAME);
    int height = 10;
    auto rect = new QGraphicsRectItem(0, game->sceneSize().height() - height,
                                      game->sceneSize().width(), height);
    addForm(rect);
    QColor color(0x0000FF);
    rect->setBrush({color});
    rect->setPen({color});
}






