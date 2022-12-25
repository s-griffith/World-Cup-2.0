#include "Player.h"

//--------------------------------Constructor and Destructor------------------------------------

Player::Player(const int playerId, const int gamesPlayed, const int ability, const int cards, const bool goalKeeper,
               permutation_t& spirit, permutation_t& partialSpirit, Team* tmpTeam, Player* parent) :
    m_playerId(playerId),
    m_gamesPlayed(gamesPlayed),
    m_ability(ability),
    m_cards(cards),
    m_goalkeeper(goalKeeper),
    m_spirit(spirit),
    m_partialSpirit(partialSpirit),
    m_team(tmpTeam),
    m_parent(parent)
{}


Player::~Player() {
    m_team = nullptr;
    m_parent = nullptr;
}


//---------------------------------------Getters---------------------------------------------

int Player::get_playerId() const
{
    return m_playerId;
}


int Player::get_gamesPlayed() const
{
    return m_gamesPlayed;
}


int Player::get_ability() const
{
    return m_ability;
}


int Player::get_cards() const
{
    return m_cards;
}


bool Player::get_goalkeeper() const
{
    return m_goalkeeper;
}


permutation_t& Player::get_spirit()
{
    return m_spirit;
}


permutation_t& Player::get_partialSpirit()
{
    return m_partialSpirit;
}


Team* Player::get_team()
{
    return m_team;
}


Player* Player::get_parent()
{
    return m_parent;
}

//---------------------------------------Setters---------------------------------------------

void Player::update_gamesPlayed(const int gamesPlayed)
{
    m_gamesPlayed += gamesPlayed;
}


void Player::update_cards(const int cardsReceived)
{
    m_cards += cardsReceived;
}


void Player::update_team(Team* tmpTeam)
{
    m_team = tmpTeam;
}

