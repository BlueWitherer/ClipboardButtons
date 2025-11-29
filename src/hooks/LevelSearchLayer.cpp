#include <Geode/Geode.hpp>

#include <Geode/modify/LevelSearchLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardLevelSearchLayer, LevelSearchLayer) {
    bool init(int type) {
        if (LevelSearchLayer::init(type)) {
            log::info("hooked search layer");
            return true;
        } else {
            return false;
        };
    };
};