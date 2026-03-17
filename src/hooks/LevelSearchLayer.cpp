#include "../ClipboardMenu.hpp"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookLevelSearchLayer, LevelSearchLayer) {
    FTDIN_HOOK_ALL(inputnodefix::layer::level_search_layer);

    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;

        if (auto search = getChildByID("search-bar")) {
            log::trace("node {} found", search->getID());

            if (auto menu = typeinfo_cast<ClipboardMenu*>(search->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.5f);
        };

        return true;
    };
};