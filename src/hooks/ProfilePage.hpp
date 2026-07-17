#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include "../utils/data.hpp"

#include <Geode/modify/ProfilePage.hpp>

#include <alphalaneous.badgify/include/Badgify.hpp>

class $modify(GDWTProfilePage, ProfilePage) {
    struct Fields{
        async::TaskHolder<PlayerDataFuture::Output> playerDataListener;
        // std::unordered_map<CCObject*, std::pair<std::string, std::string>> allBadges;
    };

    public:
        /*
        void playersDataRecieved(PlayerDataFuture::Output out);

        void loadPageFromUserInfo(GJUserScore* a2);

        void createBadge(const std::string& ID, const std::string& name, const std::string& description, const int& orderPrio);

        void onBadgeClicked(CCObject* sender);
        */

        static void isUserInBadge(std::string badgeID, int accountID, alpha::badgify::Badge badge, CCSprite* sprite);
};