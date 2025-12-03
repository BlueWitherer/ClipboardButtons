#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>

#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    struct Fields {
        Ref<ClipboardMenu> menu = nullptr;

        bool noEditor = Mod::get()->getSettingValue<bool>("disable-editor");
    };

    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        log::debug("hooked text node \"{}\"", getID());

        if (LevelEditorLayer::get() && m_fields->noEditor) return true;

        m_fields->menu = ClipboardMenu::create(this);
        addChild(m_fields->menu);

        return true;
    };

    void setTouchEnabled(bool value) {
        if (m_fields->menu) m_fields->menu->setVisible(value);
        CCTextInputNode::setTouchEnabled(value);
    };
};

class $modify(ClipboardEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer * layer) {
        if (!EditorPauseLayer::init(layer)) return false;

        if (auto guidelinesMenu = static_cast<CCMenu*>(getChildByID("guidelines-menu"))) {
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName("icon.png"_spr, 0.875f);
            btnSprite->setScale(0.875f);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(ClipboardEditorPauseLayer::onClipboardBtn)
            );
            btn->setID("settings"_spr);

            guidelinesMenu->addChild(btn);
            guidelinesMenu->updateLayout(true);
        };

        return true;
    };

    void onClipboardBtn(CCObject*) {
        openSettingsPopup(Mod::get());
    };
};