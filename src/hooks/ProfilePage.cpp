#include "../hooks/ProfilePage.hpp"

void GDWTProfilePage::loadPageFromUserInfo(GJUserScore* user) {
    ProfilePage::loadPageFromUserInfo(user);
    auto layer = m_mainLayer;
        
    CCMenu* username_menu = typeinfo_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));

    std::vector<int> managmentIDs{
        11684904,
        9074168,
        9975327,
        1380770
    };

    for (int i = 0; i < managmentIDs.size(); i++)
    {
        if (managmentIDs[i] == user->m_accountID){
            addManagmentBadge(username_menu);
            break;
        }
    }

    std::vector<int> tourneyWinners2024IDs{
        8303773,
        6801276,
        11860237,
        5682434,
        7664649,
        4955259,
        26556060,
        2932785
    };

    for (int i = 0; i < tourneyWinners2024IDs.size(); i++)
    {
        if (tourneyWinners2024IDs[i] == user->m_accountID){
            addTourneyWinners2024Badge(username_menu);
            break;
        }
    }

    username_menu->updateLayout();
}

void GDWTProfilePage::addManagmentBadge(CCMenu* m){
    auto yourBadge = CCSprite::create("GDWT-Staff-Badge.png"_spr);
    auto bButton = CCMenuItemSpriteExtra::create(
        yourBadge,
        nullptr,
        this,
        menu_selector(GDWTProfilePage::managmentBadgeClicked)
    );
    bButton->setID("gdwtmanagment-badge");
    m->addChild(bButton);
}

void GDWTProfilePage::managmentBadgeClicked(CCObject*){
    auto alert = FLAlertLayer::create("GDWT Managment", "This user is a member of the <cy>GD World Tournament management team</c>.\n\nThey are responsible for <cg>overseeing and creating</c> various GD World Tournament events, along with <co>developing systems</c> to further help the project as a whole.", "OK");
    alert->show();
}

void GDWTProfilePage::addTourneyWinners2024Badge(CCMenu* m){
    auto yourBadge = CCSprite::create("24_GDWT_Champion_Badge.png"_spr);
    auto bButton = CCMenuItemSpriteExtra::create(
        yourBadge,
        nullptr,
        this,
        menu_selector(GDWTProfilePage::tourneyWinners2024BadgeClicked)
    );
    bButton->setID("gdwt2024worldchampion-badge");
    m->addChild(bButton);
}

void GDWTProfilePage::tourneyWinners2024BadgeClicked(CCObject*){
    auto alert = FLAlertLayer::create(nullptr, "2024 GD World Champion", "This player was a member of the South Korea team for the <cl>2024 GD World Tournament</c>. After battling to get out of their group in the group stage, they defeated <cr>Canada</c>, <cb>France</c>, and finally <cg>Mexico</c> to become champions of the inaugural <cl>GD World Tournament</c>.\n\nThe team consisted of <cy>Baeru</c>, <cy>Clutter</c>, <cy>Laniakea</c>, <cy>PoCle</c>, <cy>Guraud</c>, <cy>hanni</c>, <cy>IcaroX</c>, and <cy>Knicide</c>", "OK", nullptr, 400, false, 200, 1);
    alert->show();
}
