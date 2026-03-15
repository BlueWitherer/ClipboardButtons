#include "../ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;

        if (auto name = getChildByID("level-name-input")) {
            log::trace("node {} found", name->getID());

            if (auto menu = typeinfo_cast<ClipboardMenu*>(name->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);
        };

        if (auto desc = getChildByID("description-input")) {
            log::trace("node {} found", desc->getID());

            if (auto menu = typeinfo_cast<ClipboardMenu*>(desc->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 1.375f);
        };

        return true;
    };
};