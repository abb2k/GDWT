#include "../layers/GDWTTeamCell.hpp"
#include "../layers/flagDisplay.hpp"
#include "../layers/GDWTTeamLayer.hpp"

GDWTTeamCell* GDWTTeamCell::create(const Team& _team, CCSize size, bool s) {
    auto ret = new GDWTTeamCell();
    if (ret && ret->init(_team, size, s)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTTeamCell::init(const Team& _team, CCSize size, bool s){

    setContentSize(size);

    ccColor4B color;
    if (!s)
        color = { 116, 165, 168 , 255};
    else
        color = { 64, 92, 94 , 255};

    auto bg = CCLayerColor::create(color);
    bg->setZOrder(-2);
    bg->setContentSize(getContentSize());
    this->addChild(bg);

    team = _team;

    auto flag = flagDisplay::create(team.countryCode);
    flag->flagButton->setEnabled(false);
    flag->setScale(1.75f);
    flag->setPosition({30, 42});
    this->addChild(flag);

    auto label = CCLabelBMFont::create(team.countryCode.c_str(), "gjFont17.fnt");
    label->setScale(0.75f);
    label->setPosition({29, 13});
    this->addChild(label);

    int playersAmount = 0;

    for (int i = 0; i < team.accounts.size(); i++)
    {
        if (team.accounts[i].isActive)
            playersAmount++;
    }
    
    auto playersLabel = CCLabelBMFont::create(fmt::format("{} players", playersAmount).c_str(), "goldFont.fnt");
    playersLabel->setScale(0.425f);
    playersLabel->setPosition({89, 42});
    playersLabel->setZOrder(1);
    this->addChild(playersLabel);

    auto playersLabelBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
    playersLabelBG->setColor({0, 0 ,0});
    playersLabelBG->setOpacity(75);
    playersLabelBG->setScale(0.8f);
    playersLabelBG->setContentSize({76, 30});
    playersLabelBG->setPosition({89, 41});
    this->addChild(playersLabelBG);

    auto Menu = CCMenu::create();
    Menu->setPosition({0, 0});
    this->addChild(Menu);

    auto moreButtonSprite = ButtonSprite::create("more");
    moreButtonSprite->setScale(0.725f);
    auto moreButton = CCMenuItemSpriteExtra::create(
        moreButtonSprite,
        nullptr,
        this,
        menu_selector(GDWTTeamCell::onMoreClicked)
    );
    moreButton->setPosition({90, 15});
    Menu->addChild(moreButton);

    return true;
};

void GDWTTeamCell::onMoreClicked(CCObject*){
    GDWTTeamLayer::create(team)->show();
}