#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(GDWTLevelInfoLayer, LevelInfoLayer) {
    public:
        void onPlay(cocos2d::CCObject* sender);

        void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
};