#include "../hooks/LevelInfoLayer.hpp"
#include "../utils/data.hpp"

void GDWTLevelInfoLayer::onPlay(cocos2d::CCObject* sender){

    LevelInfoLayer::onPlay(sender);

    if (m_isBusy && data::getIsInMatch()){
        data::disable2point1Percent(m_level);
    }
}

void GDWTLevelInfoLayer::FLAlert_Clicked(FLAlertLayer* p0, bool p1){

    if (auto title = static_cast<CCLabelBMFont*>(p0->m_mainLayer->getChildByID("title")) && p1 && m_isBusy && data::getIsInMatch()){
        data::disable2point1Percent(m_level);
    }

    LevelInfoLayer::FLAlert_Clicked(p0, p1);
}

bool GDWTLevelInfoLayer::init(GJGameLevel* level, bool challenge){
    if (!LevelInfoLayer::init(level, challenge)) return false;

    if (auto playMenu = this->getChildByID("play-menu")){

        auto indicator = CCLabelBMFont::create(".", "bigFont.fnt");
        indicator->setColor({ 0, 255, 0 });
        indicator->setPositionY(12);
        indicator->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        playMenu->addChild(indicator);

        if (auto playButton = playMenu->getChildByID("play-button")){
            indicator->setPositionX(playButton->getPositionX() + playButton->getContentWidth() / 2);
            indicator->setPositionY(playButton->getPositionY() - playButton->getContentHeight() / 2 + indicator->getContentHeight() / 2);
        }

        indicator->setVisible(data::getIsInMatch());
    }

    return true;
}