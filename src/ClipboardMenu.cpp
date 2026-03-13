#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>
#include "Geode/loader/Log.hpp"
#include "Geode/utils/ZStringView.hpp"
#include "Geode/utils/string.hpp"

using namespace geode::prelude;

// ez clipboard
namespace cb = utils::clipboard;

// it's modding time >:3
static auto ezcb = Mod::get();

class ClipboardMenu::Impl final {
public:
    WeakRef<CCTextInputNode> inputNode = nullptr;

    float scale = ezcb->getSettingValue<double>("btn-scale");
    int64_t opacity = ezcb->getSettingValue<int64_t>("btn-opacity");

    bool space = ezcb->getSettingValue<bool>("btn-paste-space");
};

ClipboardMenu::ClipboardMenu() : m_impl(std::make_unique<Impl>()) {};
ClipboardMenu::~ClipboardMenu() {};

void ClipboardMenu::reload() {
    removeAllChildrenWithCleanup(true);

    auto clipboardBtns = std::to_array<ClipboardButton>(
        {{"copy-btn",
             "copy.png"_spr,
             [this](Button*) {
                 if (auto input = m_impl->inputNode.lock()) {
                     auto const txt = input->getString();
                     if (txt.size() > 0) cb::write(txt);

                     log::trace("copied text '{}'", txt);
                 } else {
                     log::error("Text input node missing to copy text from");
                 };
             }},
            {"paste-btn",
                "paste.png"_spr,
                [this](Button*) {
                    if (auto input = m_impl->inputNode.lock()) {
                        auto const cbTxt = cb::read();

                        auto t = m_impl->space ? utils::string::trimRight(cbTxt) : cbTxt;
                        auto txt = m_impl->space ? fmt::format("{} ", t) : std::move(t);

                        txt = utils::string::filter(txt, input->m_allowedChars);

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

                        if (input->isTouchEnabled() && txt.size() > 0) input->setString(fmt::format("{}{}", input->getString(), txt));
                        log::trace("pasted text '{}'", txt);
                    } else {
                        log::error("text input node missing to paste text to");
                    };
                }}});

    for (auto& clipboardBtn : clipboardBtns) {
        auto btn = Button::createWithSpriteFrameName(
            clipboardBtn.spriteFrame,
            std::move(clipboardBtn.callback));
        btn->setID(clipboardBtn.id);
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

bool ClipboardMenu::init(CCTextInputNode* textInput) {
    m_impl->inputNode = textInput;

    if (!CCNode::init()) return false;

    auto layout = ColumnLayout::create()
                      ->setGap(1.25f * m_impl->scale)
                      ->setAutoScale(true)
                      ->setAxisReverse(true)
                      ->setAxisAlignment(AxisAlignment::Center);

    setID("menu"_spr);
    setScale(m_impl->scale);
    setPosition({textInput->getScaledContentWidth() / 2.f, 0.f});
    setContentHeight(textInput->getScaledContentHeight());
    setAnchorPoint({1, 0.5});
    setLayout(layout);

    reload();

    return true;
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