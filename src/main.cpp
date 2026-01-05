#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>

#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    struct Fields {
        ClipboardMenu* m_clipboard = nullptr;

        bool noEditor = Mod::get()->getSettingValue<bool>("disable-editor");
        bool always = Mod::get()->getSettingValue<bool>("btn-always");
    };

    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        log::debug("hooked text node \"{}\"", getID());

        if (LevelEditorLayer::get() && m_fields->noEditor) return true;

        m_fields->m_clipboard = ClipboardMenu::create(this);
        m_fields->m_clipboard->setVisible(showMenu());

        addChild(m_fields->m_clipboard);

        return true;
    };

    void setTouchEnabled(bool value) {
        CCTextInputNode::setTouchEnabled(value);
        if (m_fields->m_clipboard) m_fields->m_clipboard->setVisible(value && m_fields->always);
    };

    bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * tField) {
        if (m_fields->m_clipboard) m_fields->m_clipboard->setVisible(isTouchEnabled());
        return CCTextInputNode::onTextFieldAttachWithIME(tField);
    };

    bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * tField) {
        if (m_fields->m_clipboard) m_fields->m_clipboard->setVisible(showMenu());
        return CCTextInputNode::onTextFieldDetachWithIME(tField);
    };

    bool showMenu() {
        return isTouchEnabled() && m_fields->always;
    };
};

class $modify(ClipboardEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer * layer) {
        if (!EditorPauseLayer::init(layer)) return false;

        if (auto guidelinesMenu = static_cast<CCMenu*>(getChildByID("guidelines-m_clipboard"))) {
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