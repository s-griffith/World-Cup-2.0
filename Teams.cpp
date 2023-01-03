#include "Teams.h"

//--------------------------------Constructors, Assignment Operator and Destructor----------------------------

Team::Team(const int teamID) :
        m_id(teamID),
        m_points(0),
        m_numPlayers(0),
        m_numGoalkeepers(0),
        m_numGames(0),
        m_totalAbility(0),
        m_teamSpirit(),
        m_players(nullptr)
{}


Team::Team() :
        m_id(0),
        m_points(0),
        m_numPlayers(0),
        m_numGoalkeepers(0),
        m_numGames(0),
        m_totalAbility(0),
        m_teamSpirit(),
        m_players(nullptr)
{}


/*Team& Team::operator=(const Team& other) {
    m_id = other.m_id;
    m_points = other.m_points;
    m_numPlayers = other.m_numPlayers;
    m_numGoalkeepers = other.m_numGoalkeepers;
    m_numGames = other.m_numGames;
    m_totalAbility += other.m_totalAbility;
    m_teamSpirit = m_teamSpirit * other.m_teamSpirit;
    m_players = nullptr;
    return *this;
}*/


Team::~Team() {
    m_players = nullptr;
}


//-----------------------------------------------------Getters--------------------------------------------------

int Team::get_points() const {
    return m_points;
}

int Team::get_num_players() const {
    return m_numPlayers;
}

int Team::get_games() const {
    return m_numGames;
}

int Team::get_teamID() const  {
    return m_id;
}

Player* Team::get_allPlayers() const {
    return m_players;
}   

permutation_t Team::get_teamSpirit() const {
    return m_teamSpirit;
}

int Team::get_ability() const {
    return m_totalAbility;
}

int Team::get_score() const {
    return m_points + m_totalAbility;
}

int Team::get_spirit_strength() const {
    return m_teamSpirit.strength();
}

//-------------------------------------Update Stats Functions----------------------------

void Team::set_teamPlayers(Player* players) {
    m_players = players;
}

bool Team::is_valid() const{
    if (m_numGoalkeepers >= 1) {
        return true;
    }
    return false;
}

void Team::add_game() {
    m_numGames++;
}

void Team::update_points_won() {
    m_points += 3;
}

void Team::update_points_tie() {
    m_points += 1;
}

void Team::update_players(Player* player) {
    m_players = player;
}



//-------------------------------------Helper Functions for WorldCup----------------------------

void Team::add_player(Player* player, const int id, const permutation_t& spirit, const int ability, 
                                                                                const int cards, const bool goalkeeper){
    if (m_numPlayers == 0) {
        m_teamSpirit = spirit;
    }
    else {
        m_teamSpirit = m_teamSpirit * spirit;
    }
    m_totalAbility += ability;
    m_numPlayers++;
    if (goalkeeper) {
        m_numGoalkeepers++;
    }
}



void Team::teams_unite(Team& bought) {
    if (m_numPlayers == 0) {
        m_teamSpirit = bought.m_teamSpirit;
    }
    else if (bought.m_numPlayers != 0) {
            m_teamSpirit = m_teamSpirit * bought.m_teamSpirit;
    }
    m_points += bought.m_points;
    m_numPlayers += bought.m_numPlayers;
    m_numGoalkeepers += bought.m_numGoalkeepers;
    m_numGames = 0;
    m_totalAbility += bought.m_totalAbility;
}

//--------------------------------world_cup Players Functions----------------------------

