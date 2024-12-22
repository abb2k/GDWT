#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GeneralGDWTCell : public CCNode {
    protected:
        bool init();
    public:
        static GeneralGDWTCell* create();

        CCSize size;
};