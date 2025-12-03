#include "../ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/ShareCommentLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardShareCommentLayer, ShareCommentLayer) {
    bool init(gd::string title, int charLimit, CommentType type, int ID, gd::string desc) {
        if (!ShareCommentLayer::init(title, charLimit, type, ID, desc)) return false;

        if (auto menu = static_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) {
            menu->setPositionX(menu->getPositionX() - 6.25f);
            menu->setButtonScale(menu->getButtonScale() * 2.5f);
        };

        return true;
    };
};