#include "../ClipboardMenu.h"

#include <Geode/Geode.hpp>

#include <Geode/modify/ShareCommentLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookShareCommentLayer, ShareCommentLayer) {
    bool init(gd::string title, int charLimit, CommentType type, int ID, gd::string desc) {
        if (!ShareCommentLayer::init(std::move(title), charLimit, type, ID, std::move(desc))) return false;

        if (auto menu = typeinfo_cast<ClipboardMenu*>(m_commentInput->getChildByID("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 1.125f);

        return true;
    };
};