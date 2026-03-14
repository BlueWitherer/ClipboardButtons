#include "../ClipboardMenu.hpp"
#include "Geode/loader/Loader.hpp"

#include <Geode/Geode.hpp>

#include <Geode/modify/SecretLayer.hpp>
#include <Geode/modify/SecretLayer2.hpp>
#include <Geode/modify/SecretLayer4.hpp>
#include <Geode/modify/SecretLayer5.hpp>

using namespace geode::prelude;

class $modify(CbHookSecretLayer, SecretLayer) {
    bool init() {
        if (!SecretLayer::init()) return false;

        queueInMainThread([this]() {
            if (auto menu = typeinfo_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.875f);
        });

        return true;
    };
};

class $modify(CbHookSecretLayer2, SecretLayer2) {
    bool init() {
        if (!SecretLayer2::init()) return false;

        queueInMainThread([this]() {
            if (auto menu = typeinfo_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.875f);
        });

        return true;
    };
};

class $modify(CbHookSecretLayer4, SecretLayer4) {
    bool init() {
        if (!SecretLayer4::init()) return false;

        queueInMainThread([this]() {
            if (auto menu = typeinfo_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.875f);
        });

        return true;
    };
};

class $modify(CbHookSecretLayer5, SecretLayer5) {
    bool init() {
        if (!SecretLayer5::init()) return false;

        queueInMainThread([this]() {
            if (auto menu = typeinfo_cast<ClipboardMenu*>(getChildByIDRecursive("menu"_spr))) menu->setButtonScale(menu->getButtonScale() * 0.875f);
        });

        return true;
    };
};