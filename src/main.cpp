#include <Geode/Geode.hpp>

#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = geode::utils::clipboard;

class $modify(ClipboardCCTextInputNode, CCTextInputNode) {
    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        if (CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) {
            log::debug("hooked text node \"{}\"", getID());

            auto menuLayout = ColumnLayout::create();
            menuLayout->setGap(1.25f);
            menuLayout->setAxisReverse(true);
            menuLayout->setAxisAlignment(AxisAlignment::Center);

            auto menu = CCMenu::create();
            menu->setID("menu"_spr);
            menu->setAnchorPoint({ 1, 0.5 });
            menu->setPosition({ getScaledContentWidth() / 2.f, 0.f });
            menu->setContentSize({ 0.f, getScaledContentHeight() });
            menu->setLayout(menuLayout);

            addChild(menu);

            auto copyBtnSprite = CCSprite::createWithSpriteFrameName("copy.png"_spr);
            copyBtnSprite->setScale(0.325f);
            copyBtnSprite->setOpacity(75);

            auto copyBtn = CCMenuItemSpriteExtra::create(
                copyBtnSprite,
                this,
                menu_selector(ClipboardCCTextInputNode::copyText)
            );
            copyBtn->setID("copy-btn");

            menu->addChild(copyBtn);

            auto pasteBtnSprite = CCSprite::createWithSpriteFrameName("paste.png"_spr);
            pasteBtnSprite->setScale(0.325f);
            pasteBtnSprite->setOpacity(75);

            auto pasteBtn = CCMenuItemSpriteExtra::create(
                pasteBtnSprite,
                this,
                menu_selector(ClipboardCCTextInputNode::pasteText)
            );
            pasteBtn->setID("paste-btn");

            menu->addChild(pasteBtn);

            menu->updateLayout(true);

            return true;
        } else {
            return false;
        };
    };

    void copyText(CCObject * sender) {
        auto txt = getString();
        if (isKeyboardEnabled() || isKeypadEnabled()) cb::write(txt);
        log::info("copied text: {}", txt);
    };

    void pasteText(CCObject * sender) {
        auto txt = cb::read();
        if (isKeyboardEnabled() || isKeypadEnabled()) setString(fmt::format("{}{}", getString(), txt));
        log::info("pasted text: {}", txt);
    };
};