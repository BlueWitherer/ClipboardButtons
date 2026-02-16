#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = utils::clipboard;

class ClipboardMenu::Impl final {
public:
    Ref<CCTextInputNode> inputNode = nullptr;

    float scale = static_cast<float>(Mod::get()->getSettingValue<double>("btn-scale"));
    int64_t opacity = Mod::get()->getSettingValue<int64_t>("btn-opacity");

    bool space = Mod::get()->getSettingValue<bool>("btn-paste-space");
};

ClipboardMenu::ClipboardMenu() : m_impl(std::make_unique<Impl>()) {};

ClipboardMenu::~ClipboardMenu() {};

bool ClipboardMenu::init(CCTextInputNode* textInput) {
    m_impl->inputNode = textInput;

    if (!CCMenu::init()) return false;

    auto layout = ColumnLayout::create()
        ->setGap(1.25f * m_impl->scale)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Center);

    setID("menu"_spr);
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setPosition({ textInput->getScaledContentWidth() / 2.f, 0.f });
    setContentHeight(textInput->getScaledContentHeight());
    setAnchorPoint({ 1, 0.5 });
    setLayout(layout);

    reload();

    return true;
};

void ClipboardMenu::reload() {
    removeAllChildrenWithCleanup(true);

    auto copyBtnSprite = CCSprite::createWithSpriteFrameName("copy.png"_spr);
    copyBtnSprite->setScale(0.325f * m_impl->scale);
    copyBtnSprite->setOpacity(m_impl->opacity);

    auto copyBtn = CCMenuItemSpriteExtra::create(
        copyBtnSprite,
        this,
        menu_selector(ClipboardMenu::copyText)
    );
    copyBtn->setID("copy-btn");

    addChild(copyBtn);

    auto pasteBtnSprite = CCSprite::createWithSpriteFrameName("paste.png"_spr);
    pasteBtnSprite->setScale(0.325f * m_impl->scale);
    pasteBtnSprite->setOpacity(m_impl->opacity);

    auto pasteBtn = CCMenuItemSpriteExtra::create(
        pasteBtnSprite,
        this,
        menu_selector(ClipboardMenu::pasteText)
    );
    pasteBtn->setID("paste-btn");

    addChild(pasteBtn);

    updateLayout();

    scheduleOnce(schedule_selector(ClipboardMenu::rePos), 0.0125f);
};

void ClipboardMenu::rePos(float) {
    if (m_impl->inputNode) {
        if (auto parent = typeinfo_cast<TextInput*>(m_impl->inputNode->getParent())) {
            log::debug("TextInput parent found for \"{}\"", m_impl->inputNode->getID());

            auto width = parent->getScaledContentWidth();
            auto pos = m_impl->inputNode->getPositionX();

            setPosition({ (width - pos) - 1.25f, 0.f });
        } else {
            log::debug("No TextInput parent found for \"{}\"", m_impl->inputNode->getID());
        };

        setScale(getScaledContentHeight() / m_impl->inputNode->getScaledContentHeight());
    } else {
        log::error("Text input node not found");
    };
};

void ClipboardMenu::copyText(CCObject*) {
    if (m_impl->inputNode) {
        auto txt = m_impl->inputNode->getString();
        if (txt.size() > 0) cb::write(std::move(txt));
    } else {
        log::error("Text input node missing to copy text from");
    };
};

void ClipboardMenu::pasteText(CCObject*) {
    if (m_impl->inputNode) {
        auto cbTxt = cb::read();

        auto t = m_impl->space ? utils::string::trimRight(std::move(cbTxt)) : std::move(cbTxt);
        auto txt = m_impl->space ? fmt::format("{} ", std::move(t)) : std::move(t);

        auto totalSize = static_cast<int>(txt.size() + m_impl->inputNode->getString().size());
        if (totalSize > m_impl->inputNode->m_maxLabelLength) {
            auto excess = totalSize - m_impl->inputNode->m_maxLabelLength;
            if (excess < txt.size()) {
                txt.erase(excess);
            } else {
                txt.clear();
            };

            txt.shrink_to_fit();
        };

        if (m_impl->inputNode->isTouchEnabled() && txt.size() > 0) m_impl->inputNode->setString(fmt::format("{}{}", m_impl->inputNode->getString(), std::move(txt)));
    } else {
        log::error("text input node missing to paste text to");
    };
};

void ClipboardMenu::setButtonScale(float scale) {
    if (scale >= 10.f) scale = 10.f;
    if (scale <= 0.125f) scale = 0.125f;

    m_impl->scale = scale;

    if (auto layout = typeinfo_cast<ColumnLayout*>(getLayout())) {
        layout->setGap(1.25f * scale);
        setLayout(layout);
    };

    reload();
};

float ClipboardMenu::getButtonScale() const noexcept {
    return m_impl->scale;
};

ClipboardMenu* ClipboardMenu::create(CCTextInputNode* textInput) {
    auto ret = new ClipboardMenu();
    if (ret->init(textInput)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};