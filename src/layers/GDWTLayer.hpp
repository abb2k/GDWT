#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include "../layers/GDWTSelectionCursor.hpp"

using namespace geode::prelude;

class GDWTLayer : public CCLayer {
    protected:
        bool init();
    public:
        static GDWTLayer* create();

        void open();

    private:

        void keyBackClicked();

        void OnBackButton(CCObject*);

        ScrollLayer* mainScroll;
        void updateMainScrollSize();
        void eraseListforRefresh();
        LoadingCircle* refreshLoadingCircle;

        GDWTSelectionCursor* myCursor;
        void SideButtonClicked(CCObject* sender);
        void onOptionSwitched(CCNode* const option);

        CCMenuItemSpriteExtra* addSideMenuButton(const std::string& name);
        CCMenu* sideMenu;

        CCMenuItemSpriteExtra* matchesBtn;
        CCMenuItemSpriteExtra* teamsBtn;
        CCMenuItemSpriteExtra* matchGroupsBtn;
        CCMenuItemSpriteExtra* playerBtn;

        std::unordered_map<std::string, std::vector<Match>> matchesPage{};
        std::unordered_map<std::string, std::vector<Team>> teamsPage{};
        std::unordered_map<std::string, std::vector<MatchGroup>> matchGroupsPage{};

        Task<Result<>> loadMatchesPage();
        EventListener<Task<Result<>>> loadMatchPageListener;
        void createMatchesPage(Task<Result<>>::Event* e);

        void openJoinMatchMenu(CCObject*);
};