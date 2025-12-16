#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ClipboardMenu : public CCMenu {
    private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

protected:
    ClipboardMenu();
    virtual ~ClipboardMenu();

    void reload();

    void copyText(CCObject*);
    void pasteText(CCObject*);

    bool init(CCTextInputNode* textInput);

public:
    static ClipboardMenu* create(CCTextInputNode* textInput);

    void setButtonScale(float scale);

    float getButtonScale() const;
    int getButtonOpacity() const;
};