#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/CCTextInputNode.hpp>

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