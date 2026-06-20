#include <Geode/Geode.hpp>
#include "BigPortal/BigPortal.hpp"

using namespace geode::prelude;

#include <Geode/modify/GameObject.hpp>
class $modify(GameObject)
{
    static GameObject *createWithKey(int key)
    {
        if (BigPortal::is(key))
            return BigPortal::create();
        else
            return GameObject::createWithKey(key);
    }
};
