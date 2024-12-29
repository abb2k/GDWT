#include "../layers/GDWTSelectionCursor.hpp"
#include "../utils/ScaleContentTo.hpp"

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

void GDWTSelectionCursor::addOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) != options.end()) return;
    options.push_back(option);

    if (options.size() == 1)
        GDWTSelectionCursor::MoveMeTo(0, true);
}

void GDWTSelectionCursor::removeOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) != options.end() || !options.size()) return;

    options.erase(std::remove(options.begin(), options.end(), option), options.end());
}

void GDWTSelectionCursor::removeOption(const int optionIndex){
    if (options.size() - 1 > optionIndex || optionIndex < 0) return;

    options.erase(options.begin() + optionIndex);
}

void GDWTSelectionCursor::eraseAllOptions(){
    options.clear();
}


void GDWTSelectionCursor::moveOptionBy(int amount){
    if (!options.size()) return;

    int newOption = abs(currentOption + amount) % options.size();

    GDWTSelectionCursor::MoveMeTo(newOption);
}

void GDWTSelectionCursor::moveOptionTo(int optionIndex){
    if (options.size() - 1 > optionIndex || optionIndex < 0) return;

    GDWTSelectionCursor::MoveMeTo(optionIndex);
}

void GDWTSelectionCursor::moveOptionTo(CCNode* const option){
    auto position = std::find(options.begin(), options.end(), option);

    if (position == options.end() || !options.size()) return;

    int newOption = std::distance(options.begin(), position);

    GDWTSelectionCursor::MoveMeTo(newOption);
}

void GDWTSelectionCursor::setAnimationSpeed(float speed){
    animSpeed = speed;
}

void GDWTSelectionCursor::setOffset(const CCPoint& offset){
    this->offset = offset;
}

void GDWTSelectionCursor::setWrapOffset(float offset){
    wrapOffset = offset;
}

void GDWTSelectionCursor::setIdleMoveDistance(float dictance){
    idleMoveDist = dictance;
}


void GDWTSelectionCursor::setCallback(const std::function<void(CCNode* const)>& callback){
    this->callback = callback;
}

void GDWTSelectionCursor::realign(){
    GDWTSelectionCursor::MoveMeTo(currentOption, true);
}

void GDWTSelectionCursor::MoveMeTo(const int optionIndex, bool realign){
    if ((animSpeed == 0 || optionIndex == currentOption) && !realign) return;

    currentOption = optionIndex;
    CCNode* option = options[optionIndex];

    if (callback != NULL && !realign)
        callback(option);

    float lineLength = option->getContentWidth() / 2 / this->getScaleX();

    topLeft->stopAllActions();
    topRight->stopAllActions();
    bottomRight->stopAllActions();
    bottomLeft->stopAllActions();
    this->stopAllActions();

    bottomLeft->runAction(CCEaseInOut::create(ScaleContentTo::create(1 / animSpeed, lineLength, bottomLeft->getContentHeight()), 2));
    bottomRight->runAction(CCEaseInOut::create(ScaleContentTo::create(1 / animSpeed, lineLength, bottomRight->getContentHeight()), 2));
    topLeft->runAction(CCEaseInOut::create(ScaleContentTo::create(1 / animSpeed, lineLength, topLeft->getContentHeight()), 2));
    topRight->runAction(CCEaseInOut::create(ScaleContentTo::create(1 / animSpeed, lineLength, topRight->getContentHeight()), 2));

    this->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, this->getParent()->convertToNodeSpace(option->getParent()->convertToWorldSpace(option->getPosition())) + offset), 2));

    topOrigen = option->getContentHeight() / 2 / this->getScaleY() + wrapOffset;
    bottomOrigen = -option->getContentHeight() / 2 / this->getScaleY() - wrapOffset;

    topLeft->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, topOrigen)), 2));
    topRight->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, topOrigen)), 2));
    bottomRight->runAction(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, bottomOrigen)), 2));
    bottomLeft->runAction(CCSequence::create(CCEaseInOut::create(CCMoveTo::create(1 / animSpeed, ccp(0, bottomOrigen)), 2), CCCallFunc::create(this, callfunc_selector(GDWTSelectionCursor::startIdleAnim)), nullptr));
}

void GDWTSelectionCursor::startIdleAnim(){
    float extra = idleMoveDist / this->getScaleY();

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