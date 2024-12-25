#include "../layers/GDWTSelectionCursor.hpp"

GDWTSelectionCursor* GDWTSelectionCursor::create() {
    auto ret = new GDWTSelectionCursor();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GDWTSelectionCursor::init(){

    bottomLeft = CCScale9Sprite::createWithSpriteFrameName("persp_outline_01_001.png");
    bottomLeft->setAnchorPoint(ccp(1, .5f));
    this->addChild(bottomLeft);

    bottomRight = CCScale9Sprite::createWithSpriteFrameName("persp_outline_01_001.png");
    bottomRight->setAnchorPoint(ccp(1, .5f));
    bottomRight->setScaleX(-1);
    this->addChild(bottomRight);

    topLeft = CCScale9Sprite::createWithSpriteFrameName("persp_outline_01_001.png");
    topLeft->setAnchorPoint(ccp(1, .5f));
    topLeft->setScaleY(-1);
    this->addChild(topLeft);

    topRight = CCScale9Sprite::createWithSpriteFrameName("persp_outline_01_001.png");
    topRight->setAnchorPoint(ccp(1, .5f));
    topRight->setScale(-1);
    this->addChild(topRight);

    return true;
};

void GDWTSelectionCursor::AddOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) != options.end()) return;
    options.push_back(option);

    if (options.size() == 1)
        GDWTSelectionCursor::MoveMeTo(option);
}

void GDWTSelectionCursor::RemoveOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) != options.end() || !options.size()) return;

    options.erase(std::remove(options.begin(), options.end(), option), options.end());
}

void GDWTSelectionCursor::RemoveOption(const int optionIndex){
    if (options.size() - 1 > optionIndex || optionIndex < 0) return;

    options.erase(options.begin() + optionIndex);
}

void GDWTSelectionCursor::EraseAllOptions(){
    options.clear();
}


void GDWTSelectionCursor::MoveOptionBy(int amount){
    if (!options.size()) return;

    currentOption = abs(currentOption + amount) % options.size();

    GDWTSelectionCursor::MoveMeTo(options[currentOption]);
}

void GDWTSelectionCursor::MoveOptionTo(int optionIndex){
    if (options.size() - 1 > optionIndex || optionIndex < 0) return;

    currentOption = optionIndex;
    GDWTSelectionCursor::MoveMeTo(options[optionIndex]);
}

void GDWTSelectionCursor::MoveOptionTo(CCNode* const option){
    auto position = std::find(options.begin(), options.end(), option);
    log::info("fdisuhf");
    if (position == options.end() || !options.size()) return;
    log::info("gdfhr");

    currentOption = std::distance(options.begin(), position);

    log::info("{} | {}", currentOption, options.size());

    GDWTSelectionCursor::MoveMeTo(options[currentOption]);
}

void GDWTSelectionCursor::SetAnimationSpeed(float speed){
    animSpeed = speed;
}

void GDWTSelectionCursor::SetOffset(const CCPoint& offset){
    this->offset = offset;
}

void GDWTSelectionCursor::MoveMeTo(CCNode* const option){
    if (animSpeed == 0) return;

    float lineLength = option->getContentWidth() / 2;

    bottomLeft->setContentWidth(lineLength);
    bottomRight->setContentWidth(lineLength);
    topLeft->setContentWidth(lineLength);
    topRight->setContentWidth(lineLength);

    this->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, this->getParent()->convertToNodeSpace(option->getParent()->convertToWorldSpace(option->getPosition())) + offset), 2));

    topOrigen = option->getContentHeight() / 2;
    bottomOrigen = -option->getContentHeight() / 2;

    topLeft->stopAllActions();
    topRight->stopAllActions();
    bottomRight->stopAllActions();
    bottomLeft->stopAllActions();

    topLeft->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, topOrigen)), 2));
    topRight->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, topOrigen)), 2));
    bottomRight->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, bottomOrigen)), 2));
    bottomLeft->runAction(CCSequence::create(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, bottomOrigen)), 2), CCCallFunc::create(this, callfunc_selector(GDWTSelectionCursor::startIdleAnim)), nullptr));
}

void GDWTSelectionCursor::startIdleAnim(){
    float extra = 5;

    topLeft->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, topOrigen + extra)), 2),
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, topOrigen)), 2),
            nullptr
        )
    ));
    topRight->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, topOrigen + extra)), 2),
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, topOrigen)), 2),
            nullptr
        )
    ));

    bottomLeft->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, bottomOrigen - extra)), 2),
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, bottomOrigen)), 2),
            nullptr
        )
    ));
    bottomRight->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, bottomOrigen - extra)), 2),
            CCEaseInOut::create(CCMoveTo::create(1, ccp(0, bottomOrigen)), 2),
            nullptr
        )
    ));
}