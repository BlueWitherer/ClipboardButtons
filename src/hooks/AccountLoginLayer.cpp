#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/AccountLoginLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookAccountLoginLayer, AccountLoginLayer) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::account_login_layer);

    bool init(gd::string username) {
        if (!AccountLoginLayer::init(std::move(username))) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_usernameInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.675f);
        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_passwordInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.675f);

        return true;
    };
};