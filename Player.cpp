#include "Player.h"

//--------------------------------Constructor and Destructor------------------------------------

Player::Player(const int playerId, const int gamesPlayed, const int ability, const int cards, const bool goalKeeper,
               const permutation_t& spirit, const permutation_t& partialSpirit, Team* tmpTeam, Player* parent) :
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


void Player::detach() 
{
	m_team = nullptr;
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

//---------------------------------------Union Find---------------------------------------------

void Player::find()
{
    if (m_parent == nullptr || m_parent->m_parent == nullptr)
        return;
    find_update_games(this);
    find_update_partial_spirit(this);
    find_update_parents(this);
}

int Player::find_update_games(Player* tmpPlayer)
{
    if (tmpPlayer->m_parent == nullptr) {
        return 0;
    }
    tmpPlayer->m_gamesPlayed += find_update_games(tmpPlayer->m_parent);
    return tmpPlayer->m_gamesPlayed;
}

permutation_t Player::find_update_partial_spirit(Player* tmpPlayer)
{
    if (tmpPlayer->m_parent == nullptr || tmpPlayer->m_parent->m_parent == nullptr) {
        return tmpPlayer->m_partialSpirit;
    }
    tmpPlayer->m_partialSpirit = find_update_partial_spirit(tmpPlayer->m_parent) * tmpPlayer->m_partialSpirit;
    return tmpPlayer->m_partialSpirit;
}

Player* Player::find_update_parents(Player* tmpPlayer)
{
    if (tmpPlayer->m_parent == nullptr) {
        return this;
    }
    tmpPlayer->m_parent = find_update_parents(tmpPlayer->m_parent);
    return tmpPlayer->m_parent;
}

Player* Player::players_union(Player* otherTeam, int currentNumPlayers, int otherNumPlayers, \
            permutation_t currentTeamSpirit, permutation_t otherTeamSpirit)
{
    //Nothing to unite!
    if (currentNumPlayers == 0 || otherNumPlayers == 0) {
        return nullptr;
    }
    //Other team is smaller - they will be joining the current team's upside down tree
    if (currentNumPlayers >= otherNumPlayers) {
        //Change root of other team to current team's root
        otherTeam->m_parent = this;
        //Update partial spirit for team
        otherTeam->m_partialSpirit = currentTeamSpirit * otherTeam->m_partialSpirit;
        otherTeam->m_partialSpirit = otherTeam->m_parent->m_partialSpirit.inv() * otherTeam->m_partialSpirit;
        //Update games played of other team
        otherTeam->m_gamesPlayed -= m_gamesPlayed;
        //Return root of new player upside-down tree
        return this;
    }
    //Change root of current team to other team's root
    m_parent = otherTeam;
    //Update partial spirit for team
    otherTeam->m_partialSpirit = currentTeamSpirit * m_partialSpirit;
    m_partialSpirit = m_parent->m_partialSpirit.inv() * m_partialSpirit;
    //Update games played of current team
    m_gamesPlayed -= m_parent->m_gamesPlayed;
    //Return root of new player upside-down tree
    return otherTeam;
}