#include "../layers/GDWTMatchCell.hpp"
#include <Geode/utils/web.hpp>
#include "../layers/CountyTextDisplay.hpp"
#include "../layers/GDWTMatchLayer.hpp"
#include "../layers/UserDisplay.hpp"

GDWTMatchCell* GDWTMatchCell::create(const Match& _match, CCSize size, bool s) {
    auto ret = new GDWTMatchCell();
    if (ret && ret->init(_match, size, s)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTMatchCell::init(const Match& _match, CCSize size, bool s){

    setContentSize(size);

    match = _match;

    ccColor3B color;
    if (!s)
        color = { 116, 165, 168 };
    else
        color = { 64, 92, 94 };

    auto bg = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    bg->setColor(color);
    bg->setContentSize(getContentSize());
    bg->setPosition(getContentSize() / 2);
    this->addChild(bg);

    //date

    auto dateLabel = CCLabelBMFont::create("Date:", "chatFont.fnt");
    dateLabel->setScale(0.9f);
    dateLabel->setPosition({22, 12});
    this->addChild(dateLabel);

    auto dateText = TextInput::create(size.width / 3.5f, "NA", "chatFont.fnt");
    dateText->setString(match.date.c_str());
    dateText->setScale(0.6f);
    dateText->setPosition({dateLabel->getPositionX() + dateLabel->getScaledContentWidth() / 2 + dateText->getScaledContentWidth() / 2, dateLabel->getPositionY()});
    dateText->setEnabled(false);
    dateText->getInputNode()->getPlaceholderLabel()->setOpacity(255);
    this->addChild(dateText);

    //hlabel

    auto hostLabel = CCLabelBMFont::create("Host:", "chatFont.fnt");
    hostLabel->setScale(0.9f);
    hostLabel->setPosition({90, 12});
    this->addChild(hostLabel);

    auto hostText = TextInput::create(size.width / 2.9f, "NA", "chatFont.fnt");

    std::string hosText = "";

    if (match.hosts.size() > 1){
        hosText = "...";
    }
    else{
        hosText = match.hosts[0].displayName;
    }
    

    hostText->setString(hosText.c_str());
    hostText->setScale(0.6f);
    hostText->setPosition({hostLabel->getPositionX() + hostLabel->getScaledContentWidth() / 2 + hostText->getScaledContentWidth() / 2, hostLabel->getPositionY()});
    hostText->setEnabled(false);
    hostText->getInputNode()->getPlaceholderLabel()->setOpacity(255);
    this->addChild(hostText);

    //buttons

    auto bottomLeftMenu = CCMenu::create();
    bottomLeftMenu->setPosition({0, 0});
    this->addChild(bottomLeftMenu);

    auto LiveButtonSprite = CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
    LiveButtonSprite->setScale(0.65f);
	auto LiveButton = CCMenuItemSpriteExtra::create(
		LiveButtonSprite,
		this,
		menu_selector(GDWTMatchCell::onLiveClicked)
	);
    LiveButton->setPosition({184, 12});

    bottomLeftMenu->addChild(LiveButton);

    auto hostD = UserDisplay::create(match.hosts[0], false);
    hostD->setPosition({161, 12});
    this->addChild(hostD);

    //

    auto VSbg = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    VSbg->setColor({0, 0 ,0});
    VSbg->setOpacity(75);
    VSbg->setContentSize({135 ,31});
    VSbg->setPosition({71 ,40});
    this->addChild(VSbg);

    auto vsText = CountyTextDisplay::create(match.teams, {130 ,29}, true);
    vsText->setPosition({71 ,40});
    this->addChild(vsText);

    auto moreButtonSprite = ButtonSprite::create("more");
    moreButtonSprite->setScale(0.65f);
    auto moreButton = CCMenuItemSpriteExtra::create(
        moreButtonSprite,
        nullptr,
        this,
        menu_selector(GDWTMatchCell::onMoreClicked)
    );
    moreButton->setPosition({170, 40});
    bottomLeftMenu->addChild(moreButton);

    /* old ui test

    auto hostLabel = CCLabelBMFont::create("Host:", "bigFont.fnt");
    hostLabel->setScale(0.4f);
    hostLabel->setPosition({size.width / 8, size.height / 1.2f});
    this->addChild(hostLabel);

    auto hostText = InputNode::create(size.width / 2.75f, "NA", "bigFont.fnt");
    hostText->setString(match.host.c_str());
    hostText->setScale(0.6f);
    hostText->setPosition({hostLabel->getPositionX() + hostLabel->getScaledContentWidth() / 2 + hostText->getScaledContentWidth() / 2, hostLabel->getPositionY()});
    hostText->setEnabled(false);
    this->addChild(hostText);

    auto cohostLabel = CCLabelBMFont::create("Co-Host:", "bigFont.fnt");
    cohostLabel->setScale(0.4f);
    cohostLabel->setPosition({size.width / 1.7f, size.height / 1.2f});
    this->addChild(cohostLabel);

    auto cohostText = InputNode::create(size.width / 2.5f, "NA", "bigFont.fnt");
    cohostText->setString(match.coHost.c_str());
    cohostText->setScale(0.6f);
    cohostText->setPosition({cohostLabel->getPositionX() + cohostLabel->getScaledContentWidth() / 2 + cohostText->getScaledContentWidth() / 2, cohostLabel->getPositionY()});
    cohostText->setEnabled(false);
    this->addChild(cohostText);

    std::string vsTextS = "";

    for (int i = 0; i < match.teams.size(); i++)
    {
        vsTextS += match.teams[i];
        if (i != match.teams.size() - 1)
            vsTextS += " vs ";
    }
    
    auto vsText = InputNode::create(size.width / 1.1f, "NA", "gjFont03.fnt");
    vsText->setString(vsTextS);
    vsText->setPosition(size / 2);
    vsText->setEnabled(false);
    this->addChild(vsText);

    auto dateLabel = CCLabelBMFont::create("Date:", "bigFont.fnt");
    dateLabel->setScale(0.4f);
    dateLabel->setPosition({size.width / 8.2f, size.height / 6.8f});
    this->addChild(dateLabel);

    auto dateText = InputNode::create(size.width / 3.5f, "NA", "bigFont.fnt");
    dateText->setString(match.date.c_str());
    dateText->setScale(0.6f);
    dateText->setPosition({dateLabel->getPositionX() + dateLabel->getScaledContentWidth() / 2 + dateText->getScaledContentWidth() / 2, dateLabel->getPositionY()});
    dateText->setEnabled(false);
    this->addChild(dateText);

    auto bottomLeftMenu = CCMenu::create();
    auto layout = RowLayout::create();
    layout->setAxisAlignment(AxisAlignment::End);
    bottomLeftMenu->setLayout(layout);
    bottomLeftMenu->setContentSize({size.width / 1.8f, size.height / 4});
    bottomLeftMenu->setPosition({size.width / 1.45f, size.height / 6.8f});
    this->addChild(bottomLeftMenu);

    auto LiveButtonSprite = CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
    //LiveButtonSprite->setScale(0.35f);
	auto LiveButton = CCMenuItemSpriteExtra::create(
		LiveButtonSprite,
		this,
		menu_selector(GDWTMatchCell::onLiveClicked)
	);

    auto VodButtonSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
    //VodButtonSprite->setScale(0.35f);
	auto VodButton = CCMenuItemSpriteExtra::create(
		VodButtonSprite,
		this,
		menu_selector(GDWTMatchCell::onVodClicked)
	);

    auto LevelsButtonSprite = ButtonSprite::create("Levels");
    //LevelsButtonSprite->setScale(0.35f);
	auto LevelsButton = CCMenuItemSpriteExtra::create(
		LevelsButtonSprite,
		this,
		menu_selector(GDWTMatchCell::onLevelsClicked)
	);

    bottomLeftMenu->addChild(LevelsButtonSprite);
    bottomLeftMenu->addChild(VodButtonSprite);
    bottomLeftMenu->addChild(LiveButtonSprite);
    
    bottomLeftMenu->updateLayout();*/

    return true;
}

void GDWTMatchCell::onLiveClicked(CCObject*){
    web::openLinkInBrowser(match.liveLink);
}

void GDWTMatchCell::onTeamClicked(CCObject*){

}

void GDWTMatchCell::onMoreClicked(CCObject*){
    GDWTMatchLayer::create(match)->show();
}
