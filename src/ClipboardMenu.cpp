#include "ClipboardMenu.hpp"

#include <Geode/Geode.hpp>
#include "Geode/loader/Loader.hpp"

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

                        auto t = m_impl->space ? str::trim(cbTxt) : cbTxt;
                        auto txt = m_impl->space ? fmt::format("{} ", t) : std::move(t);

                        txt = str::filter(txt, input->m_allowedChars);

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

    queueInMainThread([this]() {
        if (auto input = m_impl->inputNode.lock()) {
            if (auto field = input->m_textField) {
                log::trace("Field area found for \"{}\"", input->getID());

                auto anchor = field->getAnchorPoint().x;
                auto width = input->getScaledContentWidth();

                setPosition({width - (width * anchor), 0.f});
            } else {
                log::debug("No TextInput field found for \"{}\"", input->getID());
            };
        } else {
            log::error("Text input node not found");
        };

        updateLayout();
    });
};

bool ClipboardMenu::init(CCTextInputNode* input) {
    m_impl->inputNode = input;

    if (!CCNode::init()) return false;

    auto layout = ColumnLayout::create()
                      ->setGap(1.25f * m_impl->scale)
                      ->setAxisReverse(true)
                      ->setAxisAlignment(AxisAlignment::Center)
                      ->setDefaultScaleLimits(0.f, 0.325f * m_impl->scale);

    setID("menu"_spr);
    setScale(m_impl->scale);
    setAnchorPoint({1, 0.5});
    setPosition({input->getScaledContentWidth() / 2.f, 0.f});
    setContentSize({0.f, input->getScaledContentHeight()});
    setScale(getScaledContentHeight() / (input->getScaledContentHeight() * 0.875f));
    setLayout(layout);

    reload();

    return true;
};

void ClipboardMenu::setButtonScale(float scale) {
    if (scale >= 5.f) scale = 5.f;
    if (scale <= 0.125f) scale = 0.125f;

    m_impl->scale = scale;

    if (auto layout = typeinfo_cast<ColumnLayout*>(getLayout())) {
        layout->setGap(1.25f * scale);
        if (auto input = m_impl->inputNode.lock()) layout->setDefaultScaleLimits(0.f, getScaledContentHeight() / (input->getScaledContentHeight() * 0.875f));

        setLayout(layout);
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