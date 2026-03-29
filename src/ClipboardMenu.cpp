#include "ClipboardMenu.h"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// ez clipboard
namespace cb = utils::clipboard;
// ez string
namespace str = utils::string;

// it's modding time >:3
static auto ezcb = Mod::get();

class ClipboardMenu::Impl final {
public:
    WeakRef<CCTextInputNode> inputNode = nullptr;

    float scale = ezcb->getSettingValue<float>("btn-scale");
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
             [this](auto) {
                 if (auto input = m_impl->inputNode.lock()) {
                     auto const txt = input->getString();
                     if (txt.size() > 0) cb::write(txt);

                     log::debug("copied text '{}'", txt);
                 } else {
                     log::error("Text input node missing to copy text from");
                 };
             }},
            {"paste-btn",
                "paste.png"_spr,
                [this](auto) {
                    if (auto input = m_impl->inputNode.lock()) {
                        auto const cbTxt = cb::read();
                        auto txt = m_impl->space ? fmt::format("{} ", str::trim(cbTxt)) : cbTxt;

                        if (auto textInput = typeinfo_cast<TextInput*>(input->getParent())) {
                            log::trace("parent is geode textinput, skipping filters...");

                            if (textInput->isRunning() && txt.size() > 0) textInput->setString(fmt::format("{}{}", textInput->getString(), txt));
                            log::debug("pasted text '{}' as '{}'", cbTxt, txt);
                        } else {
                            txt = str::filter(txt, input->m_allowedChars);

                            auto totalSize = static_cast<int>(txt.size() + input->getString().size());
                            if (totalSize > input->m_maxLabelLength) {
                                auto excess = totalSize - input->m_maxLabelLength;
                                if (excess < txt.size()) {
                                    log::trace("'{}' ({}) exceeds max input length of {}", txt, txt.size(), input->m_maxLabelLength);
                                    txt.erase(excess);
                                } else {
                                    txt.clear();
                                    log::trace("input is already full ({}/{})", input->getString().size(), input->m_maxLabelLength);
                                };

                                txt.shrink_to_fit();
                                log::trace("new pasting text is '{}' ({}) from previously '{}' ({})", txt, txt.size(), cbTxt, cbTxt.size());
                            };

                            if (input->isTouchEnabled() && txt.size() > 0) input->setString(fmt::format("{}{}", input->getString(), txt));
                            log::debug("pasted text '{}' as '{}'", cbTxt, txt);
                        };
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

    queueInMainThread([self = WeakRef(this)]() {
        if (auto s = self.lock()) {
            if (auto input = s->m_impl->inputNode.lock()) {
                auto ratio = s->getButtonScale() * (s->getScaledContentHeight() / input->getScaledContentHeight());

                if (auto layout = typeinfo_cast<ColumnLayout*>(s->getLayout())) layout->setDefaultScaleLimits(0.f, ratio);

                if (auto field = input->m_textField) {
                    log::trace("Field area found for \"{}\"", input->getID());

                    auto width = input->getScaledContentWidth();
                    s->setPosition({width - (width * field->getAnchorPoint().x), field->getPositionY()});
                } else {
                    log::error("No input field found for \"{}\"", input->getID());
                };

                s->setScale(ratio * 0.875f);
            } else {
                log::error("Text input node not found");
            };

            s->updateLayout();
            log::trace("Reloaded menu UI");
        };
    });
};

bool ClipboardMenu::init(CCTextInputNode* input) {
    m_impl->inputNode = input;

    if (!CCNode::init()) return false;

    auto layout = ColumnLayout::create()
                      ->setGap(2.5f * m_impl->scale)
                      ->setAxisReverse(true)
                      ->setAxisAlignment(AxisAlignment::Center)
                      ->setDefaultScaleLimits(0.f, getScaledContentHeight() / input->getScaledContentHeight());

    setID("menu"_spr);
    setAnchorPoint({1, 0.5});
    setPosition({input->getScaledContentWidth() / 2.f, 0.f});
    setContentSize({0.f, input->getScaledContentHeight()});
    setLayout(layout);

    reload();

    return true;
};

void ClipboardMenu::setButtonScale(float scale) {
    if (scale >= 5.f) scale = 5.f;
    if (scale <= 0.125f) scale = 0.125f;

    m_impl->scale = scale;

    if (auto layout = typeinfo_cast<ColumnLayout*>(getLayout())) {
        (void)layout->setGap(2.5f * scale);
        log::trace("Updated rescaled layout");
    };

    reload();
};

float ClipboardMenu::getButtonScale() const noexcept {
    return m_impl->scale;
};

ClipboardMenu* ClipboardMenu::create(CCTextInputNode* input) {
    auto ret = new ClipboardMenu();
    if (ret->init(input)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};