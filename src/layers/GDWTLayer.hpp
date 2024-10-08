#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTLayer : public CCLayer {
    protected:
        bool init();
    public:
        static GDWTLayer* create();

        void open();

        void update(float delta);

        void keyBackClicked();

        ScrollLayer* matchesScrollLayer;

        void refreshMatchesList(int amountPerPage);
        void MatchesListArrowLeft(CCObject*);
        void MatchesListArrowRight(CCObject*);
        CCMenuItemSpriteExtra* leftArrowMatches;
        CCMenuItemSpriteExtra* rightArrowMatches;
        int MatchesListPage;

        //

        ScrollLayer* teamsScrollLayer;

        void refreshTeamsList(int amountPerPage);
        void TeamsListArrowLeft(CCObject*);
        void TeamsListArrowRight(CCObject*);
        CCMenuItemSpriteExtra* leftArrowTeams;
        CCMenuItemSpriteExtra* rightArrowTeams;
        int TeamsListPage;

        //

        ScrollLayer* matchGroupsScrollLayer;

        void refreshMatchGroupsList(int amountPerPage);
        void MatchGroupsListArrowLeft(CCObject*);
        void MatchGroupsListArrowRight(CCObject*);
        CCMenuItemSpriteExtra* leftArrowMatchGroups;
        CCMenuItemSpriteExtra* rightArrowMatchGroups;
        int MatchGroupsListPage;

        int scrollSwitch;

        EventListener<MatchesTask> matchListener;

        EventListener<TeamsTask> teamListener;

        EventListener<MatchGroupsDataTask> MGListener;

        void OnBackButton(CCObject*);


        bool cnext = false;

        //

        void openJoinMatchMenu(CCObject*);
};