#include "../layers/GDWTTeamLayer.hpp"
#include "../layers/flagDisplay.hpp"
#include "../layers/UserDisplay.hpp"

GDWTTeamLayer* GDWTTeamLayer::create(Team _team) {
    auto ret = new GDWTTeamLayer();
    if (ret && ret->init(263, 210, _team, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTTeamLayer::setup(Team _team){

    team = _team;

    this->setID("gdwt-team-layer");

    auto alignmentNode = CCNode::create();
    alignmentNode->setPosition(m_buttonMenu->getPosition());
    alignmentNode->setID("aligment-node");
    m_mainLayer->addChild(alignmentNode);

    auto flag = flagDisplay::create(team.countryCode);
    flag->flagButton->setEnabled(false);
    flag->setPosition({-24, 74});
    flag->setScale(2.25f);
    alignmentNode->addChild(flag);

    std::string cc = team.countryCode;
    std::transform(cc.begin(), cc.end(), cc.begin(), ::toupper);
    auto CCLabel = CCLabelBMFont::create(cc.c_str(), "gjFont17.fnt");
    CCLabel->setPosition({29, 74});
    alignmentNode->addChild(CCLabel);

    playersScroll = ScrollLayer::create({148, 122});
    playersScroll->setZOrder(1);
    playersScroll->setPosition({-114.5f, -92.5f});
    playersScroll->m_contentLayer->setLayout(RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisAlignment(AxisAlignment::Even)
        ->setGrowCrossAxis(true)
        ->setGap(35)
    );
    playersScroll->m_contentLayer->setContentHeight(playersScroll->getContentHeight());
    alignmentNode->addChild(playersScroll);

    auto playersScrollBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    playersScrollBG->setColor({0, 0 ,0});
    playersScrollBG->setOpacity(75);
    playersScrollBG->setContentSize(playersScroll->getContentSize());
    playersScrollBG->setPosition(playersScroll->getPosition() + playersScroll->getContentSize() / 2);
    alignmentNode->addChild(playersScrollBG);

    //

    inactiveScroll = ScrollLayer::create({75, 122});
    inactiveScroll->setZOrder(1);
    inactiveScroll->setPosition({41.5f, -92.5f});
    inactiveScroll->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(inactiveScroll->getContentHeight())
        ->setGap(11)
    );
    alignmentNode->addChild(inactiveScroll);

    auto inactiveScrollBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    inactiveScrollBG->setColor({0, 0 ,0});
    inactiveScrollBG->setOpacity(75);
    inactiveScrollBG->setContentSize(inactiveScroll->getContentSize());
    inactiveScrollBG->setPosition(inactiveScroll->getPosition() + inactiveScroll->getContentSize() / 2);
    alignmentNode->addChild(inactiveScrollBG);

    for (int i = 0; i < team.accounts.size(); i++)
    {
        if (team.accounts[i].active){
            Host p;
            p.accountID = team.accounts[i].accountID;
            p.displayName = team.accounts[i].displayName;
            auto user = UserDisplay::create(p);
            user->setAnchorPoint({0.5f, 0});
            playersScroll->m_contentLayer->addChild(user);
        }
        else{
            Host p;
            p.accountID = team.accounts[i].accountID;
            p.displayName = team.accounts[i].displayName;
            auto user = UserDisplay::create(p);
            user->setAnchorPoint({0.5f, 0});
            inactiveScroll->m_contentLayer->addChild(user);
        }
    }

    playersScroll->m_contentLayer->updateLayout();

    if (playersScroll->m_contentLayer->getContentHeight() < playersScroll->getContentHeight()){
        playersScroll->m_contentLayer->setLayout(RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setCrossAxisOverflow(false)
            ->setGrowCrossAxis(true)
            ->setGap(35)
        );
        playersScroll->m_contentLayer->setContentHeight(playersScroll->getContentHeight());
        playersScroll->m_contentLayer->updateLayout();
    }

    playersScroll->moveToTop();
    inactiveScroll->m_contentLayer->updateLayout();
    inactiveScroll->m_contentLayer->setPositionX((inactiveScroll->getContentWidth() - inactiveScroll->m_contentLayer->getContentWidth()) / 2);
    inactiveScroll->moveToTop();

    auto playersScrollLabel = CCLabelBMFont::create("Players", "gjFont17.fnt");
    playersScrollLabel->setPosition({playersScroll->getPositionX() + playersScroll->getContentWidth() / 2, 40});
    playersScrollLabel->setScale(0.5f);
    alignmentNode->addChild(playersScrollLabel);

    auto inactiveScrollLabel = CCLabelBMFont::create("Inactive", "gjFont17.fnt");
    inactiveScrollLabel->setPosition({inactiveScroll->getPositionX() + inactiveScroll->getContentWidth() / 2, 40});
    inactiveScrollLabel->setScale(0.5f);
    alignmentNode->addChild(inactiveScrollLabel);

    return true;
};

void GDWTTeamLayer::show(){
    auto scene = CCScene::get();

    this->setZOrder(scene->getChildrenCount() > 0 ? scene->getHighestChildZ() + 100 : 100);
    scene->addChild(this);
}