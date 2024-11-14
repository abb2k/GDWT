#include "../layers/GDWTMatchGroupCell.hpp"
#include "../layers/flagDisplay.hpp"
#include "../layers/GDWTMatchGroupLayer.hpp"

GDWTMatchGroupCell* GDWTMatchGroupCell::create(const MatchGroup& _group, CCSize size, bool s) {
    auto ret = new GDWTMatchGroupCell();
    if (ret && ret->init(_group, size, s)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTMatchGroupCell::init(const MatchGroup& _group, CCSize size, bool s){

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

    group = _group;

    auto Menu = CCMenu::create();
    Menu->setPosition({0, 0});
    this->addChild(Menu);

    auto moreButtonSprite = ButtonSprite::create("more");
    moreButtonSprite->setScale(0.725f);
    auto moreButton = CCMenuItemSpriteExtra::create(
        moreButtonSprite,
        nullptr,
        this,
        menu_selector(GDWTMatchGroupCell::onMoreClicked)
    );
    moreButton->setPosition({90, 15});
    Menu->addChild(moreButton);

    auto nameLabel = TextInput::create(160, "", "gjFont17.fnt");
    nameLabel->setPosition({62, 44});
    nameLabel->setScale(0.8f);
    nameLabel->getBGSprite()->setContentWidth(305);
    nameLabel->setEnabled(false);
    nameLabel->setString(group.groupName);
    nameLabel->getInputNode()->getPlaceholderLabel()->setOpacity(255);
    this->addChild(nameLabel);

    l.bind([this] (MatchesTask::Event* event){
        if (auto _matches = event->getValue()){
            auto matches = _matches->unwrapOrDefault();

            if (!matches.size()){
                if (_matches->isErr())
                    data::sendError(_matches->unwrapErr());
                return;
            }

            int matchAmount = 0;

            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i].groupID == group.groupID)
                    matchAmount++;
            }

            auto matchsLabel = CCLabelBMFont::create(fmt::format("{} matches", matchAmount).c_str(), "goldFont.fnt");
            matchsLabel->setScale(0.325f);
            matchsLabel->setPosition({30, 15.5f});
            matchsLabel->setZOrder(1);
            this->addChild(matchsLabel);

            auto matchsLabelBG = CCScale9Sprite::create("square02b_small.png", {0, 0, 40, 40});
            matchsLabelBG->setColor({0, 0 ,0});
            matchsLabelBG->setOpacity(75);
            matchsLabelBG->setScale(0.8f);
            matchsLabelBG->setContentSize({64, 30});
            matchsLabelBG->setPosition({30, 15});
            this->addChild(matchsLabelBG);
        }
    });

    l.setFilter(data::getMatchesData());
    
    return true;
};

void GDWTMatchGroupCell::onMoreClicked(CCObject*){
    GDWTMatchGroupLayer::create(group)->show();
}