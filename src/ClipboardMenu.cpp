#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = geode::utils::clipboard;

class ClipboardMenu::Impl final {
    public:
    CCTextInputNode* m_textInput = nullptr;

    float m_scale = static_cast<float>(Mod::get()->getSettingValue<double>("btn-scale"));
    int64_t m_opacity = Mod::get()->getSettingValue<int64_t>("btn-opacity");
};

ClipboardMenu::ClipboardMenu() {
    m_impl = std::make_unique<Impl>();
};

ClipboardMenu::~ClipboardMenu() {};

bool ClipboardMenu::init(CCTextInputNode* textInput) {
    if (!textInput) return false;

    m_impl->m_textInput = textInput;

    if (!CCMenu::init()) return false;

    auto layout = ColumnLayout::create();
    layout->setGap(1.25f * m_impl->m_scale);
    layout->setAxisReverse(true);
    layout->setAxisAlignment(AxisAlignment::Center);

    setID("menu"_spr);
    setAnchorPoint({ 1, 0.5 });
    setPosition({ textInput->getScaledContentWidth() / 2.f, 0.f });
    setContentSize({ 0.f, textInput->getScaledContentHeight() });
    setLayout(layout);

    auto copyBtnSprite = CCSprite::createWithSpriteFrameName("copy.png"_spr);
    copyBtnSprite->setScale(0.325f * m_impl->m_scale);
    copyBtnSprite->setOpacity(m_impl->m_opacity);

    auto copyBtn = CCMenuItemSpriteExtra::create(
        copyBtnSprite,
        this,
        menu_selector(ClipboardMenu::copyText)
    );
    copyBtn->setID("copy-btn");

    addChild(copyBtn);

    auto pasteBtnSprite = CCSprite::createWithSpriteFrameName("paste.png"_spr);
    pasteBtnSprite->setScale(0.325f * m_impl->m_scale);
    pasteBtnSprite->setOpacity(m_impl->m_opacity);

    auto pasteBtn = CCMenuItemSpriteExtra::create(
        pasteBtnSprite,
        this,
        menu_selector(ClipboardMenu::pasteText)
    );
    pasteBtn->setID("paste-btn");

    addChild(pasteBtn);

    updateLayout(true);
    setVisible(textInput->isTouchEnabled());

    return true;
};

void ClipboardMenu::copyText(CCObject*) {
    if (m_impl->m_textInput) {
        auto txt = m_impl->m_textInput->getString();
        if (m_impl->m_textInput->isVisible()) cb::write(txt);
        log::info("copied text: {}", txt);
    } else {
        log::error("text input node missing to copy text from");
    };
};

void ClipboardMenu::pasteText(CCObject*) {
    if (m_impl->m_textInput) {
        auto txt = cb::read();
        if (m_impl->m_textInput->isTouchEnabled()) m_impl->m_textInput->setString(fmt::format("{}{}", m_impl->m_textInput->getString(), txt));
        log::info("pasted text: {}", txt);
    } else {
        log::error("text input node missing to paste text to");
    };
};

ClipboardMenu* ClipboardMenu::create(CCTextInputNode* textInput) {
    auto ret = new ClipboardMenu();

    if (ret && ret->init(textInput)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
}; 