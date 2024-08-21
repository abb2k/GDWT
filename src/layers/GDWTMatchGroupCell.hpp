#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTMatchGroupCell : public CCNode {
    protected:
        bool init(MatchGroup _group, CCSize size, bool s);
    public:
        static GDWTMatchGroupCell* create(MatchGroup _group, CCSize size, bool s = false);

        MatchGroup group;

        void onMoreClicked(CCObject*);

        EventListener<MatchesTask> l;
};