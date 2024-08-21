#include "../hooks/MenuLayer.hpp"
#include "../layers/GDWTLayer.hpp"
#include "../utils/data.hpp"

bool GDWTMenuLayer::init() {
	if (!MenuLayer::init()) return false;

	data::getPlayersData().listen([] (std::vector<PlayerData>*){});

	data::getTeamsData().listen([] (std::vector<Team>*){});

    data::getMatchesData().listen([] (std::vector<Match>*){});

	data::getMatchGroupsData().listen([] (std::vector<MatchGroup>*){});

	auto BMenu = this->getChildByID("bottom-menu");

    auto GDWTButtonSprite = CCSprite::create("GDTW-Button.png"_spr);
    GDWTButtonSprite->setScale(1.1f);
	auto GDWTButton = CCMenuItemSpriteExtra::create(
		GDWTButtonSprite,
		this,
		menu_selector(GDWTMenuLayer::onGDWTButton)
	);

	BMenu->addChild(GDWTButton);

	GDWTButton->setID("gdwt-button");

	BMenu->updateLayout();

	return true;
}

void GDWTMenuLayer::onGDWTButton(CCObject*) {
	GDWTLayer::create()->open();
}

