#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;
        log::info("hooked level search layer");

        if (auto search = getChildByID("search-bar")) {
            log::debug("node {} found", search->getID());
            if (auto menu = search->getChildByID("menu"_spr)) menu->setPositionX(search->getScaledContentWidth() + 10.f);
        };

        return true;
    };
};