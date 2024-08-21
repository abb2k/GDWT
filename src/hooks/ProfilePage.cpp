#include "../hooks/ProfilePage.hpp"

void GDWTProfilePage::loadPageFromUserInfo(GJUserScore* user) {
    ProfilePage::loadPageFromUserInfo(user);
    auto layer = m_mainLayer;
        
    CCMenu* username_menu = typeinfo_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));

    std::vector<int> managmentIDs{
        11684904,
        9074168,
        9975327,
        5292934,
        7194979
    };

    for (int i = 0; i < managmentIDs.size(); i++)
    {
        if (managmentIDs[i] == user->m_accountID){
            addManagmentBadge(username_menu);
            break;
        }
    }

    username_menu->updateLayout();
}

void GDWTProfilePage::addManagmentBadge(CCMenu* m){
    auto yourBadge = CCSprite::create("GDTW-Button.png"_spr);
    yourBadge->setScale(0.175f);
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