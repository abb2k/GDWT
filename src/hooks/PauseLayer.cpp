#include "../hooks/PauseLayer.hpp"
#include "../utils/data.hpp"

void GDWTPauseLayer::customSetup() {
	PauseLayer::customSetup();

    if (auto bottomMenu = this->getChildByID("bottom-button-menu")){
        auto connectionLabel = CCLabelBMFont::create("Connected to match", "bigFont.fnt");
        connectionLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        connectionLabel->setColor({ 167, 255, 167 });
        bottomMenu->addChild(connectionLabel);

        bottomMenu->updateLayout();
        connectionLabel->setScale(.5f);

        connectionLabel->setVisible(data::getIsInMatch());
    }

	return;
}