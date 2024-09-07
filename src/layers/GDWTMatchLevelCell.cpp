#include "../layers/GDWTMatchLevelCell.hpp"
#include "../layers/UserDisplay.hpp"
#include "../layers/flagDisplay.hpp"

GDWTMatchLevelCell* GDWTMatchLevelCell::create(Level _level, CCSize size, bool s) {
    auto ret = new GDWTMatchLevelCell();
    if (ret && ret->init(_level, size, s)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTMatchLevelCell::init(Level _level, CCSize size, bool s){

    setContentSize(size);

    ccColor4B color;
    if (!s)
        color = { 116, 165, 168 , 255};
    else
        color = { 64, 92, 94 , 255};

    bg = CCLayerColor::create(color);
    bg->setZOrder(-2);
    bg->setContentSize(getContentSize());
    this->addChild(bg);

    c = CCClippingNode::create(bg);
    c->setContentSize(getContentSize());
    this->addChild(c);

    level = _level;

    std::vector<std::tuple<UserDisplay*, std::string, int>> levelPlayers{};

    for (int i = 0; i < level.accountIDs.size(); i++)
    {
        levelPlayers.push_back(std::tuple<UserDisplay*, std::string, int>{UserDisplay::create(std::get<0>(level.accountIDs[i])), std::to_string(std::get<0>(level.accountIDs[i])), std::get<1>(level.accountIDs[i])});
    }

    for (int i = 0; i < level.displayNames.size(); i++)
    {
        Host p;
        p.accountID = -1;
        p.displayName = std::get<0>(level.displayNames[i]);

        levelPlayers.push_back(std::tuple<UserDisplay*, std::string, int>{UserDisplay::create(p), std::get<1>(level.displayNames[i]), std::get<1>(level.accountIDs[i])});
    }

    if (levelPlayers.size() == 2){
        //1v1
        
        std::get<0>(levelPlayers[0])->setPosition({42, 32});
        std::get<0>(levelPlayers[0])->setScale(1.25f);
        c->addChild(std::get<0>(levelPlayers[0]));

        flagDisplay* f1 = nullptr;
        try{
            f1 = flagDisplay::create(std::stoi(std::get<1>(levelPlayers[0])));
        }
        catch (...){
            f1 = flagDisplay::create(std::get<1>(levelPlayers[0]));
        }
        f1->setScale(1.5f);
        f1->setPosition({82, 32});
        c->addChild(f1);

        std::get<0>(levelPlayers[1])->setPosition({253, 32});
        std::get<0>(levelPlayers[1])->setScale(1.25f);
        c->addChild(std::get<0>(levelPlayers[1]));


        flagDisplay* f2 = nullptr;
        try{
            f2 = flagDisplay::create(std::stoi(std::get<1>(levelPlayers[1])));
        }
        catch (...){
            f2 = flagDisplay::create(std::get<1>(levelPlayers[1]));
        }
        f2->setScale(1.5f);
        f2->setPosition({213, 32});
        c->addChild(f2);

        int p1Score = std::get<2>(levelPlayers[0]);
        int p2Score = std::get<2>(levelPlayers[1]);

        auto p1ScoreLabel = CCLabelBMFont::create(fmt::format("score: {}", p1Score).c_str(), "goldFont.fnt");
        p1ScoreLabel->setScale(0.4f);
        p1ScoreLabel->setAnchorPoint({0, 0.5f});
        p1ScoreLabel->setPosition({41, 6});
        c->addChild(p1ScoreLabel);

        auto p2ScoreLabel = CCLabelBMFont::create(fmt::format("score: {}", p2Score).c_str(), "goldFont.fnt");
        p2ScoreLabel->setScale(0.4f);
        p2ScoreLabel->setAnchorPoint({1, 0.5f});
        p2ScoreLabel->setPosition({254, 6});
        c->addChild(p2ScoreLabel);

        auto glowP1 = CCSprite::createWithSpriteFrameName("d_gradient_c_01_001.png");
        glowP1->setZOrder(-1);
        glowP1->setRotation(90);
        glowP1->setScaleX(1.825f);
        glowP1->setScaleY(4.5f);
        glowP1->setPosition({68, size.height / 2});
        glowP1->setOpacity(95);
        c->addChild(glowP1);

        auto glowP2 = CCSprite::createWithSpriteFrameName("d_gradient_c_01_001.png");
        glowP2->setZOrder(-1);
        glowP2->setRotation(270);
        glowP2->setScaleX(1.825f);
        glowP2->setScaleY(4.5f);
        glowP2->setPosition({228, size.height / 2});
        glowP2->setOpacity(95);
        c->addChild(glowP2);

        if (p1Score == 0 && p2Score == 0){
            glowP2->setVisible(false);
            glowP1->setVisible(false);
        }
        else if (p1Score > p2Score){
            glowP1->setColor({ 0, 255, 0 });
            glowP2->setColor({ 255, 0, 0 });
        }
        else if (p1Score < p2Score){
            glowP2->setColor({ 0, 255, 0 });
            glowP1->setColor({ 255, 0, 0 });
        }
        else{
            glowP2->setColor({ 128, 128, 128 });
            glowP1->setColor({ 128, 128, 128 });
        }

        loadingCLayer = CCLayer::create();
        loadingCLayer->setPosition({0, 0});
        c->addChild(loadingCLayer);

        levelLoadingC = LoadingCircle::create();
        levelLoadingC->setParentLayer(loadingCLayer);
        levelLoadingC->setPosition({-137, -131});
        levelLoadingC->setScale(0.425f);
        levelLoadingC->show();

        levelName = InputNode::create(100, "levelName", "gjFont17.fnt");
        levelName->setEnabled(false);
        levelName->setString("Unknown Level");
        levelName->setPosition({size.width / 2, 36});
        levelName->setScale(0.85f);
        c->addChild(levelName);
    }
    else if (levelPlayers.size() > 2){
        //more

        highestScore = 0;

        auto playersAligmentMenu = CCMenu::create();
        playersAligmentMenu->setLayout(RowLayout::create()
            ->setAutoScale(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(35)
        );
        playersAligmentMenu->setAnchorPoint({0, 1});
        playersAligmentMenu->setPosition({14, 54});
        playersAligmentMenu->setContentWidth(225);
        playersAligmentMenu->setScale(0.875f);
        c->addChild(playersAligmentMenu);

        for (int i = 0; i < levelPlayers.size(); i++)
        {
            auto playerIcon = std::get<0>(levelPlayers[i]);
            playerIcon->setPosition({42, 32});
            playerIcon->setScale(1.25f);
            playersAligmentMenu->addChild(std::get<0>(levelPlayers[i]));

            auto playerScoreLabel = CCLabelBMFont::create(fmt::format("{}", std::get<2>(levelPlayers[i])).c_str(), "goldFont.fnt");
            playerScoreLabel->setScale(0.3f);
            playerScoreLabel->setPosition({playerIcon->getContentWidth() / 2, -12});
            playerIcon->addChild(playerScoreLabel);

            flagDisplay* flag = nullptr;
            try{
                flag = flagDisplay::create(std::stoi(std::get<1>(levelPlayers[i])));
            }
            catch (...){
                flag = flagDisplay::create(std::get<1>(levelPlayers[i]));
            }
            flag->setPosition({playerIcon->getContentWidth() / 2, -26});
            playerIcon->addChild(flag);

            
            auto glow = CCSprite::createWithSpriteFrameName("d_gradient_c_01_001.png");
            glow->setZOrder(-1);
            glow->setRotation(180);
            glow->setScaleX(2);
            glow->setScaleY(1.6f);
            glow->setPosition({playerIcon->getContentWidth() / 2, 10});
            glow->setOpacity(95);
            glow->setID("player-score-glow");
            playerIcon->addChild(glow);

            if (highestScore < std::get<2>(levelPlayers[i]))
                highestScore = std::get<2>(levelPlayers[i]);
        }

        playersAligmentMenu->updateLayout();

        playersAligmentMenu->setPositionX(playersAligmentMenu->getPositionX() * std::get<0>(levelPlayers[0])->getScaleX());
        
        for (int i = 0; i < levelPlayers.size(); i++)
        {
            auto currPlayerGlow = static_cast<CCSprite*>(std::get<0>(levelPlayers[i])->getChildByID("player-score-glow"));
            auto currPlayerScore = std::get<2>(levelPlayers[i]);

            scores.push_back(std::pair<std::string, int>{std::get<1>(levelPlayers[i]), std::get<2>(levelPlayers[i])});

            std::vector<CCSprite*> bestGlows{};

            if (highestScore == 0)
                currPlayerGlow->setVisible(false);
            else if (currPlayerScore < highestScore){
                currPlayerGlow->setColor({ 255, 0, 0 });
            }
            else if (currPlayerScore == highestScore){
                bestGlows.push_back(currPlayerGlow);
            }

            if (bestGlows.size() && bestGlows.size() == 1){
                bestGlows[0]->setColor({ 0, 255, 0 });
            }
            else{
                for (int g = 0; g < bestGlows.size(); g++)
                {
                    bestGlows[g]->setColor({ 128, 128, 128 });
                }
                
            }
        }

        loadingCLayer = CCLayer::create();
        loadingCLayer->setPosition({0, 0});
        c->addChild(loadingCLayer);

        levelLoadingC = LoadingCircle::create();
        levelLoadingC->setParentLayer(loadingCLayer);
        levelLoadingC->setPosition({-27, -133});
        levelLoadingC->setScale(0.425f);
        levelLoadingC->show();

        levelName = InputNode::create(100, "levelName", "gjFont17.fnt");
        levelName->setEnabled(false);
        levelName->setString("Unknown Level");
        levelName->setPosition({258, 28});
        levelName->setScale(0.7f);
        c->addChild(levelName);
    }

    auto playtime = CCLabelBMFont::create(fmt::format("{}m", level.playTime).c_str(), "gjFont17.fnt");
    playtime->setPosition({292.5f, 48});
    playtime->setScale(0.3f);
    playtime->setAnchorPoint({1, 0.5f});
    c->addChild(playtime);

    if (CCImage* image = data::getImage(std::to_string(level.levelID))){
        setImage(image);
        return true;
    }

    auto req = web::WebRequest();
    downloadListener.bind([this](web::WebTask::Event* e){
        if (auto res = e->getValue()){
            if (res->ok()){
                auto data = res->data();
                std::thread imageThread = std::thread([data,this](){
                    image = new CCImage();
                    image->autorelease();
                    image->initWithImageData(const_cast<uint8_t*>(data.data()),data.size());
                    geode::Loader::get()->queueInMainThread([this](){
                        data::addImage(image, std::to_string(level.levelID));
                        setImage(image);
                    });
                });
                imageThread.detach();
            }
        }
    });
    downloadListener.setFilter(req.get(fmt::format("https://raw.githubusercontent.com/cdc-sys/level-thumbnails/main/thumbs/{}.png", level.levelID)));

    return true;
};

void GDWTMatchLevelCell::loadLevel(GJGameLevel* _lvl){
    levelLoadingC->fadeAndRemove();
    if (loadingCLayer){
        loadingCLayer->removeMeAndCleanup();
        loadingCLayer = nullptr;
    }

    if (_lvl){
        lvl = _lvl;

        levelName->setString(lvl->m_levelName);
    }

    auto levelMenu = CCMenu::create();
    levelMenu->setPosition(levelName->getPosition());
    this->addChild(levelMenu);

    float oldScale = levelName->getInput()->m_placeholderLabel->getScale();
    levelName->getInput()->m_placeholderLabel->retain();
    levelName->getInput()->m_placeholderLabel->removeFromParent();
    levelName->getInput()->m_placeholderLabel->setScale(oldScale * levelName->getScale());
    auto b = CCMenuItemSpriteExtra::create(
        levelName->getInput()->m_placeholderLabel,
        nullptr,
        this,
        menu_selector(GDWTMatchLevelCell::onLevelClicked)
    );
    levelMenu->addChild(b);
    levelName->getInput()->m_placeholderLabel->release();
    
}

void GDWTMatchLevelCell::onLevelClicked(CCObject*){
    if (!lvl){
        geode::Notification::create("Level not found!", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
        return;
    }
    auto layer = LevelInfoLayer::create(lvl, false);
    auto scene = CCScene::create();

    scene->addChild(layer);
    auto transition = CCTransitionFade::create(0.5f, scene);

    CCDirector::sharedDirector()->pushScene(transition);
}

void GDWTMatchLevelCell::setImage(CCImage* image){
    auto winSize = CCDirector::get()->getWinSize();

    CCTexture2D* texture = new CCTexture2D();
    texture->autorelease();
    texture->initWithImage(image);
    auto sprite = CCSprite::createWithTexture(texture);
    sprite->setPosition(bg->getContentSize() / 2);
    sprite->setScale(0.65f);
    sprite->setZOrder(-2);
    sprite->setColor({185, 185, 185});
    c->addChild(sprite);
}
