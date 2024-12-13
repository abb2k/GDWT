#include "../utils/MDAlertLayer.hpp"

MDAlertLayer* MDAlertLayer::create(const std::string& title, const std::string& description, const std::string& btn1Name, const std::function<void(bool)>& callback, const std::string& btn2Name) {
    auto ret = new MDAlertLayer();
    if (ret && ret->initAnchored(390, 280, title, description, btn1Name, callback, btn2Name, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MDAlertLayer::setup(const std::string& title, const std::string& description, const std::string& btn1Name, const std::function<void(bool)>& callback, const std::string& btn2Name){
    this->setTitle(title);

    auto textArea = MDTextArea::create(description, {m_size.width / 1.3f, m_size.height / 1.5f});
    textArea->setPosition({m_size.width / 2, m_size.height / 2 + 10});
    m_mainLayer->addChild(textArea);

    auto btn1Sprite = ButtonSprite::create(btn1Name.c_str());
    btn1 = CCMenuItemSpriteExtra::create(
        btn1Sprite,
        this,
        menu_selector(MDAlertLayer::OnBtnClicked)
    );
    btn1->setPosition({m_size.width / 2, 30});
    m_buttonMenu->addChild(btn1);

    this->callback = callback;

    if (btn2Name == "NULL") return true;

    auto btn2Sprite = ButtonSprite::create(btn2Name.c_str());
    btn2 = CCMenuItemSpriteExtra::create(
        btn2Sprite,
        this,
        menu_selector(MDAlertLayer::OnBtnClicked)
    );
    btn2->setPosition({m_size.width / 2 + btn2->getContentWidth(), 30});
    btn1->setPositionX(btn1->getPositionX() - btn1->getContentWidth());
    m_buttonMenu->addChild(btn2);

    return true;
}

void MDAlertLayer::show(){
    auto scene = CCScene::get();

    this->setZOrder(scene->getChildrenCount() > 0 ? scene->getHighestChildZ() + 100 : 100);
    scene->addChild(this);
}

void MDAlertLayer::OnBtnClicked(CCObject* sender){
    if (btn1 == sender){
        MDAlertLayer::sendResult(true);
        return;
    }

    if (btn2 == nullptr) return;

    MDAlertLayer::sendResult(false);
}

void MDAlertLayer::sendResult(bool res){
    if (callback != NULL)
        callback(res);

    this->onClose(nullptr);
}