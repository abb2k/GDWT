#include "../hooks/PlayLayer.hpp"
#include "../utils/data.hpp"

void GDWTPlayLayer::destroyPlayer(PlayerObject* player, GameObject* p1){
    PlayLayer::destroyPlayer(player, p1);

    if (!data::getIsInMatch()) return;

    if (!player->m_isDead) return;

    if (!m_fields->hasRespawned) return;
    m_fields->hasRespawned = false;

    int combo = data::getCombo(this->m_level->m_levelID.value(), this->getCurrentPercentInt());

    if (!(m_fields->currentRunStartprecent == 0 && !m_isPracticeMode)) return;
    if (this->m_level->m_levelID == 0) return;

    sendProgressMessage(this->getCurrentPercentInt(), this->m_level, combo);
}

void GDWTPlayLayer::resetLevel() {
    PlayLayer::resetLevel();

    if (!data::getIsInMatch()) return;

    m_fields->hasRespawned = true;

    m_fields->currentRunStartprecent = this->getCurrentPercent();
}

void GDWTPlayLayer::levelComplete() {
    PlayLayer::levelComplete();

    if (!data::getIsInMatch()) return;

    if (!m_fields->hasRespawned) return;
    m_fields->hasRespawned = false;

    int combo = data::getCombo(this->m_level->m_levelID.value(), this->getCurrentPercentInt());

    if (!(m_fields->currentRunStartprecent == 0 && !m_isPracticeMode)) return;
    if (this->m_level->m_levelID == 0) return;

    sendProgressMessage(100, this->m_level, combo);
}

void GDWTPlayLayer::onDiscordMessageSent(Task<Result<>>::Event* e){
    if (auto* res = e->getValue()){

    }
}

void GDWTPlayLayer::sendProgressMessage(int precent, GJGameLevel* level, int combo){
    if (data::getCBF()){

        try{
            if (m_fields->notifCBF)
                m_fields->notifCBF->hide();
        }
        catch (...){}
        
        
        m_fields->notifCBF = geode::Notification::create("Message was not sent! you have CBF on! please disable it!", nullptr, 4);
        m_fields->notifCBF->show();

        return;
    }

    auto embed = data::embedWithPlayerColor();

    DiscordMessage message{};

    embed.title = fmt::format("{} got {}% on {}", GJAccountManager::get()->m_username, precent, level->m_levelName);

    embed.description = fmt::format("LevelID: {} | AccountID: {}", level->m_levelID.value(), GJAccountManager::get()->m_accountID);

    if (combo > 0){
        embed.description += " | Combo: x" + std::to_string(combo + 1);
    }

    message.embeds.push_back(embed);

    m_fields->listener.bind(this, &GDWTPlayLayer::onDiscordMessageSent);

    m_fields->listener.setFilter(data::SendDiscordMessage(message));

    data::SendSheetProgress(std::to_string(precent));
}