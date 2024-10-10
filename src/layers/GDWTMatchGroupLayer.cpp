#include "../layers/GDWTMatchGroupLayer.hpp"
#include "../layers/flagDisplay.hpp"
#include "../layers/GDWTMatchCell.hpp"
#include "../layers/CountyTextDisplay.hpp"

GDWTMatchGroupLayer* GDWTMatchGroupLayer::create(MatchGroup _group) {
    auto ret = new GDWTMatchGroupLayer();
    if (ret && ret->init(263, 210, _group, "square01_001.png", {0.f, 0.f, 94.f, 94.f})) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GDWTMatchGroupLayer::setup(MatchGroup _group){

    group = _group;

    this->setID("gdwt-match-group-layer");

    auto alignmentNode = CCNode::create();
    alignmentNode->setPosition(m_buttonMenu->getPosition());
    alignmentNode->setID("aligment-node");
    m_mainLayer->addChild(alignmentNode);

    auto nameLabel = InputNode::create(160, "", "gjFont17.fnt");
    nameLabel->setPosition({0, 75});
    nameLabel->setScale(1.425f);
    nameLabel->setEnabled(false);
    nameLabel->getBG()->setScale(0.25f);
    nameLabel->getBG()->setContentWidth(630);
    nameLabel->setString(group.groupName);
    alignmentNode->addChild(nameLabel);

    auto matchesListCont = CCNode::create();
    matchesListCont->setID("levels-list-container");
    matchesListCont->setScale(1.025f);
    matchesListCont->setPositionY(-9.5f);
    alignmentNode->addChild(matchesListCont);

    auto listLeft = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    listLeft->setPosition({-105.5f, -48});
    listLeft->setScaleX(0.625f);
    listLeft->setScaleY(1.15f);
    listLeft->setZOrder(1);
    matchesListCont->addChild(listLeft);

    auto listRight = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-side.png");
    listRight->setPosition({105.5f, -48});
    listRight->setScaleX(0.625f);
    listRight->setScaleY(1.15f);
    listRight->setRotation(180);
    listRight->setZOrder(1);
    matchesListCont->addChild(listRight);

    auto listTop = CCSprite::createWithSpriteFrameName("geode.loader/mods-list-bottom.png");
    listTop->setScale(0.575f);
    listTop->setPositionY(-4);
    listTop->setZOrder(1);
    listTop->setRotation(180);
    matchesListCont->addChild(listTop);

    auto listBG = CCLayerColor::create({25, 17, 37, 255});
    listBG->setZOrder(-1);
    listBG->setPosition({-200 / 2, -84});
    listBG->setContentSize({200, 70});
    matchesListCont->addChild(listBG);

    auto matchesListLayer = ScrollLayer::create({200, 70});
    matchesListLayer->setPosition({-200 / 2, -84});
    matchesListLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(matchesListLayer->getContentHeight())
        ->setGap(1)
    );
    matchesListCont->addChild(matchesListLayer);

    l.bind([this, matchesListLayer, alignmentNode] (MatchesTask::Event* event){
        if (auto _matches = event->getValue()){
            auto matches = *_matches;

            std::vector<Match> groupMatches;

            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i].groupID == group.groupID)
                    groupMatches.push_back(matches[i]);
            }

            std::vector<std::tuple<std::string, int, bool>> ctd{};

            std::vector<scoreCalcTask> tasks;

            for (int i = 0; i < groupMatches.size(); i++)
            {
                GDWTMatchCell* matchCell;
                if ((i % 2) == 0)
                    matchCell = GDWTMatchCell::create(groupMatches[i], {matchesListLayer->getContentSize().width, 60});
                else
                    matchCell = GDWTMatchCell::create(groupMatches[i], {matchesListLayer->getContentSize().width, 60}, true);

                matchesListLayer->m_contentLayer->addChild(matchCell);

                tasks.push_back(data::calculateScores(groupMatches[i].levels, groupMatches[i].teams, groupMatches[i].scoreType));
            }
            
            matchesListLayer->m_contentLayer->updateLayout();
            matchesListLayer->moveToTop();

            scoresL.bind([this, alignmentNode] (Task<std::vector<std::vector<std::tuple<std::string, int, int>> *>>::Event* event){
                if (auto _scores = event->getValue()){
                    auto scores = *_scores;

                    std::vector<std::tuple<std::string, int, bool, int>> ctd{};

                    for (int s = 0; s < scores.size(); s++)
                    {
                        auto matchScore = *scores[s];

                        for (int i = 0; i < matchScore.size(); i++)
                        {
                            bool found = false;

                            int score = 0;

                            if (group.scoreType == ScoreSystemType::Addition)
                                score = std::get<1>(matchScore[i]);
                            else
                                score = i;

                            for (int c = 0; c < ctd.size(); c++)
                            {
                                if (std::get<0>(ctd[c]) == std::get<0>(matchScore[i])){
                                    std::get<1>(ctd[c]) += score;
                                    std::get<3>(ctd[c]) += std::get<2>(matchScore[i]);
                                    found = true;
                                }
                            }

                            if (!found)
                                ctd.push_back(std::tuple<std::string, int, bool, int>{std::get<0>(matchScore[i]), score, false, std::get<2>(matchScore[i])});
                        }
                    }

                    std::ranges::sort(ctd, [](const std::tuple<std::string, int, bool, int> a, const std::tuple<std::string, int, bool, int> b) {
                        auto A = std::get<1>(a);
                        auto B = std::get<1>(b);

                        if (A == B) return std::get<3>(a) < std::get<3>(b);
                        return A < B;
                    });

                    for (int i = 0; i < ctd.size(); i++)
                    {
                        if (ctd.size() - 1 - group.amountPass < i){
                            std::get<2>(ctd[i]) = true;
                        }
                    }

                    auto CDisp = CountyTextDisplay::create(ctd, {240, 60}, false);
                    CDisp->setPosition({0, 30});
                    alignmentNode->addChild(CDisp);
                }
            });

            scoresL.setFilter(scoreCalcTask::all(
                std::move(tasks)
            ));
        }
    });

    l.setFilter(data::getMatchesData());

    return true;
};

void GDWTMatchGroupLayer::show(){
    auto scene = CCScene::get();

    this->setZOrder(scene->getChildrenCount() > 0 ? scene->getHighestChildZ() + 100 : 100);
    scene->addChild(this);
}