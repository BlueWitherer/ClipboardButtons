#include "ClipboardMenu.h"

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Button.hpp>

#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

// it's modding time >:3
static auto ezcb = Mod::get();

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    struct Fields {
        ClipboardMenu* clipboardMenu = nullptr;

        bool noEditor = ezcb->getSettingValue<bool>("disable-editor");
        bool always = ezcb->getSettingValue<bool>("btn-always");
    };

    bool init(float width, float height, const char* placeholder, const char* textFont, int fontSize, const char* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        auto f = m_fields.self();

        log::trace("hooked text node \"{}\"", getID());

        if (LevelEditorLayer::get() && f->noEditor) return true;

        f->clipboardMenu = ClipboardMenu::create(this);
        f->clipboardMenu->setVisible(showMenu());

        addChild(f->clipboardMenu, m_textField->getZOrder() + 9);
        f->clipboardMenu->updateLayout();

        return true;
    };

    void setTouchEnabled(bool value) {
        CCTextInputNode::setTouchEnabled(value);

        auto f = m_fields.self();
        if (f->clipboardMenu) f->clipboardMenu->setVisible(value && f->always);
    };

    bool onTextFieldAttachWithIME(CCTextFieldTTF* tField) {
        auto f = m_fields.self();
        if (f->clipboardMenu) f->clipboardMenu->setVisible(isTouchEnabled());

        return CCTextInputNode::onTextFieldAttachWithIME(tField);
    };

    bool onTextFieldDetachWithIME(CCTextFieldTTF* tField) {
        auto f = m_fields.self();
        if (f->clipboardMenu) f->clipboardMenu->setVisible(showMenu());

        return CCTextInputNode::onTextFieldDetachWithIME(tField);
    };

    bool showMenu() {
        return isTouchEnabled() && m_fields->always;
    };
};

class $modify(ClipboardEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) return false;

        if (auto guidelinesMenu = getChildByID("guidelines-menu")) {
            auto btn = Button::createWithNode(
                CircleButtonSprite::createWithSpriteFrameName(
                    "icon.png"_spr,
                    0.825f),
                [](auto) {
                    openSettingsPopup(ezcb);
                });
            btn->setID("clipboard-settings-btn"_spr);
            btn->setScale(0.875f);

            guidelinesMenu->addChild(btn);
            guidelinesMenu->updateLayout();
        };

        return true;
    };
};