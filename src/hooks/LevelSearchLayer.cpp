#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (!LevelSearchLayer::init(type)) return false;
        log::info("hooked search layer");

        if (auto search = getChildByID("search-bar")) {
            if (auto searchNode = search->getChildByType<CCTextInputNode*>(1)) {
                if (auto searchNodeMenu = searchNode->getChildByID("menu"_spr)) searchNodeMenu->setPositionX(searchNode->getScaledContentWidth());
            };
        };

        return true;
    };
};