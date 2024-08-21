#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(GDWTMenuLayer, MenuLayer) {
    public:
        bool init();

        void onGDWTButton(CCObject*);
};