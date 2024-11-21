#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTTeamLayer : public Popup<const Team&> {
    protected:
        bool setup(const Team& _team) override;
    public:
        static GDWTTeamLayer* create(const Team& _team);

        void show() override;

        Team team;

        ScrollLayer* playersScroll;
        ScrollLayer* inactiveScroll;
};