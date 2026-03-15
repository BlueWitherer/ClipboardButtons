#include "../ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;

        if (auto search = getChildByID("search-bar")) {
            log::trace("node {} found", search->getID());

            if (auto menu = typeinfo_cast<ClipboardMenu*>(search->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);
        };

        return true;
    };
};