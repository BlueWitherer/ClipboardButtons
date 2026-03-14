#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

using namespace geode::prelude;

class ClipboardMenu final : public CCNode {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    struct ClipboardButton final {
        const char* id;
        const char* spriteFrame;
        Button::ButtonCallback callback;
    };

    void reload();

protected:
    ClipboardMenu();
    ~ClipboardMenu();

    bool init(CCTextInputNode* input);

public:
    static ClipboardMenu* create(CCTextInputNode* input);

    void setButtonScale(float scale);

    float getButtonScale() const noexcept;
};