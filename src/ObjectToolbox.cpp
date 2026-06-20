#include <Geode/Geode.hpp>
#include "BigPortal/BigPortal.hpp"

using namespace geode::prelude;

#include <Geode/modify/ObjectToolbox.hpp>
class $modify(ObjectToolbox)
{
    static ObjectToolbox *sharedState()
    {
        auto state = ObjectToolbox::sharedState();

        static bool injected = false;
        if (!injected)
        {
            injected = true;
            // this is the frame in the editor (specifically the delete tab)
            state->m_allKeys[BigPortal::OBJECT_ID] = BigPortal::FRAME_FRONT.c_str();
        }

        return state;
    }
};
