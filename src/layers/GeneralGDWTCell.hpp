#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include <Geode/utils/web.hpp>
#include "../layers/UserDisplay.hpp"

using boolTask = Task<bool>;

using namespace geode::prelude;

class GeneralGDWTCell : public CCMenu {
    protected:
        bool init(const std::string& title);
    public:
        static GeneralGDWTCell* create(const std::string& title);

        CCSize size;

        CCNode* mainContainer;

        CCMenuItemSpriteExtra* overallBtn;

        CCLayer* content;

        geode::SimpleTextArea* titleLabel;
        CCScale9Sprite* BG;

        void OnClick(CCObject*);

        void setCentralContent(const Match& match);

        void loadMatchImage(Ref<CCImage> image, CCNode* alignImageTo);

        boolTask loadMatchThumbnail(std::string matchName, CCNode* alignImageTo);
        EventListener<boolTask> thumbnailListener;

        void setCentralContent(const Team& team);

        void setCentralContent(const MatchGroup& MG);

        void clearContent(bool nothingMessage = false);

        void playEnterTransition(float baseDuration, int enterIndex = 0);

        void setTitle(const std::string& title);

        CCNode* tagsContainer;
        void addTag(const Tag& tag);
        void removeAllTags();
        void updateTagsDisplay();

        std::set<Tag> tags{};
};