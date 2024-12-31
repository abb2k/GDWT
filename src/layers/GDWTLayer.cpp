#include "../layers/GDWTLayer.hpp"
#include "../layers/GeneralGDWTCell.hpp"
#include "../layers/JoinMatchLayer.hpp"

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
    geode::addSideArt(sideArt, SideArt::All, SideArtStyle::LayerGray, false);

    for (auto& child : CCArrayExt<CCSprite*>(sideArt->getChildren()))
    {
        child->setColor({ 151, 151, 151 });
    }

    sideArt->setID("side-art");
    this->addChild(sideArt);

    mainScroll = ScrollLayer::create(CCSize(winSize.width / 1.75f, winSize.height / 1.5f));
    mainScroll->m_contentLayer->setLayout(RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setAutoScale(false)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(20)
        ->setCrossAxisOverflow(false)
    );
    mainScroll->setPosition(winSize / 2 - mainScroll->getContentSize() / 2);
    this->addChild(mainScroll);

    refreshLoadingCircle = LoadingCircle::create();
    refreshLoadingCircle->setParentLayer(this);
    refreshLoadingCircle->show();
    refreshLoadingCircle->setVisible(false);
    
    data::getMatchesData().listen([this] (Result<std::vector<Match>>* res){
        if (res == nullptr) return;
        if (res->isErr()) return;
        if (!res->unwrap().size()) return;

        for (int i = 0; i < res->unwrap().size(); i++)
        {
            auto cell = GeneralGDWTCell::create("2024 GD World Tournament Playoffs");
            cell->playEnterTransition(.5f, i + 1);
            cell->setCentralContent(res->unwrap()[i]);
            Tag tag1;
            tag1.color = { 72, 255, 0 };
            tag1.name = "GDWT";
            cell->addTag(tag1);
            Tag tag3;
            Tag tag2;
            tag2.color = { 255, 0, 0 };
            tag2.name = "2024\n";
            cell->addTag(tag2);
            tag3.color = { 255, 0, 0 };
            tag3.name = "silly\n";
            cell->addTag(tag3);
            mainScroll->m_contentLayer->addChild(cell);
        }

        updateMainScrollSize();
    });

    //
    auto menu = CCMenu::create();
    this->addChild(menu);

    auto joinMatchButtonS = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    joinMatchButtonS->setScale(0.35f);
    auto joinMatchButton = CCMenuItemSpriteExtra::create(
        joinMatchButtonS,
        nullptr,
        this,
        menu_selector(GDWTLayer::openJoinMatchMenu)
    );
    joinMatchButton->setPosition({0, -137.5f});
    menu->addChild(joinMatchButton);

    // side menu

    auto sideMenuBG = CCScale9Sprite::create("square02_small.png");
    sideMenuBG->setContentSize({80, 200});
    sideMenuBG->setPosition(ccp(484, winSize.height / 2));
    sideMenuBG->setID("right-menu");
    sideMenuBG->setOpacity(200);
    this->addChild(sideMenuBG);

    sideMenu = CCMenu::create();
    sideMenu->setPosition(sideMenuBG->getContentSize() / 2);
    sideMenu->setAnchorPoint(ccp(.5f, .5f));
    sideMenu->setContentSize(sideMenuBG->getContentSize());
    sideMenuBG->addChild(sideMenu);

    myCursor = GDWTSelectionCursor::create();
    this->addChild(myCursor);
    myCursor->setPositionX(sideMenuBG->getPositionX());
    myCursor->setScale(.5f);
    myCursor->setAnimationSpeed(2);
    myCursor->setIdleMoveDistance(2);
    myCursor->setWrapOffset(1);
    myCursor->setCallback(std::bind(&GDWTLayer::onOptionSwitched, this, std::placeholders::_1));

    matchesBtn = GDWTLayer::addSideMenuButton("Match");
    matchesBtn->setPositionY(80);

    teamsBtn = GDWTLayer::addSideMenuButton("Team");
    teamsBtn->setPositionY(55);

    matchGroupsBtn = GDWTLayer::addSideMenuButton("Match\nGroup");
    matchGroupsBtn->setPositionY(20);

    playerBtn = GDWTLayer::addSideMenuButton("Player");
    playerBtn->setPositionY(-80);

    myCursor->realign();

    //

    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
	this->setKeypadEnabled(true);

    this->scheduleUpdate();

    return true;
}

void GDWTLayer::updateMainScrollSize(){
    mainScroll->m_contentLayer->updateLayout();

    int amountPerRow = 0;
    float startHight = -1;

    int overallAmount = mainScroll->m_contentLayer->getChildren()->count();
    float cellHight = 0;

    for (auto& child : CCArrayExt<CCNode*>(mainScroll->m_contentLayer->getChildren()))
    {
        if (startHight == -1){
            startHight = child->getPositionY();
            cellHight = child->getContentHeight();
        }

        if (child->getPositionY() == startHight)
            amountPerRow++;
        else break;
    }

    auto layout = static_cast<RowLayout*>(mainScroll->m_contentLayer->getLayout());

    float newContentHeight = overallAmount / amountPerRow * (cellHight + layout->getGap());

    mainScroll->m_contentLayer->setContentHeight(newContentHeight);
    mainScroll->m_contentLayer->updateLayout();
    mainScroll->moveToTop();
}

void GDWTLayer::eraseListforRefresh(){
    auto eraseNode = CCNode::create();
    eraseNode->setPosition(mainScroll->m_contentLayer->getPosition());
    eraseNode->setAnchorPoint(mainScroll->m_contentLayer->getAnchorPoint());
    eraseNode->setScale(mainScroll->m_contentLayer->getScale());
    mainScroll->addChild(eraseNode);
    refreshLoadingCircle->setVisible(true);

    CCArray* children = mainScroll->m_contentLayer->getChildren()->shallowCopy();

    for (auto& child : CCArrayExt<CCNode*>(children))
    {
        //todo: move to cleanup node thats seperate from main list

        child->retain();
        child->removeFromParent();
        eraseNode->addChild(child);
        child->release();

        if (auto cell = typeinfo_cast<GeneralGDWTCell*>(child)){
            cell->cleanRemoveWithAnimation();
            continue;
        }

        child->runAction(CCFadeOut::create(.5f));
    }
}

void GDWTLayer::SideButtonClicked(CCObject* sender){
    myCursor->moveOptionTo(static_cast<CCNode*>(sender));
}

void GDWTLayer::onOptionSwitched(CCNode* const option){
    GDWTLayer::eraseListforRefresh();

    if (option == matchesBtn){
        //matches clicked
        loadMatchPageListener.bind(this, &GDWTLayer::createMatchesPage);
        loadMatchPageListener.setFilter(GDWTLayer::loadMatchesPage());
    }
    else if (option == teamsBtn){
        //teams clicked
    }
    else if (option == matchGroupsBtn){
        //match groups clicked
    }
    else if (option == playerBtn){
        //player area clicked
    }
}

CCMenuItemSpriteExtra* GDWTLayer::addSideMenuButton(const std::string& name){
    auto s = CCLabelBMFont::create(name.c_str(), "gjFont17.fnt");
    s->setScale(.5f);
    auto toReturn = CCMenuItemSpriteExtra::create(
        s,
        nullptr,
        this,
        menu_selector(GDWTLayer::SideButtonClicked)
    );
    sideMenu->addChild(toReturn);
    myCursor->addOption(toReturn);

    return toReturn;
}

void GDWTLayer::open(){
    auto scene = CCScene::create();

	scene->addChild(this);

	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void GDWTLayer::keyBackClicked(){
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void GDWTLayer::OnBackButton(CCObject*){
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

Task<Result<>> GDWTLayer::loadMatchesPage(){
    if (matchesPage.size()) return Task<Result<>>::immediate(Ok());

    return data::getMatchesData().map(
        [this] (MatchesTask::Value* matchesRes) -> Result<> {
            if (!matchesRes) return Err("Failed getting matches!");
            if (matchesRes->isErr()) return Err("Failed getting matches! - {}", matchesRes->unwrapErr());

            auto matches = matchesRes->unwrap();

            for (const auto& match : matches)
            {
                std::ostringstream stream;
                std::copy(match.tags.begin(), match.tags.end(), std::ostream_iterator<Tag>(stream, ","));
                std::string tagString = stream.str();

                if (matchesPage.contains(tagString))
                    matchesPage[tagString].push_back(match);
                else
                    matchesPage.emplace(tagString, std::vector<Match>{match});
            }
            
            return Ok();
        },
        [](auto) -> std::monostate {
            return std::monostate();
        }
    );
}

void GDWTLayer::createMatchesPage(Task<Result<>>::Event* e){

}


void GDWTLayer::openJoinMatchMenu(CCObject*){
    joinMatchLayer::create()->show();
}
