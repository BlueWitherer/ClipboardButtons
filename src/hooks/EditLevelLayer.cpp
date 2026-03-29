#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookEditLevelLayer, EditLevelLayer) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::edit_level_layer);

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