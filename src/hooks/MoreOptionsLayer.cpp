#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/MoreOptionsLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookMoreOptionsLayer, MoreOptionsLayer) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::more_options_layer);

    bool init() {
        if (!MoreOptionsLayer::init()) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_offsetInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.5f);

        return true;
    };
};