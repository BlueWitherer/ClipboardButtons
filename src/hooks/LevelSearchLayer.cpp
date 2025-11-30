#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;
        log::info("hooked search layer");

        if (auto search = getChildByID("search-bar")) {
            log::debug("node {} found", search->getID());
            if (auto searchMenu = search->getChildByID("menu"_spr)) searchMenu->setPositionX(search->getScaledContentWidth() + 10.f);
        };

        return true;
    };
};