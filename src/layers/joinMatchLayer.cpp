#include "../layers/joinMatchLayer.hpp"
#include "../utils/data.hpp"

joinMatchLayer* joinMatchLayer::create() {
    auto ret = new joinMatchLayer();
    if (ret && ret->init(215, 125, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool joinMatchLayer::setup(){

    this->setTitle("Join Match");

    codeInput = TextInput::create(175, "Code");
    codeInput->setCommonFilter(CommonFilter::Base64Normal);
    codeInput->setPasswordMode(true);
    this->m_buttonMenu->addChild(codeInput);

    joinSprite = ButtonSprite::create("JOIN");
    joinSpriteDisabled = ButtonSprite::create("JOIN", "goldFont.fnt", "GJ_button_05.png");
    joinButton = CCMenuItemSpriteExtra::create(
        joinSprite,
        nullptr,
        this,
        menu_selector(joinMatchLayer::onJoinBtnClicked)
    );
    joinButton->setPosition({50, -35});
    joinSpriteDisabled->setVisible(false);
    joinSpriteDisabled->setPosition(joinButton->getContentSize() / 2);
    joinButton->addChild(joinSpriteDisabled);
    this->m_buttonMenu->addChild(joinButton);

    leaveSprite = ButtonSprite::create("LEAVE", "goldFont.fnt", "GJ_button_06.png");
    leaveSpriteDisabled = ButtonSprite::create("LEAVE", "goldFont.fnt", "GJ_button_05.png");
    leaveButton = CCMenuItemSpriteExtra::create(
        leaveSprite,
        nullptr,
        this,
        menu_selector(joinMatchLayer::onLeaveBtnClicked)
    );
    leaveButton->setPosition({-40, -35});
    leaveSpriteDisabled->setVisible(false);
    leaveSpriteDisabled->setPosition(leaveButton->getContentSize() / 2);
    leaveButton->addChild(leaveSpriteDisabled);
    this->m_buttonMenu->addChild(leaveButton);

    using namespace std::chrono;
    auto now = system_clock::now();
    long long a = time_point_cast<seconds>(now).time_since_epoch().count();
    a = a / 100;

    log::info("val: {} | key: {}", "8JeBdWSQQ-uW-5NDMxdK8SZZBbQU0mqsk7N7G7GgOLnueEWedNcWV___ytGwxck7SO88", std::to_string(a));

    log::info("{}", data::encrypt("8JeBdWSQQ-uW-5NDMxdK8SZZBbQU0mqsk7N7G7GgOLnueEWedNcWV___ytGwxck7SO88", std::to_string(a)).value());

    updateButtonsState();

    return true;
};

void joinMatchLayer::show(){
    auto scene = CCScene::get();

    this->setZOrder(scene->getChildrenCount() > 0 ? scene->getHighestChildZ() + 100 : 100);
    scene->addChild(this);
}

void joinMatchLayer::onJoinBtnClicked(CCObject*){
    if (!connecting)
        joinMatch();
}

void joinMatchLayer::onLeaveBtnClicked(CCObject*){
    leaveMatchAlert = FLAlertLayer::create(this, "WARNING!", "Are you sure you want to leave the current match?", "No", "Yes");
    leaveMatchAlert->show();
}


void joinMatchLayer::leaveMatch(){
    data::leaveMatch();
    FLAlertLayer::create("Success!", "You left the current match.", "OK")->show();
    updateButtonsState();
}

void joinMatchLayer::joinMatch(){
    connecting = true;
    auto res = data::joinMatch(codeInput->getString());

    if (res.isErr()){
        FLAlertLayer::create("Error!", res.err().value(), "OK")->show();
        connecting = false;
    }
    else{
        auto task = res.value();
        discordMessageListener.bind([this](Task<Result<>>::Event* e){
            if (auto* res2 = e->getValue()){
                if (res2->isOk()){
                    FLAlertLayer::create("Success!", "Joined match successfully!", "OK")->show();
                }
                else{
                    FLAlertLayer::create("Error!", res2->err().value(), "OK")->show();
                }
                updateButtonsState();
                connecting = false;
            }
        });

        discordMessageListener.setFilter(task);
    }
}

void joinMatchLayer::updateButtonsState(){
    joinButton->setEnabled(!data::getIsInMatch());
    joinSprite->setVisible(!data::getIsInMatch());
    leaveSpriteDisabled->setVisible(!data::getIsInMatch());

    leaveButton->setEnabled(data::getIsInMatch());
    leaveSprite->setVisible(data::getIsInMatch());
    joinSpriteDisabled->setVisible(data::getIsInMatch());
}

void joinMatchLayer::FLAlert_Clicked(FLAlertLayer* alert, bool confirm){
    if (alert == leaveMatchAlert && confirm){
        leaveMatch();
    }
}