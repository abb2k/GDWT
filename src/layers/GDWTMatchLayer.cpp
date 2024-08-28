#include "../layers/GDWTMatchLayer.hpp"
#include "../layers/CountyTextDisplay.hpp"
#include "../layers/UserDisplay.hpp"
#include <Geode/utils/web.hpp>

GDWTMatchLayer* GDWTMatchLayer::create(Match _match) {
    auto ret = new GDWTMatchLayer();
    if (ret && ret->init(390, 280, _match, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTMatchLayer::setup(Match _match){

    match = _match;

    this->setID("gdwt-match-layer");

    auto alignmentNode = CCNode::create();
    alignmentNode->setPosition(m_buttonMenu->getPosition());
    alignmentNode->setID("aligment-node");
    m_mainLayer->addChild(alignmentNode);

    auto nameLabel = InputNode::create(300, "Match Name", "gjFont17.fnt");
    nameLabel->setString(match.matchName.c_str());
    nameLabel->setEnabled(false);
    nameLabel->setPositionY(110);
    alignmentNode->addChild(nameLabel);

    auto cText = CountyTextDisplay::create(match.teams, {300, 40}, false);
    cText->setPositionY(84);
    alignmentNode->addChild(cText);

    //

    auto hostsCont = CCNode::create();
    hostsCont->setID("hosts-container");
    hostsCont->setPosition({-108, 15});
    alignmentNode->addChild(hostsCont);

    std::string hLabeltext = "Host";
    if (match.coHosts.size() > 1)
        hLabeltext += 's';

    auto hostsLabel = CCLabelBMFont::create(hLabeltext.c_str(), "gjFont17.fnt");
    hostsLabel->setPositionY(42);
    hostsLabel->setScale(0.5f);
    hostsCont->addChild(hostsLabel);

    hostsScrollLayer = ScrollLayer::create({75, 65});
    hostsScrollLayer->setZOrder(1);
    hostsScrollLayer->setPosition(-hostsScrollLayer->getContentSize() / 2);
    hostsScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(hostsScrollLayer->getContentHeight())
        ->setGap(11)
    );
    hostsCont->addChild(hostsScrollLayer);

    auto hostsScrollLayerBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    hostsScrollLayerBG->setColor({0, 0 ,0});
    hostsScrollLayerBG->setOpacity(75);
    hostsScrollLayerBG->setContentSize(hostsScrollLayer->getContentSize());
    hostsCont->addChild(hostsScrollLayerBG);

    if (!match.hosts.size()){
        auto na = CCLabelBMFont::create("None", "gjFont17.fnt");
        na->setZOrder(1);
        na->setScale(0.6f);
        hostsCont->addChild(na);
    }

    for (int i = 0; i < match.hosts.size(); i++)
    {
        auto h = UserDisplay::create(match.hosts[i]);
        h->setAnchorPoint({0.5f, 0});
        hostsScrollLayer->m_contentLayer->addChild(h);
    }
    hostsScrollLayer->m_contentLayer->updateLayout();

    hostsScrollLayer->m_contentLayer->setPositionX((hostsScrollLayer->getContentWidth() - hostsScrollLayer->m_contentLayer->getContentWidth()) / 2);
    hostsScrollLayer->moveToTop();

    //

    auto coHostsCont = CCNode::create();
    coHostsCont->setID("co-host-container");
    coHostsCont->setPosition({108, 15});
    alignmentNode->addChild(coHostsCont);

    std::string cohLabeltext = "co-Host";
    if (match.coHosts.size() > 1)
        cohLabeltext += 's';

    auto coHostsLabel = CCLabelBMFont::create(cohLabeltext.c_str(), "gjFont17.fnt");
    coHostsLabel->setPositionY(42);
    coHostsLabel->setScale(0.5f);
    coHostsCont->addChild(coHostsLabel);

    coHostsScrollLayer = ScrollLayer::create({75, 65});
    coHostsScrollLayer->setZOrder(1);
    coHostsScrollLayer->setPosition(-coHostsScrollLayer->getContentSize() / 2);
    coHostsScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(coHostsScrollLayer->getContentHeight())
        ->setGap(11)
    );
    coHostsCont->addChild(coHostsScrollLayer);

    auto coHostsScrollLayerBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    coHostsScrollLayerBG->setColor({0, 0 ,0});
    coHostsScrollLayerBG->setOpacity(75);
    coHostsScrollLayerBG->setContentSize(coHostsScrollLayer->getContentSize());
    coHostsCont->addChild(coHostsScrollLayerBG);

    if (!match.coHosts.size()){
        auto na = CCLabelBMFont::create("None", "gjFont17.fnt");
        na->setZOrder(1);
        na->setScale(0.6f);
        coHostsCont->addChild(na);
    }

    for (int i = 0; i < match.coHosts.size(); i++)
    {
        auto h = UserDisplay::create(match.coHosts[i]);
        h->setAnchorPoint({0.5f, 0});
        coHostsScrollLayer->m_contentLayer->addChild(h);
    }
    coHostsScrollLayer->m_contentLayer->updateLayout();

    coHostsScrollLayer->m_contentLayer->setPositionX((coHostsScrollLayer->getContentWidth() - coHostsScrollLayer->m_contentLayer->getContentWidth()) / 2);
    coHostsScrollLayer->moveToTop();

    //

    auto infoCont = CCNode::create();
    infoCont->setID("info-container");
    infoCont->setPositionY(15);
    alignmentNode->addChild(infoCont);

    auto infoLabel = CCLabelBMFont::create("Info", "gjFont17.fnt");
    infoLabel->setScale(0.5f);
    infoLabel->setPositionY(36);
    infoCont->addChild(infoLabel);

    auto infoBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    infoBG->setColor({0, 0 ,0});
    infoBG->setOpacity(75);
    infoBG->setPositionY(-5);
    infoBG->setContentSize({130, 65});
    infoCont->addChild(infoBG);

    auto infoMenu = CCMenu::create();
    infoMenu->setPosition({0, 0});
    infoCont->addChild(infoMenu);

    auto dateLabel = CCLabelBMFont::create("Date", "gjFont17.fnt");
    dateLabel->setPosition({-25, 14});
    dateLabel->setScale(0.55f);
    infoCont->addChild(dateLabel);

    auto dateText = InputNode::create(80, "date", "gjFont17.fnt");
    dateText->setPosition({-25, -9});
    dateText->setScale(0.8f);
    dateText->setString(match.date);
    dateText->setEnabled(false);
    infoCont->addChild(dateText);

    auto LiveButtonSprite = CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
    LiveButtonSprite->setScale(0.8f);
	auto LiveButton = CCMenuItemSpriteExtra::create(
		LiveButtonSprite,
		this,
		menu_selector(GDWTMatchLayer::twitchClicked)
	);
    LiveButton->setPosition({50, 0});
    infoMenu->addChild(LiveButton);

    auto VodButtonSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    VodButtonSprite->setScale(0.8f);
	auto VodButton = CCMenuItemSpriteExtra::create(
		VodButtonSprite,
		this,
		menu_selector(GDWTMatchLayer::ytClicked)
	);
    VodButton->setPosition({23, 0});
    infoMenu->addChild(VodButton);

    auto STypeLabel = CCLabelBMFont::create("Score type:", "gjFont17.fnt");
    STypeLabel->setPosition({-25, -28});
    STypeLabel->setScale(0.325f);
    infoCont->addChild(STypeLabel);

    auto STypeLabelText = InputNode::create(160, "", "gjFont17.fnt");
    STypeLabelText->setPosition({33, -28});
    STypeLabelText->setScale(0.325f);
    STypeLabelText->getBG()->setOpacity(0);
    STypeLabelText->setEnabled(false);
    STypeLabelText->setString(data::ScoreSystemTypeToString(match.scoreType));
    infoCont->addChild(STypeLabelText);

    //

    auto levelsListCont = CCNode::create();
    levelsListCont->setID("levels-list-container");
    levelsListCont->setPositionY(-42);
    alignmentNode->addChild(levelsListCont);

    auto listLeft = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    listLeft->setPosition({-155, -51});
    listLeft->setScaleY(1.2f);
    listLeft->setZOrder(1);
    levelsListCont->addChild(listLeft);

    auto listRight = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    listRight->setPosition({155, -51});
    listRight->setScaleY(1.2f);
    listRight->setRotation(180);
    listRight->setZOrder(1);
    levelsListCont->addChild(listRight);

    auto listTop = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    listTop->setScale(0.85f);
    listTop->setZOrder(1);
    listTop->setRotation(180);
    levelsListCont->addChild(listTop);

    auto listBG = CCLayerColor::create({25, 17, 37, 255});
    listBG->setZOrder(-1);
    listBG->setPosition({-148, -89});
    listBG->setContentSize({295, 76});
    levelsListCont->addChild(listBG);

    levelsListLayer = ScrollLayer::create({295, 76});
    levelsListLayer->setPosition({-148, -89});
    levelsListLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(levelsListLayer->getContentHeight())
        ->setGap(1)
    );
    levelsListCont->addChild(levelsListLayer);

    GameLevelManager::get()->m_levelManagerDelegate = this;

    for (int i = 0; i < match.levels.size(); i++)
    {
        GDWTMatchLevelCell* levelCell;
            if ((i % 2) == 0)
                levelCell = GDWTMatchLevelCell::create(match.levels[i], {295, 55});
            else
                levelCell = GDWTMatchLevelCell::create(match.levels[i], {295, 55}, true);
        
        levelsListLayer->m_contentLayer->addChild(levelCell);

        levelCells.push_back(levelCell);

        auto lvl = data::getLoadedLevelByID(match.levels[i].levelID);

        if (lvl){
            for (int i = 0; i < levelCells.size(); i++)
            {
                if (levelCells[i]->level.levelID == lvl->m_levelID){
                    levelCells[i]->loadLevel(lvl);
                }
            }
        }
        else{
            GameLevelManager::get()->getOnlineLevels(GJSearchObject::create(SearchType::Search, std::to_string(match.levels[i].levelID)));
        }
    }
    levelsListLayer->m_contentLayer->updateLayout();
    
    levelsListLayer->moveToTop();

    //

    overallScoresListener.bind([cText, this] (scoreCalcTask::Event* e) {
        if (auto _scores = e->getValue()){
            auto scores = *_scores;

            for (int i = 0; i < scores.size(); i++)
            {
                auto label = CCLabelBMFont::create(fmt::format("{}", std::get<1>(scores[i])).c_str(), "goldFont.fnt");
                label->setScale(0.5f);

                flagDisplay* flagD = nullptr;

                for (int f = 0; f < cText->flags.size(); f++)
                {
                    std::string lowerCCode = cText->flags[f]->cCode;
                    std::transform(lowerCCode.begin(), lowerCCode.end(), lowerCCode.begin(), ::tolower);

                    std::string lowerTeamCCode = std::get<0>(scores[i]);
                    std::transform(lowerTeamCCode.begin(), lowerTeamCCode.end(), lowerTeamCCode.begin(), ::tolower);

                    if (lowerCCode == lowerTeamCCode){
                        label->setPosition(cText->flags[f]->getPosition() + ccp(0, -13));
                        flagD = cText->flags[f];
                    }
                }

                if (!flagD) continue;

                bool green = false;

                if (i >= scores.size() - 1 - (match.scoreType == ScoreSystemType::WinDrawT2 || match.scoreType == ScoreSystemType::AdditionT2))
                    green = true;

                CCNode* glow;
                if (green)
                    glow = data::createCircleGlow({0, 255, 0}, 80);
                else
                    glow = data::createCircleGlow({255, 0, 0}, 120);

                glow->setZOrder(-1);
                flagD->addChild(glow);

                cText->scalingPoint->addChild(label);
            }
        }
    });

    overallScoresListener.setFilter(data::calculateScores(match.levels, match.teams, match.scoreType));

    scheduleUpdate();

    return true;
};

void GDWTMatchLayer::show(){
    auto scene = CCScene::get();

    this->setZOrder(scene->getChildrenCount() > 0 ? scene->getHighestChildZ() + 100 : 100);
    scene->addChild(this);
}

void GDWTMatchLayer::ytClicked(CCObject*){
    if (match.vodLink == "NA")
        geode::Notification::create("No vod link avalable.", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
    else
        web::openLinkInBrowser(match.vodLink);
}

void GDWTMatchLayer::twitchClicked(CCObject*){
    if (match.liveLink == "NA")
        geode::Notification::create("No live link avalable.", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
    else
        web::openLinkInBrowser(match.liveLink);
}

void GDWTMatchLayer::update(float delta){

    CCRect LevelsListRect = {hostsScrollLayer->getPositionX(), hostsScrollLayer->getPositionY(), hostsScrollLayer->getScaledContentSize().width, hostsScrollLayer->getScaledContentSize().height};

    CCRect LinkedLevelsListRect = {coHostsScrollLayer->getPositionX(), coHostsScrollLayer->getPositionY(), coHostsScrollLayer->getScaledContentSize().width, coHostsScrollLayer->getScaledContentSize().height};

    CCRect levelsListLayerRect = {levelsListLayer->getPositionX(), levelsListLayer->getPositionY(), levelsListLayer->getScaledContentSize().width, levelsListLayer->getScaledContentSize().height};

    CCNode* phostsScrollLayer = hostsScrollLayer->getParent();

    CCNode* pcoHostsScrollLayer = coHostsScrollLayer->getParent();

    CCNode* plevelsListLayer = levelsListLayer->getParent();

    if (LevelsListRect.containsPoint(phostsScrollLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 1){
        scrollSwitch = 1;

        hostsScrollLayer->retain();
        hostsScrollLayer->removeFromParent();
        phostsScrollLayer->addChild(hostsScrollLayer);
        hostsScrollLayer->release();
    }

    if (LinkedLevelsListRect.containsPoint(pcoHostsScrollLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 2){
        scrollSwitch = 2;

        coHostsScrollLayer->retain();
        coHostsScrollLayer->removeFromParent();
        pcoHostsScrollLayer->addChild(coHostsScrollLayer);
        coHostsScrollLayer->release();
    }

    if (levelsListLayerRect.containsPoint(plevelsListLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 0){
        scrollSwitch = 0;

        levelsListLayer->retain();
        levelsListLayer->removeFromParent();
        plevelsListLayer->addChild(levelsListLayer);
        levelsListLayer->release();
    }
}

void GDWTMatchLayer::loadLevelsFinished(cocos2d::CCArray* levels, char const* hash){
    if (levels->count() == 0) {
        log::info("failed to fetch level");
    }

    auto* lvl = static_cast<GJGameLevel*>(
        levels->objectAtIndex(0)
    );

    data::loadLevel(lvl);

    for (int i = 0; i < levelCells.size(); i++)
    {
        if (levelCells[i]->level.levelID == lvl->m_levelID){
            levelCells[i]->loadLevel(lvl);
        }
    }
}

void GDWTMatchLayer::loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2){
    loadLevelsFinished(p0, p1);
}

void GDWTMatchLayer::loadLevelsFailed(char const* p0){
    loadLevelsFailed(p0, -1);
}

void GDWTMatchLayer::loadLevelsFailed(char const* p0, int p1){
    for (int i = 0; i < levelCells.size(); i++)
    {
        if (levelCells[i]->level.levelID == p1){
            levelCells[i]->loadLevel(nullptr);
        }
    }
}

void GDWTMatchLayer::onClose(cocos2d::CCObject* s){
    GameLevelManager::get()->m_levelManagerDelegate = nullptr;
    Popup<Match>::onClose(s);
}
