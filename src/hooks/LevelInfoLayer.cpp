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
        log::info("ran");
        data::disable2point1Percent(m_level);
    }

    LevelInfoLayer::FLAlert_Clicked(p0, p1);
}