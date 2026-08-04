#include "BigPortal.hpp"

#include "../GJBaseGameLayer.cpp"

// constants

const ccColor3B BigPortal::EFFECT_COLOR = ccc3(0, 128, 255);

const std::string BigPortal::FRAME_BACK = "big-portal-back.png"_spr;

const std::string BigPortal::FRAME_FRONT = "big-portal-front.png"_spr;

const int BigPortal::PARENT_MODE = 10;

const float BigPortal::PLAYER_SIZE = 1.6f;

const std::string BigPortal::STRING_ID = "big-portal"_spr;

// functions

BigPortal* BigPortal::create(ObjectInfo* info) {
    // ObjectAPI will internally handle auto releasing and calling init.
    return new BigPortal(info);
}

PopupOptions BigPortal::getEditSpecialConfig(const Selected& selected) {
    return PopupConfig::builder()
        .width(200)
        .height(140)
        .title("Big Portal")
        .menu(AxisLayoutMenu::builder()
                  // TODO use main axis scaling @smjs
                  // TODO scale up the ToggleMenu a little bit to match rob
                  .mainAxisAlignment(MainAxisAlignment::Center)
                  .menu(ToggleMenu::builder()
                            .id("no-multi-activate"_spr)
                            .title("No Multi\nActivate")
                            .currentValue([](const Selected& selected, Popup* popup) {
                                return getCommonValueOrDefault(selected,
                                                               &BigPortal::m_isNoMultiActivate);
                            })
                            .onValue([](const bool value, const Selected& selected, Popup* popup) {
                                applyValueToSelected(selected, &BigPortal::m_isNoMultiActivate,
                                                     value);
                            })
                            .build())
                  .build())
        .build();
}

void BigPortal::setPlayerSize(PlayerObject* player, float s) {
    player->m_vehicleSize = s;
    player->m_spriteWidthScale = s;
    player->m_spriteHeightScale = s;

    // see PlayerObject::togglePlayerScale
    player->m_landParticles0->loadScaledDefaults(s);
    player->m_landParticles1->loadScaledDefaults(s);
    player->m_playerGroundParticles->loadScaledDefaults(s);
    player->m_vehicleGroundParticles->loadScaledDefaults(s);
    player->m_trailingParticles->loadScaledDefaults(s);
    player->m_shipClickParticles->loadScaledDefaults(s);
    player->m_ufoClickParticles->loadScaledDefaults(s);
    player->m_robotBurstParticles->loadScaledDefaults(s);
    player->m_dashParticles->loadScaledDefaults(s);
    player->m_swingBurstParticles1->loadScaledDefaults(s);
    player->m_swingBurstParticles2->loadScaledDefaults(s);
    player->m_regularTrail->setStroke(player->m_streakStrokeWidth * s *
                                      (player->m_isDart ? .8f : 1.f));
    if (player->m_playEffects || player->m_isDart) {
        player->m_waveTrail->m_waveSize = s;
    }
    if (player->m_ghostTrail) {
        player->m_ghostTrail->m_playerScale = s;
    }
    if (player->m_isBall && player->m_isRotating && !player->m_isLocked && !player->m_isDashing) {
        player->m_isRotating = false;
        player->m_isBallRotating2 = false;
        player->m_isBallRotating = false;
        player->m_rotationSpeed = 0.0;
        {
            // LATER contains m_vehicleSize
            player->runBallRotation(1.0);
        }
    }
    player->placeStreakPoint();
    player->updateRobotAnimationSpeed();  // LATER contains m_vehicleSize
}

void BigPortal::activatedByPlayer(PlayerObject* player) {
    EffectGameObject::activatedByPlayer(player);

    player->m_lastActivatedPortal = this;
    player->m_lastPortalPos = getPosition();

    playShineEffect();

    if (player->m_vehicleSize == PLAYER_SIZE) {
        return;
    }

    setPlayerSize(player, PLAYER_SIZE);

    auto playLayer = PlayLayer::get();
    if (player->m_playEffects && !player->m_maybeReducedEffects && playLayer &&
        !playLayer->m_skipArtReload) {
        runScaleAction(player);
        if (!m_hasNoEffects) {
            spawnLightning(player);
            spawnPortalCircle(player);
            spawnScaleCircle(player);
        }
    } else {
        player->updatePlayerScale();
    }
}

void BigPortal::customSetup() {
    EffectGameObject::customSetup();

    m_parentMode = PARENT_MODE;

    // https://github.com/glow13/CustomObjectsAPI/blob/a8c341d22e1ffcf67cba01bd86569758a80c34b3/include/object/CustomPortalObject.hpp
    // THANK YOU!!!!!
    setRawHitbox(CCSize{31.f, 90.f});
    m_baseColor->m_defaultColorID = 0;
    m_zFixedZLayer = true;
    m_defaultZLayer = ZLayer::T1;
    m_particleOffset = CCPoint{-5, 0};

    m_isTrigger = false;
    m_isSpawnTriggered = false;
    m_isTouchTriggered = true;
    m_isMultiTriggered = false;
    m_dontIgnoreDuration = false;

    if (!m_editorEnabled) {
        auto baseLayer = static_cast<MyGJBaseGameLayer*>(GJBaseGameLayer::get());
        auto parent = baseLayer->m_fields->m_customLayerB1;
        m_portalBack = CCSprite::createWithSpriteFrameName(FRAME_BACK.c_str());
        m_portalBack->setPosition(parent->convertToNodeSpace(CCPoint{}));
        parent->addChild(m_portalBack, -90);
    }

    if (!m_editorEnabled && !m_hasNoParticles) {
        auto particles = createAndAddParticle(6, "portalEffect08.plist", 4,
                                              tCCPositionType::kCCPositionTypeGrouped);
        particles->setStartColor(ccc4FFromccc3B(BigPortal::EFFECT_COLOR));
        particles->setStartColorVar(ccc4f(.25f, .25f, 0.f, .5f));
        particles->setEndColor(ccc4FFromccc3B(BigPortal::EFFECT_COLOR));
        claimParticle();
    }
}

bool BigPortal::canAllowMultiActivate() { return true; }

bool BigPortal::hasBeenActivated() {
    return EffectGameObject::hasBeenActivated() && m_isNoMultiActivate;
}

void BigPortal::setOpacity(unsigned char opacity) {
    EffectGameObject::setOpacity(opacity);
    if (m_portalBack) m_portalBack->setOpacity(opacity);
}

void BigPortal::setPosition(CCPoint const& position) {
    EffectGameObject::setPosition(position);
    if (m_portalBack) m_portalBack->setPosition(position);
}

void BigPortal::setRotation(float rotation) {
    EffectGameObject::setRotation(rotation);
    if (m_portalBack) m_portalBack->setRotation(rotation);
}

void BigPortal::setScale(float scale) {
    EffectGameObject::setScale(scale);
    if (m_portalBack) m_portalBack->setScale(scale);
}

void BigPortal::setVisible(bool visible) {
    EffectGameObject::setVisible(visible);
    if (m_portalBack) m_portalBack->setVisible(visible);
}

void BigPortal::triggerObject(GJBaseGameLayer* layer, int uniqueID,
                              gd::vector<int> const* remapKeys) {
    EffectGameObject::triggerObject(layer, uniqueID, remapKeys);

    auto player = (layer->m_player2->m_uniqueID == uniqueID) ? layer->m_player2 : layer->m_player1;
    layer->m_effectManager->removeTriggeredID(m_uniqueID, player->m_uniqueID);

    if (!layer->canBeActivatedByPlayer(player, this)) {
        return;
    }

    activatedByPlayer(player);
}

void BigPortal::playShineEffect() {
    int objectId = m_objectID;
    m_objectID = 99;
    EffectGameObject::playShineEffect();
    m_objectID = objectId;
}

void BigPortal::runScaleAction(PlayerObject* player) {
    player->m_actionManager->m_internalActions->removeObjectForKey(6);
    auto scaleTo = CCScaleTo::create(.5f, PLAYER_SIZE);
    auto ease = CCEaseElasticOut::create(scaleTo);
    ease->setTag(6);
    player->m_actionManager->m_internalActions->setObject(ease, ease->getTag());
    player->runAction(ease);
}

void BigPortal::spawnLightning(PlayerObject* player) {
    if (auto baseLayer = GJBaseGameLayer::get()) {
        baseLayer->lightningFlash(getPosition(), EFFECT_COLOR);
    }
}

void BigPortal::spawnPortalCircle(PlayerObject* player) {
    int objectId = m_objectID;
    m_objectID = 99;
    player->spawnPortalCircle(EFFECT_COLOR, 45.f);
    m_objectID = objectId;
}

void BigPortal::spawnScaleCircle(PlayerObject* player) {
    auto wave = CCCircleWave::create(10.f, 60.f, .35f, false, true);
    wave->followObject(player, true);
    wave->m_color = EFFECT_COLOR;

    if (auto parentLayer = player->getParent()) {
        parentLayer->addChild(wave);
    }

    if (auto playLayer = PlayLayer::get()) {
        playLayer->m_circleWaveArray->addObject(wave);
    }

    // for green portal:
    // color = ccc3(0, 255, 150)
    // duration = 0.3
    // radius = 10 -> 40

    // for pink portal:
    // color = ccc3(255, 0, 150)
    // duration = 0.25
    // radius = 50 -> 2
}
