#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

using namespace geode::prelude;

class ClipboardMenu : public CCNode {
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

    void rePos(float);

    bool init(CCTextInputNode* textInput);

public:
    static ClipboardMenu* create(CCTextInputNode* textInput);

    void setButtonScale(float scale);

    float getButtonScale() const noexcept;
};