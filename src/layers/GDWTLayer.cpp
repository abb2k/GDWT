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

    auto mainScroll = ScrollLayer::create(CCSize(winSize.width / 1.75f, winSize.height / 1.25f));
    mainScroll->m_contentLayer->setLayout(RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setAutoScale(false)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(20)
        ->setCrossAxisOverflow(false)
    );

    for (int i = 0; i < 5; i++)
    {
        auto cell = GeneralGDWTCell::create("2024 GD World Tournament Playoffs", i + 1);
        mainScroll->m_contentLayer->addChild(cell);
    }
    
    mainScroll->m_contentLayer->updateLayout();
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
