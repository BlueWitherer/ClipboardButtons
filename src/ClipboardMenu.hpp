#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ClipboardMenu : public CCMenu {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    ClipboardMenu();
    virtual ~ClipboardMenu();

    void copyText(CCObject*);
    void pasteText(CCObject*);

    bool init(CCTextInputNode* textInput);

public:
    static ClipboardMenu* create(CCTextInputNode* textInput);
};