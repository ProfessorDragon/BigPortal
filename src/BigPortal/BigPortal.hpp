#pragma once

#include <Geode/Geode.hpp>
#include <smjs.object-collab/include/object_collab.hpp>

using namespace geode::prelude;
using namespace object_collab::prelude;

class $object(BigPortal, EffectGameObject) {
   public:
    static const ccColor3B EFFECT_COLOR;

    static const std::string FRAME_BACK;

    static const std::string FRAME_FRONT;

    static const int PARENT_MODE;

    static const float PLAYER_SIZE;

    static const std::string STRING_ID;

   public:
    static bool is(int objectId) {
        return objectId == ObjectAPI::getCustomObjectNumericID(STRING_ID);
    }

    static bool is(GameObject* obj) { return is(obj->m_objectID); }

   public:
    static BigPortal* create(ObjectInfo* info);

    static PopupOptions getEditSpecialConfig(const Selected& selected);

    BigPortal(ObjectInfo* info)
        : CustomObject(info, ObjectTraits::builder()
                                 .gameObjectType(GameObjectType::MiniSizePortal)
                                 .defaultMainColorID(0)
                                 .defaultZLayer(ZLayer::T1)
                                 .build()) {}

    static void setPlayerSize(PlayerObject* player, float s);

    void activatedByPlayer(PlayerObject* player) override;

    void playShineEffect();

    void postPlayLayerInit() override;

    void runScaleAction(PlayerObject* player);

    void spawnLightning(PlayerObject* player);

    void spawnPortalCircle(PlayerObject* player);

    void spawnScaleCircle(PlayerObject* player);
};
