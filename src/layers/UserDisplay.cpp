#include "../layers/UserDisplay.hpp"

UserDisplay* UserDisplay::create(int accountID, bool withName) {
    auto ret = new UserDisplay();
    if (ret && ret->init(accountID, withName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

UserDisplay* UserDisplay::create(Host host, bool withName) {
    auto ret = new UserDisplay();
    ret->displayName = host.displayName;
    if (ret && ret->init(host.accountID, withName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

UserDisplay* UserDisplay::create(Player player, bool withName) {
    auto ret = new UserDisplay();
    ret->displayName = player.displayName;
    if (ret && ret->init(player.accountID, withName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool UserDisplay::init(int accountID, bool withName){

    setAnchorPoint({0.5f, 0.5f});

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    this->addChild(menu);
    accID = accountID;

    player = SimplePlayer::create(2);
    player->setColor({ 148, 148, 148 });
    player->setSecondColor({ 185, 185, 185 });
    player->setScale(0.65f);
    
    auto hostButton = CCMenuItemSpriteExtra::create(
        player,
        nullptr,
        this,
        menu_selector(UserDisplay::userClicked)
    );
    hostButton->setContentSize({25, 25});
    menu->addChild(hostButton);

    loadingCLayer = CCLayer::create();
    loadingCLayer->setPosition(-loadingCLayer->getContentSize() / 2);
    loadingCLayer->setZOrder(10);
    this->addChild(loadingCLayer);

    IconLoadingC = LoadingCircle::create();
    IconLoadingC->setPosition(hostButton->getContentSize() / 2);
    IconLoadingC->setScale(0.25f);
    IconLoadingC->setParentLayer(loadingCLayer);
    IconLoadingC->show();
        
    Plistener.bind(this, &UserDisplay::onPlayerInfoReceved);

    Plistener.setFilter(data::getPlayersData());

    player->setPosition(hostButton->getContentSize() / 2);

    CCSize content = hostButton->getContentSize();
    setContentSize(content);
    
    if (withName){
        labelCont = CCNode::create();
        labelCont->setPositionY(-hostButton->getContentHeight() / 2);
        this->addChild(labelCont);

        nameLabel = CCLabelBMFont::create(displayName.c_str(), "gjFont17.fnt");
        nameLabel->setScale(0.25f);
        nameLabel->setAnchorPoint({0.5f, 1});
        labelCont->addChild(nameLabel);
        updateNameLength();
        

        //if (nameLabel->getScaledContentWidth() > 20)
            //content.width = nameLabel->getScaledContentWidth();

        //content.height += nameLabel->getScaledContentHeight() * 2;
    }

    
    
    if (withName)
        labelCont->setPosition({content.width / 2, content.height / 2 - 10});
    
    hostButton->setPosition(content / 2);


    return true;
}

void UserDisplay::userClicked(CCObject*){
    if (accID != -1)
        ProfilePage::create(accID, GJAccountManager::get()->m_accountID == accID)->show();
    else
        geode::Notification::create("Failed to fetch user info!", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
}

void UserDisplay::onPlayerInfoReceved(PlayerDataTask::Event* event){
    if (auto _players = event->getValue()){
        auto players = *_players;

        bool doesExist = false;

        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].accountID == accID){
                doesExist = true;

                player->updatePlayerFrame(players[i].iconID, IconType::Cube);
                auto col1 = GameManager::get()->colorForIdx(players[i].color1ID);
                player->setColor(col1);
                player->setSecondColor(GameManager::get()->colorForIdx(players[i].color2ID));
                player->m_outlineSprite->setVisible(players[i].glowEnabled);

                ccColor3B colcom;
                colcom = {0, 0, 0};

                if (col1 == colcom)
                    player->m_outlineSprite->setVisible(true);

                player->m_outlineSprite->setColor(GameManager::get()->colorForIdx(players[i].glowColorID));
                if (nameLabel){
                    nameLabel->setString(players[i].ingameUserName.c_str());
                    updateNameLength();
                }

                IconLoadingC->fadeAndRemove();
                loadingCLayer->removeMeAndCleanup();

                break;
            }
        }
        
        if (!doesExist){
            if (accID != -1){
                Ulistener.bind(this, &UserDisplay::onDInfoReceved);

                Ulistener.setFilter(data::getUsersInfo({accID}));
            }
            else{
                IconLoadingC->fadeAndRemove();
                loadingCLayer->removeMeAndCleanup();
            }
        }
    }
}

void UserDisplay::onDInfoReceved(UserInfoTask::Event* event) {
    if (auto _users = event->getValue()){
        auto users = *_users;
        if (users.size()){
            if (users[0]->accountID != -1){

                player->updatePlayerFrame(users[0]->accIcon, IconType::Cube);
                auto col1 = GameManager::get()->colorForIdx(users[0]->playerColor);
                player->setColor(col1);
                player->setSecondColor(GameManager::get()->colorForIdx(users[0]->playerColor2));
                player->m_outlineSprite->setVisible(users[0]->accGlow);

                ccColor3B colcom;
                colcom = {0, 0, 0};

                if (col1 == colcom)
                    player->m_outlineSprite->setVisible(true);

                player->m_outlineSprite->setColor(GameManager::get()->colorForIdx(users[0]->glowColor));

                if (displayName == "" && nameLabel){
                    nameLabel->setString(users[0]->userName.c_str());
                    updateNameLength();
                }

                Tlistener.bind([this] (TeamsTask::Event* e){
                    if (auto _teams = e->getValue()){
                        auto teams = *_teams;

                        for (int i = 0; i < teams.size(); i++)
                        {
                            bool didFind = false;
                            for (int a = 0; a < teams[i].accounts.size(); a++)
                            {
                                if (teams[i].accounts[a].displayName != "" && teams[i].accounts[a].accountID == accID){
                                    nameLabel->setString(teams[i].accounts[a].displayName.c_str());
                                    updateNameLength();
                                    didFind = true;
                                    break;
                                }
                            }
                            if (didFind)
                                break;
                        }
                        
                    }
                });

                Tlistener.setFilter(data::getTeamsData());
            }
                
        }
        else{
            IconLoadingC->fadeAndRemove();
            loadingCLayer->removeMeAndCleanup();
        }

        IconLoadingC->fadeAndRemove();
        loadingCLayer->removeMeAndCleanup();
    }
    else if (event->isCancelled()){
        IconLoadingC->fadeAndRemove();
        loadingCLayer->removeMeAndCleanup();
    }
    
}

void UserDisplay::updateNameLength(){
    if (nameLabel->getContentSize().width > 190)
        labelCont->setScale(190 / nameLabel->getContentSize().width);
}
