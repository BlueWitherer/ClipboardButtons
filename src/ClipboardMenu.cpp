#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = utils::clipboard;

class ClipboardMenu::Impl final {
public:
    WeakRef<CCTextInputNode> inputNode = nullptr;

    float scale = static_cast<float>(Mod::get()->getSettingValue<double>("btn-scale"));
    int64_t opacity = Mod::get()->getSettingValue<int64_t>("btn-opacity");

    bool space = Mod::get()->getSettingValue<bool>("btn-paste-space");
};

ClipboardMenu::ClipboardMenu() : m_impl(std::make_unique<Impl>()) {};

ClipboardMenu::~ClipboardMenu() {};

bool ClipboardMenu::init(CCTextInputNode* textInput) {
    m_impl->inputNode = textInput;

    if (!CCNode::init()) return false;

    auto layout = ColumnLayout::create()
                      ->setGap(1.25f * m_impl->scale)
                      ->setAxisReverse(true)
                      ->setAxisAlignment(AxisAlignment::Center);

    setID("menu"_spr);
    setPosition({textInput->getScaledContentWidth() / 2.f, 0.f});
    setContentHeight(textInput->getScaledContentHeight());
    setAnchorPoint({1, 0.5});
    setLayout(layout);

    reload();

    return true;
};

void ClipboardMenu::reload() {
    removeAllChildrenWithCleanup(true);

    auto clipboardBtns = std::to_array<ClipboardButton>(
        {{"copy-btn",
             "copy.png"_spr,
             [this](Button*) {
                 if (auto input = m_impl->inputNode.lock()) {
                     auto txt = input->getString();
                     if (txt.size() > 0) cb::write(std::move(txt));
                 } else {
                     log::error("Text input node missing to copy text from");
                 };
             }},
            {"paste-btn",
                "paste.png"_spr,
                [this](Button*) {
                    if (auto input = m_impl->inputNode.lock()) {
                        auto cbTxt = cb::read();

                        auto t = m_impl->space ? utils::string::trimRight(std::move(cbTxt)) : std::move(cbTxt);
                        auto txt = m_impl->space ? fmt::format("{} ", std::move(t)) : std::move(t);

                        auto totalSize = static_cast<int>(txt.size() + input->getString().size());
                        if (totalSize > input->m_maxLabelLength) {
                            auto excess = totalSize - input->m_maxLabelLength;
                            if (excess < txt.size()) {
                                txt.erase(excess);
                            } else {
                                txt.clear();
                            };

                            txt.shrink_to_fit();
                        };

                        if (input->isTouchEnabled() && txt.size() > 0) input->setString(fmt::format("{}{}", input->getString(), std::move(txt)));
                    } else {
                        log::error("text input node missing to paste text to");
                    };
                }}});

    for (auto& clipboardBtn : clipboardBtns) {
        auto btn = Button::createWithSpriteFrameName(
            clipboardBtn.spriteFrame,
            std::move(clipboardBtn.callback));
        btn->setID(clipboardBtn.id);
        btn->setScale(0.325f * m_impl->scale);
        btn->setOpacity(m_impl->opacity);

        addChild(btn);
    };

    updateLayout();

    if (auto input = m_impl->inputNode.lock()) {
        if (auto parent = typeinfo_cast<TextInput*>(input->getParent())) {
            log::trace("TextInput parent found for \"{}\"", input->getID());

            auto width = parent->getScaledContentWidth();
            auto pos = input->getPositionX();

            setPosition({(width - pos) - 1.25f, 0.f});
        } else {
            log::debug("No TextInput parent found for \"{}\"", input->getID());
        };

        setScale(getScaledContentHeight() / input->getScaledContentHeight());
    } else {
        log::error("Text input node not found");
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