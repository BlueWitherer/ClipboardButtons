#include <Geode/Geode.hpp>

#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = geode::utils::clipboard;

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    struct Fields {
        float scale = static_cast<float>(Mod::get()->getSettingValue<double>("btn-scale"));
        int64_t opacity = Mod::get()->getSettingValue<int64_t>("btn-opacity");

        Ref<CCMenu> menu = nullptr;
    };

    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;

        log::debug("hooked text node \"{}\"", getID());

        auto menuLayout = ColumnLayout::create();
        menuLayout->setGap(1.25f * (m_fields->scale * 0.5f));
        menuLayout->setAxisReverse(true);
        menuLayout->setAxisAlignment(AxisAlignment::Center);

        m_fields->menu = CCMenu::create();
        m_fields->menu->setID("menu"_spr);
        m_fields->menu->setAnchorPoint({ 1, 0.5 });
        m_fields->menu->setPosition({ getScaledContentWidth() / 2.f, 0.f });
        m_fields->menu->setContentSize({ 0.f, getScaledContentHeight() });
        m_fields->menu->setLayout(menuLayout);

        addChild(m_fields->menu);

        auto copyBtnSprite = CCSprite::createWithSpriteFrameName("copy.png"_spr);
        copyBtnSprite->setScale(0.325f * m_fields->scale);
        copyBtnSprite->setOpacity(m_fields->opacity);

        auto copyBtn = CCMenuItemSpriteExtra::create(
            copyBtnSprite,
            this,
            menu_selector(ClipboardCCTextInputNode::copyText)
        );
        copyBtn->setID("copy-btn");

        m_fields->menu->addChild(copyBtn);

        auto pasteBtnSprite = CCSprite::createWithSpriteFrameName("paste.png"_spr);
        pasteBtnSprite->setScale(0.325f * m_fields->scale);
        pasteBtnSprite->setOpacity(m_fields->opacity);

        auto pasteBtn = CCMenuItemSpriteExtra::create(
            pasteBtnSprite,
            this,
            menu_selector(ClipboardCCTextInputNode::pasteText)
        );
        pasteBtn->setID("paste-btn");

        m_fields->menu->addChild(pasteBtn);

        m_fields->menu->updateLayout(true);
        m_fields->menu->setVisible(isTouchEnabled());

        return true;
    };

    void setTouchEnabled(bool value) {
        if (m_fields->menu) m_fields->menu->setVisible(value);
        CCTextInputNode::setTouchEnabled(value);
    };

    void copyText(CCObject * sender) {
        auto txt = getString();
        if (isVisible()) cb::write(txt);
        log::info("copied text: {}", txt);
    };

    void pasteText(CCObject * sender) {
        auto txt = cb::read();
        if (isTouchEnabled()) setString(fmt::format("{}{}", getString(), txt));
        log::info("pasted text: {}", txt);
    };
};