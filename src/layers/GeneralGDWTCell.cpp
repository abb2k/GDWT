#include "../layers/GeneralGDWTCell.hpp"

GeneralGDWTCell* GeneralGDWTCell::create(const std::string& title, int enterIndex) {
    auto ret = new GeneralGDWTCell();
    if (ret && ret->init(title, enterIndex)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GeneralGDWTCell::init(const std::string& title, int enterIndex){

    size = CCSize(90, 90);

    this->setContentSize(size);

    mainContainer = CCNode::create();
    this->addChild(mainContainer);

    BG = CCScale9Sprite::create("square02_small.png");
    BG->setContentSize(size);
    BG->setPosition(size / 2);
    mainContainer->addChild(BG);

    titleLabel = geode::SimpleTextArea::create(title, "gjFont17.fnt", 0.2f);
    titleLabel->setWidth(80);
    titleLabel->setAnchorPoint({.5f, 1});
    titleLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    titleLabel->setWrappingMode(WrappingMode::SPACE_WRAP);
    titleLabel->setPosition(ccp(size.width / 2, 86));
    mainContainer->addChild(titleLabel);

    content = CCNode::create();
    mainContainer->addChild(content);

    GeneralGDWTCell::clearContent(true);

    GeneralGDWTCell::playEnterTransition(.5f, enterIndex);

    return true;
}

void GeneralGDWTCell::setCentralContent(const Match& match){
    GeneralGDWTCell::clearContent();

    GeneralGDWTCell::setTitle(match.matchName);
    /*
    - first host
    - countries vs
    - date
    - name
    */
}

void GeneralGDWTCell::setCentralContent(const Team& team){
    GeneralGDWTCell::clearContent();

    /*
    - flag
    - name
    - player amount
    */
}

void GeneralGDWTCell::setCentralContent(const MatchGroup& MG){
    GeneralGDWTCell::clearContent();

    /*
    - name
    - winners amount
    - countries amount
    - matches amount
    */
}

void GeneralGDWTCell::clearContent(bool nothingMessage){
    content->removeAllChildrenWithCleanup(true);

    if (!nothingMessage) return;

    auto NALabel = geode::SimpleTextArea::create("Nothing\nFound!", "bigFont.fnt", .5f);
    NALabel->setPosition(size / 2);
    NALabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    content->addChild(NALabel);
}

void GeneralGDWTCell::playEnterTransition(float baseDuration, int enterIndex){
    //get all effected objects
    CCArray* allChildern = mainContainer->getChildren()->shallowCopy();

    allChildern->addObjectsFromArray(content->getChildren());

    //reset values
    for (auto& child : CCArrayExt<CCNode*>(allChildern)){
        if (auto protocol = typeinfo_cast<CCRGBAProtocol*>(child))
            protocol->setOpacity(0);
        
        if (auto textArea = typeinfo_cast<geode::SimpleTextArea*>(child))
            textArea->setColor({
                textArea->getColor().r,
                textArea->getColor().g,
                textArea->getColor().b,
                0
            });
    }

    mainContainer->setPositionY(-40);

    //run animation
    float waitTime = enterIndex / 7.5f;
    
    for (auto& child : CCArrayExt<CCNode*>(allChildern)){
        if (auto textArea = typeinfo_cast<geode::SimpleTextArea*>(child)){
            for (auto& line : textArea->getLines())
            {
                line->runAction(CCSequence::create(CCDelayTime::create(waitTime), CCFadeIn::create(baseDuration), nullptr));
            }
            break;
        }

        GLbyte fadeTo = 255;

        if (BG == child)
            fadeTo = 200;

        child->runAction(CCSequence::create(CCDelayTime::create(waitTime), CCFadeTo::create(baseDuration, fadeTo), nullptr));
    }

    mainContainer->runAction(CCSequence::create(
        CCDelayTime::create(waitTime), 
        CCEaseOut::create(CCMoveTo::create(baseDuration, ccp(0, 0)), 2),
        nullptr
    ));
}

void GeneralGDWTCell::setTitle(const std::string& title){
    titleLabel->setText(title);
}