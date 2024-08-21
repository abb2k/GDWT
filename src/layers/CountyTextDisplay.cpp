#include "../layers/CountyTextDisplay.hpp"

CountyTextDisplay* CountyTextDisplay::create(std::vector<std::string> _counrtyCodes, CCSize _size, bool autoScale) {
    auto ret = new CountyTextDisplay();
    if (ret && ret->init(_counrtyCodes, _size, autoScale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CountyTextDisplay* CountyTextDisplay::create(std::vector<std::tuple<std::string, int, bool, int>> _counrtyCodesWPlacement, CCSize _size, bool autoScale) {
    auto ret = new CountyTextDisplay();
    if (ret && ret->init(_counrtyCodesWPlacement, _size, autoScale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool CountyTextDisplay::init(std::vector<std::string> _counrtyCodes, CCSize _size, bool autoScale){

    counrtyCodes = _counrtyCodes;
    size = _size;

    setContentSize(size);
    setAnchorPoint({0.5f, 0.5f});

    scalingPoint = CCNode::create();
    scalingPoint->setPosition(size / 2);
    this->addChild(scalingPoint);

    float dictance = 0;
    float extra = 0;

    for (int i = 0; i < _counrtyCodes.size(); i++)
    {
        auto s = flagDisplay::create(_counrtyCodes[i]);
        
        flags.push_back(s);

        if (i != 0)
            s->setPositionX(dictance + s->getScaledContentWidth() / 2);
        scalingPoint->addChild(s);

        dictance += s->getScaledContentWidth();

        if (i == 0){
            dictance /= 2;
            extra = dictance;
        }
            
        
        if (_counrtyCodes.size() - 1 != i){
            auto vsText = CCLabelBMFont::create(" VS ", "gjFont17.fnt");
            vsText->setScale(0.5f);
            vsText->setPositionX(dictance + vsText->getScaledContentWidth() / 2);
            scalingPoint->addChild(vsText);

            dictance += vsText->getScaledContentWidth();
        }
    }

    CCObject* child;
    CCARRAY_FOREACH(scalingPoint->getChildren(), child){
        auto n = static_cast<CCNode*>(child);
        n->setPositionX(n->getPositionX() - (dictance - extra) / 2);
    }
    if (autoScale)
        scalingPoint->setScale(_size.width / ((dictance + extra)));



    return true;
}

bool CountyTextDisplay::init(std::vector<std::tuple<std::string, int, bool, int>> counrtyCodesWPlacement, CCSize _size, bool autoScale){
    size = _size;

    setContentSize(size);
    setAnchorPoint({0.5f, 0.5f});

    auto menu = CCMenu::create();
    menu->setPosition(size / 2);
    menu->setContentSize(size);
    menu->setLayout(RowLayout::create()
        ->setAutoScale(true)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisLineAlignment(AxisAlignment::Center)
    );
    menu->setAnchorPoint({0.5f, 0.5f});
    this->addChild(menu);

    for (int i = 0; i < counrtyCodesWPlacement.size(); i++)
    {
        auto cont = CCNode::create();
        cont->setContentSize({40, 35});
        cont->setAnchorPoint({0.5f, 0.5f});

        auto s = flagDisplay::create(std::get<0>(counrtyCodesWPlacement[i]));
        s->setPosition({20, 23});
        flags.push_back(s);
        counrtyCodes.push_back(s->cCode);
        cont->addChild(s);

        std::string points = std::to_string(std::get<1>(counrtyCodesWPlacement[i]));
        auto label = CCLabelBMFont::create(points.c_str(), "goldFont.fnt");
        label->setPosition({20, 9});
        label->setScale(0.5f);
        cont->addChild(label);

        bool green = std::get<2>(counrtyCodesWPlacement[i]);

        CCNode* glow;
        if (green)
            glow = data::createCircleGlow({0, 255, 0}, 80);
        else
            glow = data::createCircleGlow({255, 0, 0}, 120);

        glow->setZOrder(-1);
        glow->setPosition({20, 23});
        cont->addChild(glow);

        menu->addChild(cont);
    }

    menu->updateLayout();

    return true;
}