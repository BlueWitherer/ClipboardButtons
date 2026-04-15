#include "../ClipboardMenu.h"

#include <cheeseworks.fixinputnodesizes/include/API.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/GJWriteMessagePopup.hpp>

using namespace geode::prelude;

class $modify(CbHookGJWriteMessagePopup, GJWriteMessagePopup) {
    INPUTNODEFIX_HOOK_ALL(inputnodefix::layer::write_message_popup);

    bool init(int accountID, int messageID) {
        if (!GJWriteMessagePopup::init(accountID, messageID)) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_subjectInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.425f);
        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_messageInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 2.5f);

        return true;
    };
};