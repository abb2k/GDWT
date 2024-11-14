#include "../hooks/MenuLayer.hpp"
#include "../layers/GDWTLayer.hpp"
#include "../utils/data.hpp"

bool GDWTMenuLayer::init() {
	if (!MenuLayer::init()) return false;
 
	data::getPlayersData().listen([] (Result<std::vector<PlayerData>>*){});

	data::getTeamsData().listen([] (Result<std::vector<Team>>*){});

    data::getMatchesData().listen([] (Result<std::vector<Match>>*){});

	data::getMatchGroupsData().listen([] (Result<std::vector<MatchGroup>>*){});

	auto BMenu = this->getChildByID("bottom-menu");

    auto GDWTButtonSprite = CCSprite::create("GDTW-Button.png"_spr);
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

