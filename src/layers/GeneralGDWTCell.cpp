#include "../layers/GeneralGDWTCell.hpp"

GeneralGDWTCell* GeneralGDWTCell::create() {
    auto ret = new GeneralGDWTCell();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GeneralGDWTCell::init(){

    size = CCSize(90, 60);

    this->setContentSize(size);

    auto BG = CCScale9Sprite::create("square02_small.png");
    BG->setContentSize(size);
    BG->setPosition(size / 2);
    this->addChild(BG);

    



    return true;
}