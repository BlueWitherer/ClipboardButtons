#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/SecretLayer.hpp>
#include <Geode/modify/SecretLayer2.hpp>
#include <Geode/modify/SecretLayer4.hpp>
#include <Geode/modify/SecretLayer5.hpp>

using namespace geode::prelude;

class $modify(CbHookSecretLayer, SecretLayer) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::secret_layers);

    bool init() {
        if (!SecretLayer::init()) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_searchInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);

        return true;
    };
};

class $modify(CbHookSecretLayer2, SecretLayer2) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::secret_layers);

    bool init() {
        if (!SecretLayer2::init()) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_searchInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);

        return true;
    };
};

class $modify(CbHookSecretLayer4, SecretLayer4) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::secret_layers);

    bool init() {
        if (!SecretLayer4::init()) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_searchInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);

        return true;
    };
};

class $modify(CbHookSecretLayer5, SecretLayer5) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::secret_layers);

    bool init() {
        if (!SecretLayer5::init()) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_textInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.75f);

        return true;
    };
};