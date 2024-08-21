#include "../layers/flagDisplay.hpp"
#include "../layers/GDWTTeamLayer.hpp"

flagDisplay* flagDisplay::create(int accountID) {
    auto ret = new flagDisplay();
    if (ret && ret->init(accountID, "")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

flagDisplay* flagDisplay::create(std::string countryCode) {
    auto ret = new flagDisplay();
    if (ret && ret->init(-1, countryCode)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool flagDisplay::init(int accountID, std::string countryCode){

    setAnchorPoint({0.5f, 0.5f});

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    this->addChild(menu);
    accID = accountID;

    CCSprite* flag;

    if (countryCode != ""){
        std::transform(countryCode.begin(), countryCode.end(), countryCode.begin(), ::tolower);
        flag = CCSprite::createWithSpriteFrameName(fmt::format("{}.png"_spr, countryCode).c_str());
    }
    else
        flag = CCSprite::createWithSpriteFrameName("unknown-flag.png"_spr);
    
    cCode = countryCode;
    
    flagButton = CCMenuItemSpriteExtra::create(
        flag,
        nullptr,
        this,
        menu_selector(flagDisplay::flagClicked)
    );
    menu->addChild(flagButton);

    listener.bind(this, &flagDisplay::onTeamsInfo);

    listener.setFilter(data::getTeamsData());

    loadingCLayer = CCLayer::create();
    loadingCLayer->setPosition(-loadingCLayer->getContentSize() / 2);
    this->addChild(loadingCLayer);

    IconLoadingC = LoadingCircle::create();
    IconLoadingC->setScale(0.25f);
    IconLoadingC->setParentLayer(loadingCLayer);
    IconLoadingC->show();

    return true;
}

void flagDisplay::flagClicked(CCObject*){
    GDWTTeamLayer::create(myTeam)->show();
}

void flagDisplay::onTeamsInfo(TeamsTask::Event* event){
    if (auto _teams = event->getValue()){
        auto teams = *_teams;
        for (int i = 0; i < teams.size(); i++){
            if (cCode != ""){
                std::string lowerCCode = teams[i].countryCode;
                std::transform(lowerCCode.begin(), lowerCCode.end(), lowerCCode.begin(), ::tolower);
                if (lowerCCode == cCode){
                    myTeam = teams[i];
                }
            }
            else{
                bool hasFound = false;
                for (int ac = 0; ac < teams[i].accounts.size(); ac++){
                    if (teams[i].accounts[ac].accountID == accID){
                        
                        std::string lowerCCode = teams[i].countryCode;
                        std::transform(lowerCCode.begin(), lowerCCode.end(), lowerCCode.begin(), ::tolower);

                        auto newFlag = CCSprite::createWithSpriteFrameName(fmt::format("{}.png"_spr, lowerCCode).c_str());

                        myTeam = teams[i];

                        flagButton->setSprite(newFlag);
                        hasFound = true;
                        break;
                    }
                }
                if (hasFound)
                    break;    
            }
        }

        IconLoadingC->fadeAndRemove();
        loadingCLayer->removeMeAndCleanup();
    }
    else if (event->isCancelled()){
        IconLoadingC->fadeAndRemove();
        loadingCLayer->removeMeAndCleanup();
    }
    
}


CCSize flagDisplay::getContentSize(){ return flagButton->getContentSize(); }

float flagDisplay::getContentWidth(){ return flagButton->getContentWidth(); }

float flagDisplay::getContentHeight(){ return flagButton->getContentHeight(); }

CCSize flagDisplay::getScaledContentSize(){ return flagButton->getScaledContentSize(); }

float flagDisplay::getScaledContentWidth(){ return flagButton->getScaledContentWidth(); }

float flagDisplay::getScaledContentHeight(){ return flagButton->getScaledContentHeight(); }
