#include "ClipboardMenu.hpp"
#include "Geode/loader/Log.hpp"

#include <Geode/Geode.hpp>

#include <Geode/ui/GeodeUI.hpp>

#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    struct Fields {
        ClipboardMenu* clipboardMenu = nullptr;

        bool noEditor = Mod::get()->getSettingValue<bool>("disable-editor");
        bool always = Mod::get()->getSettingValue<bool>("btn-always");
    };

    bool init(float width, float height, const char* placeholder, const char* textFont, int fontSize, const char* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        auto f = m_fields.self();

        log::trace("hooked text node \"{}\"", getID());

        if (LevelEditorLayer::get() && f->noEditor) return true;

        f->clipboardMenu = ClipboardMenu::create(this);
        f->clipboardMenu->setVisible(showMenu());

        addChild(f->clipboardMenu, 9);
        f->clipboardMenu->updateLayout();

        if (m_textField) log::trace("{} | m_textField->getScaledContentWidth {}", getID(), m_textField->getScaledContentWidth());
        if (m_textArea) log::trace("{} | m_textArea->getScaledContentWidth {}", getID(), m_textArea->getScaledContentWidth());

        return true;
    };

    void setTouchEnabled(bool value) {
        CCTextInputNode::setTouchEnabled(value);
        if (m_fields->clipboardMenu) m_fields->clipboardMenu->setVisible(value && m_fields->always);
    };

    bool onTextFieldAttachWithIME(CCTextFieldTTF* tField) {
        if (m_fields->clipboardMenu) m_fields->clipboardMenu->setVisible(isTouchEnabled());
        return CCTextInputNode::onTextFieldAttachWithIME(tField);
    };

    bool onTextFieldDetachWithIME(CCTextFieldTTF* tField) {
        if (m_fields->clipboardMenu) m_fields->clipboardMenu->setVisible(showMenu());
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
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName("icon.png"_spr, 0.825f);
            btnSprite->setScale(0.875f);

            auto btn = CCMenuItemExt::createSpriteExtra(
                btnSprite,
                [](auto) {
                    openSettingsPopup(Mod::get());
                });
            btn->setID("clipboard-settings-btn"_spr);

            guidelinesMenu->addChild(btn);
            guidelinesMenu->updateLayout();
        };

        return true;
    };
};