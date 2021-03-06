/**
  * @file
  */

/**
  * @defgroup controllers
  * @brief Overseers of entities that can add some interactive to the game.
  */

#pragma once

#include "stdafx.hpp"
#include "entity.hpp"
#include "defs.hpp"


namespace Engy
{

class Game;

/**
 * @ingroup controllers
 * @brief Base class for any controller in game.
 *
 * By default deletes itself if entity deleted and if game deleted.
 *
 * QGraphicsItem provides key events for derived classes.
 *
 * Define constructors of derived classes like this:
 * @code
 * class DerivedController
 *        : public Controller {
 *     Q_OBJECT // Mb if is needed
 *     ENGY_CREATABLE_CONTROLLER
 *
 * engy_controller_ctor:
 *     DerivedController(...);
 *
 * ...
 *
 * };
 * @endcode
 * to avoid controller creatoin on stack. Because default behaviour is
 * self deletion if parents (game or entity) deleted.
 */
class Controller
        : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Creates specific derived class.
     * @param args Class constructor arguments.
     * @return Pointer to controller.
     */
    template <class C, class... Args>
    static C * create(Args && ...args);

    /**
     * @brief game
     * @return Pointer to game, controller binded with.
     */
    Game * game();
    Game const * game() const;
    /**
     * @brief entuty
     * @return Shared pointer to entity.
     * If pointer is empty, emits "entityDeleted" and returnes empty sharded.
     */
    Entity * entity();
    Entity const * entity() const;

protected slots:
    /**
     * @brief harakiri Controller deletes itself.
     */
    void harakiri();

protected:
    /**
     * @brief Controller
     * @param game
     * @param entity
     */
    explicit Controller(Entity * entity);

    /**
     * @brief deleteControllerOnEntityDeleted
     * @param val If controller should delete itself if entity deleted.
     * @warning It should be "return"  statement in controller method
     * after emitting "entityDeleted" on val == true.
     */
    void deleteControllerIfEntityDeleted(bool val = true);
    /**
     * @brief deleteControllerIfGameDeleted
     * @param val If controller should delete itself if game deleted.
     */
    void deleteControllerIfGameDeleted(bool val = true);

private:
    Game   * game_;
    Entity * entity_;

    bool deleteControllerIfEntityDeleted_ = false;
    bool deleteControllerIfGameDeleted_   = false;
};


template <class C, class... Args>
C * Controller::create(Args && ...args) {
    static_assert (std::is_base_of_v<Controller, C>,
                   "class C should be controller");
    return new C(std::forward<Args>(args)...);
}


#define ENGY_CREATABLE_CONTROLLER ENGY_CREATABLE(Controller)
#define engy_controller_ctor ENGY_HEAP_ONLY_CONSTRUCTIBLE

} // Engy
