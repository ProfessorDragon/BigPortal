#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerCheckpoint.hpp>
class $modify(MyPlayerCheckpoint, PlayerCheckpoint) {
    struct Fields {
        float m_vehicleSize = 1.f;
    };
};
