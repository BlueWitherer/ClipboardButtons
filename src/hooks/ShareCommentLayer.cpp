#include "../ClipboardMenu.hpp"
#include "Geode/loader/Loader.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/ShareCommentLayer.hpp>

using namespace geode::prelude;

class $modify(CbHookShareCommentLayer, ShareCommentLayer) {
    bool init(gd::string title, int charLimit, CommentType type, int ID, gd::string desc) {
        if (!ShareCommentLayer::init(title, charLimit, type, ID, desc)) return false;

        queueInMainThread([this]() {
            if (auto menu = typeinfo_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 1.25f);
        });

        return true;
    };
};