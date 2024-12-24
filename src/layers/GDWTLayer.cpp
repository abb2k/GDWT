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

    mainScroll = ScrollLayer::create(CCSize(winSize.width / 1.75f, winSize.height / 1.25f));
    mainScroll->m_contentLayer->setLayout(RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setAutoScale(false)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(20)
        ->setCrossAxisOverflow(false)
    );
    
    data::getMatchesData().listen([this] (Result<std::vector<Match>>* res){
        if (res == nullptr) return;
        if (res->isErr()) return;
        if (!res->unwrap().size()) return;

        float cellHight = 0;

        for (int i = 0; i < res->unwrap().size(); i++)
        {
            auto cell = GeneralGDWTCell::create("2024 GD World Tournament Playoffs", i + 1);
            cellHight = cell->getContentHeight();
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
        mainScroll->m_contentLayer->updateLayout();

        int amountPerRow = 0;
        float startHight = -1;
        for (auto& child : CCArrayExt<CCNode*>(mainScroll->m_contentLayer->getChildren()))
        {
            if (startHight == -1)
                startHight = child->getPositionY();

            if (child->getPositionY() == startHight)
                amountPerRow++;
            else break;
        }

        auto layout = static_cast<RowLayout*>(mainScroll->m_contentLayer->getLayout());
    
        float newContentHeight = res->unwrap().size() / amountPerRow * (cellHight + layout->getGap());

        mainScroll->m_contentLayer->setContentHeight(newContentHeight);
        mainScroll->m_contentLayer->updateLayout();
        mainScroll->moveToTop();
    });

    mainScroll->setPosition(winSize / 2 - mainScroll->getContentSize() / 2);
    this->addChild(mainScroll);



    //

    auto joinMatchButtonS = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    joinMatchButtonS->setScale(0.35f);
    auto joinMatchButton = CCMenuItemSpriteExtra::create(
        joinMatchButtonS,
        nullptr,
        this,
        menu_selector(GDWTLayer::openJoinMatchMenu)
    );
    joinMatchButton->setPosition({0, -137.5f});
    //menu->addChild(joinMatchButton);
    
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

void GDWTLayer::OnBackButton(CCObject*){
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void GDWTLayer::openJoinMatchMenu(CCObject*){
    joinMatchLayer::create()->show();
}
