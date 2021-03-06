/**
  * @file
  */

#pragma once

#include "controller.hpp"


namespace Engy
{

/**
 * @ingroup controllers
 * @brief The ECArrowKeys class
 * Controller that handles arrow keys events.
 */
class ECArrowKeys final
        : public Controller {
    Q_OBJECT
    ENGY_CREATABLE_CONTROLLER

    engy_controller_ctor:
        explicit ECArrowKeys(Entity * entity);

public:
    using OptD = std::optional<double>;

    void setDx(OptD dx);
    OptD dx() const;
    void setDy(OptD dy);
    OptD dy() const;

    void setRBorder(OptD x);
    OptD rborder() const;
    void setLBorder(OptD x);
    OptD lborder() const;
    void setTBorder(OptD x);
    OptD tborder() const;
    void setBBorder(OptD x);
    OptD bborder() const;

public slots:
    void keyPressed(QKeyEvent * event);

private slots:
    void sceneResized();

private:
    QPointF left () const;
    QPointF right() const;
    QPointF up   () const;
    QPointF down () const;

private:
    OptD dx_ = 20;
    OptD dy_ = 20;

    OptD lborder_ = std::nullopt;
    OptD rborder_ = std::nullopt;
    OptD tborder_ = std::nullopt;
    OptD bborder_ = std::nullopt;
};

} // Engy
