#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/ProfilePage.hpp>
class $modify(GDWTProfilePage, ProfilePage) {
    public:
        void loadPageFromUserInfo(GJUserScore* a2);

        void managmentBadgeClicked(CCObject*);

        void addManagmentBadge(CCMenu* m);

        void addTourneyWinners2024Badge(CCMenu* m);

        void tourneyWinners2024BadgeClicked(CCObject*);
};