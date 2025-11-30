#include "../ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel * level) {
        if (!EditLevelLayer::init(level)) return false;
        log::info("hook edit level layer");

        if (auto name = getChildByID("level-name-input")) {
            log::debug("node {} found", name->getID());
            if (auto menu = name->getChildByID("menu"_spr)) menu->setPositionX(menu->getPositionX() + 25.f);
        };

        if (auto desc = getChildByID("description-input")) {
            log::debug("node {} found", desc->getID());

            if (auto menu = static_cast<ClipboardMenu*>(desc->getChildByID("menu"_spr))) {
                menu->setPositionX(menu->getPositionX() - 8.75f);
                menu->setButtonScale(menu->getButtonScale() * 2.5f);
            };
        };

        return true;
    };
};