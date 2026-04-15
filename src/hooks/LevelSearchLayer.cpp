#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookLevelSearchLayer, LevelSearchLayer) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::level_search_layer);

    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_searchInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.525f);

        return true;
    };
};