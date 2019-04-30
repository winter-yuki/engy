#include "component.hpp"


namespace Engy
{

void Component::setEntity(Entity * e)
{
    entity_ = e;
}


Entity * Component::entity()
{
    return entity_;
}

} // Engy
