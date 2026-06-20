#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BigPortal : public EffectGameObject
{
public:
    static const ccColor3B EFFECT_COLOR;

    static const std::string FRAME_BACK;

    static const std::string FRAME_FRONT;

    static const int OBJECT_ID;

    static const int PARENT_MODE;

    static const float PLAYER_SIZE;

protected:
    CCSprite *m_portalBack = nullptr;

public:
    static bool is(int objectId) { return objectId == OBJECT_ID; }

    static bool is(GameObject *obj) { return is(obj->m_objectID); }

public:
    static BigPortal *create();

    bool init() override;

    void customSetup() override;

    // void addMainSpriteToParent(bool reorder) override;

    // void setupCustomSprites(gd::string frameName) override;

    bool canAllowMultiActivate() override;

    bool hasBeenActivated() override;

    void resetObject() override;

    void setOpacity(unsigned char opacity) override;

    void setPosition(CCPoint const &position) override;

    void setRotation(float rotation) override;

    void setScale(float scale) override;

    void setVisible(bool visible) override;

    void triggerObject(GJBaseGameLayer *layer, int uniqueID, gd::vector<int> const *remapKeys) override;

    void playShineEffect();

    void runScaleAction(PlayerObject *player);

    void spawnLightning(PlayerObject *player);

    void spawnPortalCircle(PlayerObject *player);

    void spawnScaleCircle(PlayerObject *player);
};
