#include <Geode/Geode.hpp>
#include "BigPortal/BigPortal.hpp"
#include "PlayerObject.cpp"

using namespace geode::prelude;

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(MyGJBaseGameLayer, GJBaseGameLayer)
{
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

    // https://github.com/camila314/gd-custom-object/blob/master/src/hooks.cpp
    // struct Fields
    // {
    //     CCDictionaryExt<int, CCNode> m_effectLayerMap;
    // };

    // CCNode *parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject)
    // {
    //     if (parentMode == BigPortal::PARENT_MODE)
    //     {
    //         auto elm = m_fields->m_effectLayerMap;
    //         if (elm.size() == 0)
    //         {
    //             for (int z : {-3, -1, 1, 3, 5, 7, 9, 11})
    //             {
    //                 auto node = CCNode::create();
    //                 elm[z] = node;
    //                 m_objectLayer->addChild(node, z);
    //             }
    //         }
    //         return elm[zLayer];
    //     }

    //     return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);
    // }

    // https://github.com/glow13/CustomObjectsAPI/blob/a8c341d22e1ffcf67cba01bd86569758a80c34b3/src/hooks/GJBaseGameLayer.cpp
    struct Fields
    {
        CCSpriteBatchNode *m_customLayerT4;
        CCSpriteBatchNode *m_customBlendingLayerT4;
        CCSpriteBatchNode *m_customLayerT3;
        CCSpriteBatchNode *m_customBlendingLayerT3;
        CCSpriteBatchNode *m_customLayerT2;
        CCSpriteBatchNode *m_customBlendingLayerT2;
        CCSpriteBatchNode *m_customLayerT1;
        CCSpriteBatchNode *m_customBlendingLayerT1;
        CCSpriteBatchNode *m_customLayerB1;
        CCSpriteBatchNode *m_customBlendingLayerB1;
        CCSpriteBatchNode *m_customLayerB2;
        CCSpriteBatchNode *m_customBlendingLayerB2;
        CCSpriteBatchNode *m_customLayerB3;
        CCSpriteBatchNode *m_customBlendingLayerB3;
        CCSpriteBatchNode *m_customLayerB4;
        CCSpriteBatchNode *m_customBlendingLayerB4;
        CCSpriteBatchNode *m_customLayerB5;
        CCSpriteBatchNode *m_customBlendingLayerB5;
    };

    CCSpriteBatchNode *createAndAddBatchLayer(CCTexture2D *sheet, CCSpriteBatchNode *copy, bool blend)
    {
        auto batch = CCSpriteBatchNode::createWithTexture(sheet);
        m_objectLayer->addChild(batch);

        batch->setUseChildIndex(true);
        batch->setZOrder(copy->getZOrder());
        if (blend)
            batch->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

        return batch;
    }

    void setupLayers()
    {
        GJBaseGameLayer::setupLayers();

        std::string quality;
        switch (CCDirector::get()->getLoadedTextureQuality())
        {
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
        if (!sheet)
        {
            log::error("Failed to locate spritesheet!");
            return;
        }

        m_fields->m_customLayerT4 = createAndAddBatchLayer(sheet, m_gameLayerT4, false);
        m_fields->m_customBlendingLayerT4 = createAndAddBatchLayer(sheet, m_gameBlendingLayerT4, true);
        m_fields->m_customLayerT3 = createAndAddBatchLayer(sheet, m_gameLayerT3, false);
        m_fields->m_customBlendingLayerT3 = createAndAddBatchLayer(sheet, m_gameBlendingLayerT3, true);
        m_fields->m_customLayerT2 = createAndAddBatchLayer(sheet, m_gameLayerT2, false);
        m_fields->m_customBlendingLayerT2 = createAndAddBatchLayer(sheet, m_gameBlendingLayerT2, true);
        m_fields->m_customLayerT1 = createAndAddBatchLayer(sheet, m_gameLayerT1, false);
        m_fields->m_customBlendingLayerT1 = createAndAddBatchLayer(sheet, m_gameBlendingLayerT1, true);
        m_fields->m_customLayerB1 = createAndAddBatchLayer(sheet, m_gameLayerB1, false);
        m_fields->m_customBlendingLayerB1 = createAndAddBatchLayer(sheet, m_gameBlendingLayerB1, true);
        m_fields->m_customLayerB2 = createAndAddBatchLayer(sheet, m_gameLayerB2, false);
        m_fields->m_customBlendingLayerB2 = createAndAddBatchLayer(sheet, m_gameBlendingLayerB2, true);
        m_fields->m_customLayerB3 = createAndAddBatchLayer(sheet, m_gameLayerB3, false);
        m_fields->m_customBlendingLayerB3 = createAndAddBatchLayer(sheet, m_gameBlendingLayerB3, true);
        m_fields->m_customLayerB4 = createAndAddBatchLayer(sheet, m_gameLayerB4, false);
        m_fields->m_customBlendingLayerB4 = createAndAddBatchLayer(sheet, m_gameBlendingLayerB4, true);
        m_fields->m_customLayerB5 = createAndAddBatchLayer(sheet, m_gameLayerB5, false);
        m_fields->m_customBlendingLayerB5 = createAndAddBatchLayer(sheet, m_gameBlendingLayerB5, true);
    }

    CCNode *parentForZLayer(int zLayer, bool blending, int parentMode, int uiObject)
    {
        if (parentMode != BigPortal::PARENT_MODE || uiObject || m_fields->m_customLayerB1 == nullptr)
            return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);

        if (!blending)
            switch (static_cast<ZLayer>(zLayer))
            {
            case ZLayer::T4:
                return m_fields->m_customLayerT4;
            case ZLayer::T3:
                return m_fields->m_customLayerT3;
            case ZLayer::T2:
                return m_fields->m_customLayerT2;
            case ZLayer::T1:
                return m_fields->m_customLayerT1;
            case ZLayer::B1:
                return m_fields->m_customLayerB1;
            case ZLayer::B2:
                return m_fields->m_customLayerB2;
            case ZLayer::B3:
                return m_fields->m_customLayerB3;
            case ZLayer::B4:
                return m_fields->m_customLayerB4;
            case ZLayer::B5:
                return m_fields->m_customLayerB5;
            default:
                return m_fields->m_customLayerB1;
            }

        switch (static_cast<ZLayer>(zLayer))
        {
        case ZLayer::T4:
            return m_fields->m_customBlendingLayerT4;
        case ZLayer::T3:
            return m_fields->m_customBlendingLayerT3;
        case ZLayer::T2:
            return m_fields->m_customBlendingLayerT2;
        case ZLayer::T1:
            return m_fields->m_customBlendingLayerT1;
        case ZLayer::B1:
            return m_fields->m_customBlendingLayerB1;
        case ZLayer::B2:
            return m_fields->m_customBlendingLayerB2;
        case ZLayer::B3:
            return m_fields->m_customBlendingLayerB3;
        case ZLayer::B4:
            return m_fields->m_customBlendingLayerB4;
        case ZLayer::B5:
            return m_fields->m_customBlendingLayerB5;
        default:
            return m_fields->m_customBlendingLayerB1;
        }
    }
};
