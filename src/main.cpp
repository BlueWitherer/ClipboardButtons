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
        bool always = Mod::get()->getSettingValue<bool>("btn-always");
    };

    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        log::debug("hooked text node \"{}\"", getID());

        if (LevelEditorLayer::get() && m_fields->noEditor) return true;

        m_fields->menu = ClipboardMenu::create(this);
        m_fields->menu->setVisible(showMenu());

        addChild(m_fields->menu);

        return true;
    };

    void setTouchEnabled(bool value) {
        CCTextInputNode::setTouchEnabled(value);
        if (m_fields->menu) m_fields->menu->setVisible(value && m_fields->always);
    };

    bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * tField) {
        if (m_fields->menu) {
            m_fields->menu->setVisible(isTouchEnabled());
            alignMenu();
        };

        return CCTextInputNode::onTextFieldAttachWithIME(tField);
    };

    bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * tField) {
        if (m_fields->menu) m_fields->menu->setVisible(showMenu());
        return CCTextInputNode::onTextFieldDetachWithIME(tField);
    };

    bool showMenu() {
        return isTouchEnabled() && m_fields->always;
    };

    void alignMenu() {
        if (m_fields->menu) {
            auto anchor = getAnchorPoint();
            m_fields->menu->setPosition({ getScaledContentWidth() * (1.f - anchor.x), getScaledContentHeight() * (0.5f - anchor.y) });
        };

        // auto box = boundingBox();
        // m_fields->menu->setPosition({ box.getMaxX(), box.getMidY() });
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