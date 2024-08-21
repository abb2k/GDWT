#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTTeamLayer : public Popup<Team> {
    protected:
        bool setup(Team _team);
    public:
        static GDWTTeamLayer* create(Team _team);

        void show();

        Team team;

        ScrollLayer* playersScroll;
        ScrollLayer* inactiveScroll;
};