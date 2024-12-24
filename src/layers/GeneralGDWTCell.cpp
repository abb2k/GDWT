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
    if (!CCMenu::init()) return false;
    size = CCSize(90, 90);

    this->setContentSize(size);
    this->setAnchorPoint(ccp(0, 0));
    this->setEnabled(true);
    this->setTouchPriority(-2);

    mainContainer = CCNode::create();
    mainContainer->setContentSize(size);
    overallBtn = CCMenuItemSpriteExtra::create(
        mainContainer,
        nullptr,
        this,
        menu_selector(GeneralGDWTCell::OnClick)
    );
    overallBtn->setPosition(size / 2);
    overallBtn->m_scaleMultiplier = 1.05f;
    this->addChild(overallBtn);

    BG = CCScale9Sprite::create("square02_small.png");
    BG->setContentSize(size);
    BG->setPosition(size / 2);
    BG->setOpacity(200);
    mainContainer->addChild(BG);

    titleLabel = geode::SimpleTextArea::create(title, "gjFont17.fnt", 0.2f);
    titleLabel->setWidth(80);
    titleLabel->setAnchorPoint({.5f, 1});
    titleLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    titleLabel->setWrappingMode(WrappingMode::SPACE_WRAP);
    titleLabel->setPosition(ccp(size.width / 2, 86));
    mainContainer->addChild(titleLabel);

    content = CCLayer::create();
    mainContainer->addChild(content);

    GeneralGDWTCell::clearContent(true);

    GeneralGDWTCell::playEnterTransition(.5f, enterIndex);

    return true;
}

void GeneralGDWTCell::OnClick(CCObject*){

}

void GeneralGDWTCell::setCentralContent(const Match& match){
    GeneralGDWTCell::clearContent();

    GeneralGDWTCell::setTitle(match.matchName);

    auto ImageOutline = CCScale9Sprite::createWithSpriteFrameName("blockOutline_01_001.png");
    ImageOutline->setScale(.5f);
    ImageOutline->setContentSize(CCSize{145, 85});
    ImageOutline->setPosition(ccp(size.width / 2, 48));
    ImageOutline->setZOrder(2);
    content->addChild(ImageOutline);

    auto thumbnailLoadingCircle = LoadingCircle::create();
    thumbnailLoadingCircle->setParentLayer(content);
    thumbnailLoadingCircle->show();
    thumbnailLoadingCircle->setPosition(ImageOutline->getPosition() - thumbnailLoadingCircle->getContentSize() / 2);
    thumbnailLoadingCircle->setScale(.3f);

    thumbnailListener.bind([this, thumbnailLoadingCircle, ImageOutline] (boolTask::Event* event){
        if (auto imageRes = event->getValue()){
            thumbnailLoadingCircle->fadeAndRemove();
            if (*imageRes) return;
            //no image was found

            auto NAText = CCLabelBMFont::create("No thumbnail\nFound!", "bigFont.fnt");
            NAText->setPosition(ImageOutline->getPosition());
            NAText->setScale(0.275f);
            NAText->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            content->addChild(NAText);
        }
    });
    //dis laggs nned to fix
    thumbnailListener.setFilter(GeneralGDWTCell::loadMatchThumbnail(match.matchName, ImageOutline));

    auto hostIcon = UserDisplay::create(match.hosts[0]);
    hostIcon->setScale(0.75f);
    hostIcon->setPosition(ccp(65, 17));
    content->addChild(hostIcon);

    int hostAmount = match.hosts.size() + match.coHosts.size() - 1;
    auto hostAmountLabel = CCLabelBMFont::create(fmt::format("+{}", hostAmount).c_str(), "gjFont17.fnt");
    hostAmountLabel->setScale(.3f);
    hostAmountLabel->setAnchorPoint(ccp(0, .5f));
    hostAmountLabel->setPosition(ccp(73, 16));
    content->addChild(hostAmountLabel);

    auto dateDisplay = TextInput::create(60, "date");
    dateDisplay->setString(match.date);
    dateDisplay->setEnabled(false);
    dateDisplay->getInputNode()->m_placeholderLabel->setOpacity(255);
    dateDisplay->setPosition(ccp(27, 14));
    dateDisplay->setScale(0.625f);
    content->addChild(dateDisplay);

    /*
    - first host Y
    - countries vs / thumbnail M
    - date X
    - name Y
    */
}

void GeneralGDWTCell::loadMatchImage(Ref<CCImage> image, CCNode* alignImageTo){
    auto texture = new CCTexture2D();
    texture->autorelease();
    texture->initWithImage(image);
    auto imageSprite = CCSprite::createWithTexture(texture);
    imageSprite->setPosition(alignImageTo->getPosition());
    float padding = 0.5f;
    imageSprite->setScaleX((alignImageTo->getScaledContentWidth() - padding) / imageSprite->getContentWidth());
    imageSprite->setScaleY((alignImageTo->getScaledContentHeight() - padding) / imageSprite->getContentHeight());
    content->addChild(imageSprite);
}

boolTask GeneralGDWTCell::loadMatchThumbnail(std::string matchName, CCNode* alignImageTo){
    web::WebRequest req = web::WebRequest();

    std::replace(matchName.begin(), matchName.end(), ' ', '-');

    return req.get(fmt::format("https://raw.githubusercontent.com/abb2k/GDWT-Thumbnails/main/Thumbnails/{}.jpeg", matchName)).map(
        [this, alignImageTo](web::WebResponse* res) -> boolTask::Value {
            if (!res->ok()) return false;

            auto data = res->data();
            std::thread imageThread = std::thread([data, alignImageTo, this](){
                Ref<CCImage> image = new CCImage();
                image->autorelease();
                image->initWithImageData(const_cast<uint8_t*>(data.data()),data.size());
                geode::Loader::get()->queueInMainThread([this, image, alignImageTo](){
                    GeneralGDWTCell::loadMatchImage(image, alignImageTo);
                });
            });
            imageThread.detach();

            return true;
        },
        [](auto) -> std::monostate {
            return std::monostate();
        }
    );
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

    CCPoint prevPos = overallBtn->getPosition();
    overallBtn->setPositionY(-40);

    //run animation
    float waitTime = enterIndex / 15.f;
    
    for (auto& child : CCArrayExt<CCNode*>(allChildern)){
        if (auto textArea = typeinfo_cast<geode::SimpleTextArea*>(child)){
            for (auto& line : textArea->getLines())
            {
                auto action = CCSequence::create(CCDelayTime::create(waitTime), CCFadeIn::create(baseDuration), nullptr);
                action->setTag(1);
                line->runAction(action);
            }
            break;
        }

        GLbyte fadeTo = 255;

        if (BG == child)
            fadeTo = 200;

        auto action = CCSequence::create(CCDelayTime::create(waitTime), CCFadeTo::create(baseDuration, fadeTo), nullptr);
        action->setTag(1);
        child->runAction(action);
    }

    overallBtn->runAction(CCSequence::create(
        CCDelayTime::create(waitTime), 
        CCEaseOut::create(CCMoveTo::create(baseDuration, prevPos), 2),
        nullptr
    ));
}

void GeneralGDWTCell::setTitle(const std::string& title){
    CCAction* currAction = nullptr;
    if (titleLabel->getLines().size())
        currAction = titleLabel->getLines()[0]->getActionByTag(1);

    titleLabel->setText(title);

    if (!currAction) return;

    for (auto& line : titleLabel->getLines())
    {
        auto myAction = static_cast<CCAction*>(currAction->copy());
        line->runAction(myAction);
    }
    
}