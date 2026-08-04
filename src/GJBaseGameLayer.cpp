#include <Geode/Geode.hpp>

#include "BigPortal/BigPortal.hpp"
#include "PlayerObject.cpp"

using namespace geode::prelude;

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(MyGJBaseGameLayer, GJBaseGameLayer) {
    // unnecessary decomp, at least it was easy lmao
    // see PlayerObject::propellPlayer

    // void bumpPlayer(PlayerObject *player, EffectGameObject *object)
    // {
    //     auto myPlayer = static_cast<MyPlayerObject *>(player);
    //     if (!myPlayer || !myPlayer->isBig())
    //     {
    //         GJBaseGameLayer::bumpPlayer(player, object);
    //         return;
    //     }

    //     if (canBeActivatedByPlayer(player, object))
    //         return;

    //     // why???
    //     player->m_lastPortalPos = object->getPosition() + CCPoint{0.f, -10.f};

    //     float bumpMod = 1.f;

    //     if (object->getType() == GameObjectType::PinkJumpPad)
    //     {
    //         if (player->m_isShip)
    //             bumpMod = .35f;
    //         else if (player->m_isBird)
    //             bumpMod = .4f;
    //         else if (player->m_isBall)
    //             bumpMod = .7f;
    //         else if (player->m_isSpider)
    //             bumpMod = .7f;
    //         else
    //             bumpMod = .65f;
    //     }
    //     else if (object->getType() == GameObjectType::RedJumpPad)
    //     {
    //         if (player->m_isShip)
    //             bumpMod = player->m_vehicleSize >= 1.0 ? .63f : .95f;
    //         else if (player->m_isBird)
    //             bumpMod = player->m_vehicleSize >= 1.0 ? .6f : .98f;
    //         else
    //             bumpMod = 1.25f;
    //     }

    //     player->m_lastActivatedPortal = object;

    //     if (object->m_isReverse)
    //         player->reversePlayer(object);

    //     player->bumpPlayer(bumpMod, (int)object->getType(), object->m_hasNoEffects, object);

    //     gameEventTriggered(objectTypeToGameEvent((int)object->getType()), 0, 0);
    //     gameEventTriggered(GJGameEvent::PadActivated, 0, 0);
    // }

    // https://github.com/glow13/CustomObjectsAPI/blob/a8c341d22e1ffcf67cba01bd86569758a80c34b3/src/hooks/GJBaseGameLayer.cpp
    // custom layers are necessary to render the portal-back frame from the custom spritesheet.
    // it won't work with builtin layers because they render as a batch with builtin spritesheets.
    struct Fields {
        CCSpriteBatchNode* m_customLayerB1;
        CCSpriteBatchNode* m_customBlendingLayerB1;
    };

    CCSpriteBatchNode* createAndAddBatchLayer(CCTexture2D* sheet, CCSpriteBatchNode* copy,
                                              bool blend) {
        auto batch = CCSpriteBatchNode::createWithTexture(sheet);
        m_objectLayer->addChild(batch);

        batch->setUseChildIndex(true);
        batch->setZOrder(copy->getZOrder());
        if (blend) {
            batch->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
        }

        return batch;
    }

    void setupLayers() {
        GJBaseGameLayer::setupLayers();

        std::string quality;
        switch (CCDirector::get()->getLoadedTextureQuality()) {
            case kTextureQualityLow:
                quality = "";
                break;
            case kTextureQualityMedium:
                quality = "-hd";
                break;
            case kTextureQualityHigh:
                quality = "-uhd";
                break;
        }

        auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        auto sheet = cache->spriteFrameByName(BigPortal::FRAME_FRONT.c_str())->getTexture();
        if (!sheet) {
            log::error("Failed to locate spritesheet!");
            return;
        }

        m_fields->m_customLayerB1 = createAndAddBatchLayer(sheet, m_gameLayerB1, false);
        m_fields->m_customBlendingLayerB1 =
            createAndAddBatchLayer(sheet, m_gameBlendingLayerB1, true);
    }

    CCNode* parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject) {
        if (parentMode != BigPortal::PARENT_MODE || uiObject ||
            m_fields->m_customLayerB1 == nullptr) {
            return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);
        }

        return blending ? m_fields->m_customBlendingLayerB1 : m_fields->m_customLayerB1;
    }
};
