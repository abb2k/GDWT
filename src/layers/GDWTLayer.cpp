#include "../layers/GDWTLayer.hpp"
#include "../layers/GDWTMatchCell.hpp"
#include "../layers/GDWTTeamCell.hpp"
#include "../layers/GDWTMatchGroupCell.hpp"
#include "../layers/joinMatchLayer.hpp"

GDWTLayer* GDWTLayer::create() {
    auto ret = new GDWTLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GDWTLayer::init(){
    auto winSize = CCDirector::get()->getWinSize();

    auto bg = CCSprite::create("bg.png"_spr);
    bg->setPosition(winSize / 2);
    bg->setScaleX(winSize.width / (569 / 1.2f));
    bg->setScaleY(winSize.height / (320 / 1.2f));
    this->addChild(bg);

    auto sideArt = CCNode::create();
    sideArt->setContentSize(winSize);
    sideArt->setAnchorPoint({0.5f, 0.5f});
    sideArt->setPosition(winSize / 2);
    sideArt->setScale(1.05f);
    addSideArt(sideArt, SideArt::All, SideArtStyle::LayerGray, false);

    CCObject* child;
    CCARRAY_FOREACH(sideArt->getChildren(), child){
        static_cast<CCSprite*>(child)->setColor({ 151, 151, 151 });
    }

    sideArt->setID("side-art");
    this->addChild(sideArt);

    //-- matches --
    
    auto MatchesCont = CCNode::create();
    MatchesCont->setPosition(winSize / 2);
    MatchesCont->setScale(0.825f);
    this->addChild(MatchesCont);

    matchesScrollLayer = ScrollLayer::create({200, 240});
    matchesScrollLayer->setPosition(-matchesScrollLayer->getContentSize() / 2);
    matchesScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(matchesScrollLayer->getContentHeight())
        ->setGap(1)
    );
    MatchesCont->addChild(matchesScrollLayer);

    auto matchesListLabel = CCLabelBMFont::create("Matches", "gjFont17.fnt");
    matchesListLabel->setPositionY(151);
    matchesListLabel->setScale(0.85f);
    MatchesCont->addChild(matchesListLabel);

    auto matchesslBG = CCLayerColor::create({25, 17, 37, 255});
    matchesslBG->setZOrder(-1);
    matchesslBG->setContentSize(matchesScrollLayer->getContentSize());
    matchesslBG->setPosition(-matchesScrollLayer->getContentSize() / 2);
    MatchesCont->addChild(matchesslBG);

    auto matcheslistLeft = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    matcheslistLeft->setPositionX(-104);
    matcheslistLeft->setScaleX(0.55f);
    matcheslistLeft->setScaleY(3.75f);
    matcheslistLeft->setZOrder(1);
    MatchesCont->addChild(matcheslistLeft);

    auto matcheslistRight = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    matcheslistRight->setPositionX(104);
    matcheslistRight->setScaleX(0.55f);
    matcheslistRight->setScaleY(3.75f);
    matcheslistRight->setRotation(180);
    matcheslistRight->setZOrder(1);
    MatchesCont->addChild(matcheslistRight);

    auto matcheslistTop = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    matcheslistTop->setScale(0.565f);
    matcheslistTop->setPositionY(128);
    matcheslistTop->setZOrder(1);
    matcheslistTop->setRotation(180);
    MatchesCont->addChild(matcheslistTop);

    auto matcheslistBottom = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    matcheslistBottom->setScale(0.565f);
    matcheslistBottom->setPositionY(-128);
    matcheslistBottom->setZOrder(1);
    MatchesCont->addChild(matcheslistBottom);

    auto matchesMenu = CCMenu::create();
    matchesMenu->setPosition({0, 0});
    MatchesCont->addChild(matchesMenu);

    auto leftArrowMatchesS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    leftArrowMatchesS->setScale(0.75f);
    leftArrowMatches = CCMenuItemSpriteExtra::create(
        leftArrowMatchesS,
        nullptr,
        this,
        menu_selector(GDWTLayer::MatchesListArrowLeft)
    );
    leftArrowMatches->setPositionX(-125);
    matchesMenu->addChild(leftArrowMatches);

    auto rightArrowMatchesS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    rightArrowMatchesS->setScale(0.75f);
    rightArrowMatches = CCMenuItemSpriteExtra::create(
        rightArrowMatchesS,
        nullptr,
        this,
        menu_selector(GDWTLayer::MatchesListArrowRight)
    );
    rightArrowMatches->setPositionX(125);
    static_cast<CCNode*>(rightArrowMatches->getChildren()->objectAtIndex(0))->setScaleX(-0.75f);
    matchesMenu->addChild(rightArrowMatches);

    refreshMatchesList(10);

    //-- teams --

    auto teamsCont = CCNode::create();
    teamsCont->setPosition({winSize.width / 6.7738f, winSize.height / 2});
    teamsCont->setScale(0.825f);
    this->addChild(teamsCont);

    teamsScrollLayer = ScrollLayer::create({125, 240});
    teamsScrollLayer->setPosition(-teamsScrollLayer->getContentSize() / 2);
    teamsScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(teamsScrollLayer->getContentHeight())
        ->setGap(1)
    );
    teamsCont->addChild(teamsScrollLayer);

    auto teamsListLabel = CCLabelBMFont::create("teams", "gjFont17.fnt");
    teamsListLabel->setPositionY(151);
    teamsListLabel->setScale(0.85f);
    teamsCont->addChild(teamsListLabel);

    auto teamsslBG = CCLayerColor::create({25, 17, 37, 255});
    teamsslBG->setZOrder(-1);
    teamsslBG->setContentSize(teamsScrollLayer->getContentSize());
    teamsslBG->setPosition(-teamsScrollLayer->getContentSize() / 2);
    teamsCont->addChild(teamsslBG);

    auto teamslistLeft = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    teamslistLeft->setPositionX(-67);
    teamslistLeft->setScaleX(0.55f);
    teamslistLeft->setScaleY(3.75f);
    teamslistLeft->setZOrder(1);
    teamsCont->addChild(teamslistLeft);

    auto teamslistRight = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    teamslistRight->setPositionX(67);
    teamslistRight->setScaleX(0.55f);
    teamslistRight->setScaleY(3.75f);
    teamslistRight->setRotation(180);
    teamslistRight->setZOrder(1);
    teamsCont->addChild(teamslistRight);

    auto teamslistTop = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    teamslistTop->setScaleX(0.373f);
    teamslistTop->setScaleY(0.45f);
    teamslistTop->setPositionY(127);
    teamslistTop->setZOrder(1);
    teamslistTop->setRotation(180);
    teamsCont->addChild(teamslistTop);

    auto teamslistBottom = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    teamslistBottom->setScaleX(0.373f);
    teamslistBottom->setScaleY(0.45f);
    teamslistBottom->setPositionY(-127);
    teamslistBottom->setZOrder(1);
    teamsCont->addChild(teamslistBottom);

    auto teamsMenu = CCMenu::create();
    teamsMenu->setPosition({0, 0});
    teamsCont->addChild(teamsMenu);

    auto leftArrowteamsS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    leftArrowteamsS->setScale(0.75f);
    leftArrowTeams = CCMenuItemSpriteExtra::create(
        leftArrowteamsS,
        nullptr,
        this,
        menu_selector(GDWTLayer::TeamsListArrowLeft)
    );
    leftArrowTeams->setPositionX(-88);
    teamsMenu->addChild(leftArrowTeams);

    auto rightArrowteamsS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    rightArrowteamsS->setScale(0.75f);
    rightArrowTeams = CCMenuItemSpriteExtra::create(
        rightArrowteamsS,
        nullptr,
        this,
        menu_selector(GDWTLayer::TeamsListArrowRight)
    );
    rightArrowTeams->setPositionX(88);
    static_cast<CCNode*>(rightArrowTeams->getChildren()->objectAtIndex(0))->setScaleX(-0.75f);
    teamsMenu->addChild(rightArrowTeams);

    refreshTeamsList(10);

    //-- macth groups --

    auto matchGroupsCont = CCNode::create();
    matchGroupsCont->setPosition({winSize.width / 1.17319f, winSize.height / 2});
    matchGroupsCont->setScale(0.825f);
    this->addChild(matchGroupsCont);

    matchGroupsScrollLayer = ScrollLayer::create({125, 240});
    matchGroupsScrollLayer->setPosition(-matchGroupsScrollLayer->getContentSize() / 2);
    matchGroupsScrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(matchGroupsScrollLayer->getContentHeight())
        ->setGap(1)
    );
    matchGroupsCont->addChild(matchGroupsScrollLayer);

    auto matchGroupsListLabel = CCLabelBMFont::create("matchGroups", "gjFont17.fnt");
    matchGroupsListLabel->setPositionY(151);
    matchGroupsListLabel->setScale(0.85f);
    matchGroupsCont->addChild(matchGroupsListLabel);

    auto matchGroupslBG = CCLayerColor::create({25, 17, 37, 255});
    matchGroupslBG->setZOrder(-1);
    matchGroupslBG->setContentSize(matchGroupsScrollLayer->getContentSize());
    matchGroupslBG->setPosition(-matchGroupsScrollLayer->getContentSize() / 2);
    matchGroupsCont->addChild(matchGroupslBG);

    auto matchGroupslistLeft = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    matchGroupslistLeft->setPositionX(-67);
    matchGroupslistLeft->setScaleX(0.55f);
    matchGroupslistLeft->setScaleY(3.75f);
    matchGroupslistLeft->setZOrder(1);
    matchGroupsCont->addChild(matchGroupslistLeft);

    auto matchGroupslistRight = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    matchGroupslistRight->setPositionX(67);
    matchGroupslistRight->setScaleX(0.55f);
    matchGroupslistRight->setScaleY(3.75f);
    matchGroupslistRight->setRotation(180);
    matchGroupslistRight->setZOrder(1);
    matchGroupsCont->addChild(matchGroupslistRight);

    auto matchGroupslistTop = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    matchGroupslistTop->setScaleX(0.373f);
    matchGroupslistTop->setScaleY(0.45f);
    matchGroupslistTop->setPositionY(127);
    matchGroupslistTop->setZOrder(1);
    matchGroupslistTop->setRotation(180);
    matchGroupsCont->addChild(matchGroupslistTop);

    auto matchGroupslistBottom = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    matchGroupslistBottom->setScaleX(0.373f);
    matchGroupslistBottom->setScaleY(0.45f);
    matchGroupslistBottom->setPositionY(-127);
    matchGroupslistBottom->setZOrder(1);
    matchGroupsCont->addChild(matchGroupslistBottom);

    auto matchGroupsMenu = CCMenu::create();
    matchGroupsMenu->setPosition({0, 0});
    matchGroupsCont->addChild(matchGroupsMenu);

    auto leftArrowmatchGroupsS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    leftArrowmatchGroupsS->setScale(0.75f);
    leftArrowMatchGroups = CCMenuItemSpriteExtra::create(
        leftArrowmatchGroupsS,
        nullptr,
        this,
        menu_selector(GDWTLayer::MatchGroupsListArrowLeft)
    );
    leftArrowMatchGroups->setPositionX(-88);
    matchGroupsMenu->addChild(leftArrowMatchGroups);

    auto rightArrowmatchGroupsS = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    rightArrowmatchGroupsS->setScale(0.75f);
    rightArrowMatchGroups = CCMenuItemSpriteExtra::create(
        rightArrowmatchGroupsS,
        nullptr,
        this,
        menu_selector(GDWTLayer::MatchGroupsListArrowRight)
    );
    rightArrowMatchGroups->setPositionX(88);
    static_cast<CCNode*>(rightArrowMatchGroups->getChildren()->objectAtIndex(0))->setScaleX(-0.75f);
    matchGroupsMenu->addChild(rightArrowMatchGroups);

    refreshMatchGroupsList(10);

    auto menu = CCMenu::create();
    this->addChild(menu);

    auto backButtonS = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    backButtonS->setScale(0.85f);
    auto backButton = CCMenuItemSpriteExtra::create(
        backButtonS,
        nullptr,
        this,
        menu_selector(GDWTLayer::OnBackButton)
    );
    backButton->setPosition({-winSize.width / 2.15f, winSize.height / 2.25f});
    menu->addChild(backButton);

    //

    auto joinMatchButtonS = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    joinMatchButtonS->setScale(0.35f);
    auto joinMatchButton = CCMenuItemSpriteExtra::create(
        joinMatchButtonS,
        nullptr,
        this,
        menu_selector(GDWTLayer::openJoinMatchMenu)
    );
    joinMatchButton->setPosition({-winSize.width / -2.15f, winSize.height / -2.25f});
    menu->addChild(joinMatchButton);
    
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
	this->setKeypadEnabled(true);

    scheduleUpdate();

    return true;
}

void GDWTLayer::open(){
    auto scene = CCScene::create();

	scene->addChild(this);

	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void GDWTLayer::keyBackClicked(){
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void GDWTLayer::refreshMatchesList(int amountPerPage){
    matchListener.bind([amountPerPage, this] (MatchesTask::Event* event){
        if (auto matchesP = event->getValue()){
            auto matches = *matchesP;

            //get page thing

            int beginIndex = MatchesListPage * amountPerPage;
            int endIndex = MatchesListPage * amountPerPage + amountPerPage;

            if (matches.size() <= endIndex){
                endIndex = matches.size();
                rightArrowMatches->setVisible(false);
            }
            else
                rightArrowMatches->setVisible(true);

            if (MatchesListPage == 0)
                leftArrowMatches->setVisible(false);
            else
                leftArrowMatches->setVisible(true);
            
            std::vector<Match> pageMatches;

            try{
                pageMatches = std::vector<Match>(std::next(matches.begin(), beginIndex), std::next(matches.begin(), endIndex));
            }
            catch (...){
                MatchesListPage = matches.size() / amountPerPage;
                beginIndex = MatchesListPage * amountPerPage;
                endIndex = MatchesListPage * amountPerPage + amountPerPage;
                if (matches.size() <= endIndex)
                    endIndex = matches.size();

                pageMatches = std::vector<Match>(std::next(matches.begin(), beginIndex), std::next(matches.begin(), endIndex));
            }

            matchesScrollLayer->m_contentLayer->removeAllChildrenWithCleanup(true);

            for (int i = 0; i < pageMatches.size(); i++)
            {   
                GDWTMatchCell* matchCell;
                if ((i % 2) == 0)
                    matchCell = GDWTMatchCell::create(pageMatches[i], {matchesScrollLayer->getContentSize().width, 60});
                else
                    matchCell = GDWTMatchCell::create(pageMatches[i], {matchesScrollLayer->getContentSize().width, 60}, true);

                matchesScrollLayer->m_contentLayer->addChild(matchCell);
            }

            matchesScrollLayer->m_contentLayer->updateLayout();
            matchesScrollLayer->moveToTop();
            
        }
    });

    matchListener.setFilter(data::getMatchesData());
}

void GDWTLayer::MatchesListArrowLeft(CCObject*){
    if (MatchesListPage > 0){
        MatchesListPage--;
        refreshMatchesList(10);
    }
    
}

void GDWTLayer::MatchesListArrowRight(CCObject*){
    MatchesListPage++;
    refreshMatchesList(10);
}

//

void GDWTLayer::refreshTeamsList(int amountPerPage){
    teamListener.bind([amountPerPage, this] (TeamsTask::Event* event){
        if (auto teamsP = event->getValue()){
            auto teams = *teamsP;

            teamsScrollLayer->m_contentLayer->removeAllChildrenWithCleanup(true);
            //get page thing

            int beginIndex = TeamsListPage * amountPerPage;
            int endIndex = TeamsListPage * amountPerPage + amountPerPage;

            if (teams.size() <= endIndex){
                endIndex = teams.size();
                rightArrowTeams->setVisible(false);
            }
            else
                rightArrowTeams->setVisible(true);

            if (TeamsListPage == 0)
                leftArrowTeams->setVisible(false);
            else
                leftArrowTeams->setVisible(true);
            
            std::vector<Team> pageTeams(std::next(teams.begin(), beginIndex), std::next(teams.begin(), endIndex));

            //
            
            for (int i = 0; i < pageTeams.size(); i++)
            {   
                GDWTTeamCell* teamCell;
                if ((i % 2) == 0)
                    teamCell = GDWTTeamCell::create(pageTeams[i], {teamsScrollLayer->getContentSize().width, 60});
                else
                    teamCell = GDWTTeamCell::create(pageTeams[i], {teamsScrollLayer->getContentSize().width, 60}, true);

                teamsScrollLayer->m_contentLayer->addChild(teamCell);
            }

            teamsScrollLayer->m_contentLayer->updateLayout();
            teamsScrollLayer->moveToTop();
        }
    });

    teamListener.setFilter(data::getTeamsData());
}

void GDWTLayer::TeamsListArrowLeft(CCObject*){
    TeamsListPage--;
    refreshTeamsList(10);
}

void GDWTLayer::TeamsListArrowRight(CCObject*){
    TeamsListPage++;
    refreshTeamsList(10);
}

//

void GDWTLayer::refreshMatchGroupsList(int amountPerPage){
    MGListener.bind([amountPerPage, this] (MatchGroupsDataTask::Event* event){
        if (auto MGsP = event->getValue()){
            auto MatchGroups = *MGsP;

            matchGroupsScrollLayer->m_contentLayer->removeAllChildrenWithCleanup(true);
            //get page thing

            int beginIndex = MatchGroupsListPage * amountPerPage;
            int endIndex = MatchGroupsListPage * amountPerPage + amountPerPage;

            if (MatchGroups.size() <= endIndex){
                endIndex = MatchGroups.size();
                rightArrowMatchGroups->setVisible(false);
            }
            else
                rightArrowMatchGroups->setVisible(true);

            if (MatchGroupsListPage == 0)
                leftArrowMatchGroups->setVisible(false);
            else
                leftArrowMatchGroups->setVisible(true);
            
            std::vector<MatchGroup> pageMatchGroups(std::next(MatchGroups.begin(), beginIndex), std::next(MatchGroups.begin(), endIndex));

            //
            
            for (int i = 0; i < pageMatchGroups.size(); i++)
            {   
                GDWTMatchGroupCell* mgCell;
                if ((i % 2) == 0)
                    mgCell = GDWTMatchGroupCell::create(pageMatchGroups[i], {matchGroupsScrollLayer->getContentSize().width, 60});
                else
                    mgCell = GDWTMatchGroupCell::create(pageMatchGroups[i], {matchGroupsScrollLayer->getContentSize().width, 60}, true);

                matchGroupsScrollLayer->m_contentLayer->addChild(mgCell);
            }

            matchGroupsScrollLayer->m_contentLayer->updateLayout();
            matchGroupsScrollLayer->moveToTop();
        }
    });

    MGListener.setFilter(data::getMatchGroupsData());
}

void GDWTLayer::MatchGroupsListArrowLeft(CCObject*){
    MatchGroupsListPage--;
    refreshMatchGroupsList(10);
}

void GDWTLayer::MatchGroupsListArrowRight(CCObject*){
    MatchGroupsListPage++;
    refreshMatchGroupsList(10);
}

void GDWTLayer::update(float delta){
    auto scene = CCDirector::get()->getRunningScene();
    if (scene->getChildByID("gdwt-match-layer") || scene->getChildByID("gdwt-team-layer") || scene->getChildByID("gdwt-match-group-layer")) return;

    CCRect LevelsListRect = {matchesScrollLayer->getPositionX(), matchesScrollLayer->getPositionY(), matchesScrollLayer->getScaledContentSize().width, matchesScrollLayer->getScaledContentSize().height};

    CCRect LinkedLevelsListRect = {teamsScrollLayer->getPositionX(), teamsScrollLayer->getPositionY(), teamsScrollLayer->getScaledContentSize().width, teamsScrollLayer->getScaledContentSize().height};

    CCRect matchGroupsScrollLayerRect = {matchGroupsScrollLayer->getPositionX(), matchGroupsScrollLayer->getPositionY(), matchGroupsScrollLayer->getScaledContentSize().width, matchGroupsScrollLayer->getScaledContentSize().height};

    CCNode* pmatchesScrollLayer = matchesScrollLayer->getParent();

    CCNode* pteamsScrollLayer = teamsScrollLayer->getParent();

    CCNode* pmatchGroupsScrollLayer = matchGroupsScrollLayer->getParent();

    if (LevelsListRect.containsPoint(pmatchesScrollLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 1){
        scrollSwitch = 1;

        matchesScrollLayer->retain();
        matchesScrollLayer->removeFromParent();
        pmatchesScrollLayer->addChild(matchesScrollLayer);
        matchesScrollLayer->release();
    }

    if (LinkedLevelsListRect.containsPoint(pteamsScrollLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 2){
        scrollSwitch = 2;

        teamsScrollLayer->retain();
        teamsScrollLayer->removeFromParent();
        pteamsScrollLayer->addChild(teamsScrollLayer);
        teamsScrollLayer->release();
    }

    if (matchGroupsScrollLayerRect.containsPoint(pmatchGroupsScrollLayer->convertToNodeSpace(getMousePos())) && scrollSwitch != 0){
        scrollSwitch = 0;

        matchGroupsScrollLayer->retain();
        matchGroupsScrollLayer->removeFromParent();
        pmatchGroupsScrollLayer->addChild(matchGroupsScrollLayer);
        matchGroupsScrollLayer->release();
    }
}

void GDWTLayer::OnBackButton(CCObject*){
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void GDWTLayer::openJoinMatchMenu(CCObject*){
    joinMatchLayer::create()->show();
}
