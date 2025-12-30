#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include "../utils/data.hpp"

#include <Geode/modify/ProfilePage.hpp>

#include <alphalaneous.badges_api_reimagined/include/BadgesAPI.hpp>
class $modify(GDWTProfilePage, ProfilePage) {
    struct Fields{
        EventListener<PlayerDataTask> playerDataListener;
        std::unordered_map<CCObject*, std::pair<std::string, std::string>> allBadges;
    };

    public:
        void playersDataRecieved(PlayerDataTask::Event* e);

        void loadPageFromUserInfo(GJUserScore* a2);

        void createBadge(const std::string& ID, const std::string& name, const std::string& description, const int& orderPrio);

        void onBadgeClicked(CCObject* sender);

        static void isUserInBadge(const std::string& badgeID, const int& accountID, const Badge& badge);
};