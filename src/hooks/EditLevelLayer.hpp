#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/EditLevelLayer.hpp>
class $modify(GDWTEditLevelLayer, EditLevelLayer) {
    public:
        void onPlay(cocos2d::CCObject* sender);
};