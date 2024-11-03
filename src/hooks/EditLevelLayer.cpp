#include "../hooks/EditLevelLayer.hpp"
#include "../utils/data.hpp"

void GDWTEditLevelLayer::onPlay(cocos2d::CCObject* sender){
    
    EditLevelLayer::onPlay(sender);

    if (data::getIsInMatch())
        data::disable2point1Percent(m_level);
}