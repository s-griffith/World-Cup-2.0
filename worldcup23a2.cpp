#include "worldcup23a2.h"

world_cup_t::world_cup_t() :
    m_numTotalPlayers(0),
    m_numTeams(0),
    m_currentHashSize(7),
    m_teamsByID(),
    m_teamsByAbility()
{
    m_playersHashTable = new Tree<GenericNode<Player*>, Player*>*[m_currentHashSize];
    for (int i = 0; i < m_currentHashSize; i++) {
        m_playersHashTable[i] = new Tree<GenericNode<Player*>, Player*>();
    }
}

world_cup_t::~world_cup_t()
{
    for (int i = 0; i < m_currentHashSize; i++) {
        if (m_playersHashTable[i]->m_node->get_height() >= 0) {
            m_playersHashTable[i]->erase_data(m_playersHashTable[i]->m_node);
        }
        delete m_playersHashTable[i];
    }
    if (m_teamsByID.m_node->get_height() >= 0) {
        m_teamsByID.erase_data(m_teamsByID.m_node);
    }
    delete[] m_playersHashTable;
}


StatusType world_cup_t::add_team(int teamId)
{
	if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Team* newTeam;
    try  {
        newTeam = new Team(teamId);
        m_teamsByID.insert(newTeam, teamId);
    }
    catch (const std::bad_alloc& e) {
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const InvalidID& e) {
        delete newTeam;
        return StatusType::FAILURE;
    }
    try {
        m_teamsByAbility.insert(newTeam, teamId, 0);
    }
    catch(const InvalidID& e) {
        delete newTeam;
        return StatusType::FAILURE;
    }
    m_numTeams++;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Team* team;
    try {
        team = m_teamsByID.search_and_return_data(teamId);
        m_teamsByID.remove(teamId);
        m_teamsByAbility.remove(teamId, team->get_ability());
        if (team->get_allPlayers() != nullptr) {
            team->get_allPlayers()->update_gamesPlayed(team->get_games());
            team->get_allPlayers()->detach();
        }
        delete team;
    }
    catch(const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    m_numTeams--;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || cards < 0 || !spirit.isvalid()) {
        return StatusType::INVALID_INPUT;
    }
    //If player with same ID already exists (inactive or active), return FAILURE
    if (check_player_exists(playerId)) {
        return StatusType::FAILURE;
    }
    Team* tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    //The inputs are okay - continue adding player
    Player* playerRoot = tmpTeam->get_allPlayers();
    //Correlate the player's games played with the total team games and the root player games played
    int playerNumGames = gamesPlayed - tmpTeam->get_games();
    //If this player isn't the first player on the team
    if (playerRoot != nullptr) {
        playerNumGames -= playerRoot->get_gamesPlayed();
    }
    //Add the player's partial spirit (it's team's spirit only including the players that joined before the current player)
    permutation_t partialSpirit = tmpTeam->get_teamSpirit();
    //If this player isn't the first player on the team
    if (playerRoot != nullptr) {
        //Add the current player's spirit to its partial spirit
        partialSpirit = partialSpirit * spirit;
        //Inverse the root's spirit and add it to the player's partial spirit (it will be added back in the future)
        partialSpirit = playerRoot->get_partialSpirit().inv() * partialSpirit;
    }
    else {
        partialSpirit = spirit;
    }
    Player* tmpPlayer;
    try {
        tmpPlayer = new Player(playerId, playerNumGames, ability, cards, goalKeeper, spirit, partialSpirit, playerRoot);
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (m_numTotalPlayers + 1 == m_currentHashSize) {
        try {
            enlarge_hash_table();
        }
        catch (const std::bad_alloc& e) {
            delete tmpPlayer;
            return StatusType::ALLOCATION_ERROR;
        }
    }
    try {
        insert_player_hash_table(tmpPlayer);
    }
    catch (const std::bad_alloc& e) {
        delete tmpPlayer;
        return StatusType::ALLOCATION_ERROR;
    }
    catch (const InvalidID& e) {
        delete tmpPlayer;
        return StatusType::FAILURE;
    }
    //If this is the first player in the team, update team pointer to it's players
    if (playerRoot == nullptr) {
        tmpPlayer->update_team(tmpTeam);
        tmpTeam->set_teamPlayers(tmpPlayer);
    }
    //Remove the team from the tree sorted by player ability, and update the team's stats
    m_teamsByAbility.remove(tmpTeam->get_teamID(), tmpTeam->get_ability());
    tmpTeam->add_player(tmpPlayer, playerId, spirit, ability, cards, goalKeeper);
    //Re-insert the team from the tree sorted by player ability
    try {
        m_teamsByAbility.insert(tmpTeam, tmpTeam->get_teamID(), tmpTeam->get_ability());
    }
    catch (const InvalidID& e) {}
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    m_numTotalPlayers++;
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    int result = -1;
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* team1;
    Team* team2;
    try {
        team1 = m_teamsByID.search_and_return_data(teamId1);
    }
    catch (const NodeNotFound&) {
        return output_t<int>(StatusType::FAILURE);
    }
    try {
        team2 = m_teamsByID.search_and_return_data(teamId2);
    }
    catch (const NodeNotFound&) {
        return output_t<int>(StatusType::FAILURE);
    }
    if (!team1->is_valid() || !team2->is_valid()) {
        return output_t<int>(StatusType::FAILURE);
    }
    int score1 = team1->get_score();
    int score2 = team2->get_score();
    if (score1 > score2) {
        team1->update_points_won();
        result = 1;
    }
    else if (score2 > score1) {
        team2->update_points_won();
        result = 3;
    }
    else {
        int spirit1 = team1->get_spirit_strength(); //This doesn't need to be a function anymore
        int spirit2 = team2->get_spirit_strength();
        if (spirit1 > spirit2) {
            team1->update_points_won();
            result = 2;
        }
        else if (spirit2 > spirit1) {
            team2->update_points_won();
            result = 4;
        }
        else {
            team1->update_points_tie();
            team2->update_points_tie();
            result = 0;
        }
    }
    team1->add_game();
    team2->add_game();
	return output_t<int>(result);
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	if (playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
    //Union-find algorithm - compress path from player to root
    tmpPlayer->find();
    int gamesPlayed = tmpPlayer->get_gamesPlayed();
    //If player isn't the root of the team's players
    if (tmpPlayer->get_parent() != nullptr) {
        gamesPlayed += tmpPlayer->get_parent()->get_gamesPlayed();
        if (tmpPlayer->get_parent()->get_team() != nullptr) {
            gamesPlayed += tmpPlayer->get_parent()->get_team()->get_games();
        }
    }
    else if (tmpPlayer->get_team() != nullptr) {
        gamesPlayed += tmpPlayer->get_team()->get_games();
    }
    return output_t<int>(gamesPlayed);
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (check_player_kicked_out(playerId)) {
        return StatusType::FAILURE;
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    tmpPlayer->update_cards(cards);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if (playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>(tmpPlayer->get_cards());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
	Team* t;
    try {
        t = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
	return output_t<int>(t->get_points());
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if (i < 0 || m_numTeams == 0 || i >= m_numTeams) {
        return output_t<int>(StatusType::FAILURE);
    }
    Team* team = m_teamsByAbility.find_index(m_teamsByAbility.m_node, i)->get_data();
	return output_t<int>(team->get_teamID());
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if (playerId <= 0) {
        return output_t<permutation_t>(StatusType::INVALID_INPUT);
    }
    if (check_player_kicked_out(playerId)) {
        return output_t<permutation_t>(StatusType::FAILURE);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return output_t<permutation_t>(StatusType::FAILURE);
    }
    //Union-find algorithm - compress path from player to root
    tmpPlayer->find();
    permutation_t playerSpirit = tmpPlayer->get_partialSpirit();
    if (tmpPlayer->get_parent() != nullptr) {
        playerSpirit = tmpPlayer->get_parent()->get_partialSpirit() * playerSpirit;
    }
    return output_t<permutation_t>(playerSpirit);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    Team* buyer;
    Team* bought;
    try {
        buyer = m_teamsByID.search_and_return_data(teamId1);
    }
    catch (const NodeNotFound&) {
        return StatusType::FAILURE;
    }  
    try {
        bought = m_teamsByID.search_and_return_data(teamId2);
    }
    catch (const NodeNotFound&) {
        return StatusType::FAILURE;
    }
    int prevAbility = buyer->get_ability();
    if (buyer->get_allPlayers() != nullptr && bought->get_allPlayers() != nullptr) {
        buyer->get_allPlayers()->update_gamesPlayed(buyer->get_games());
        bought->get_allPlayers()->update_gamesPlayed(bought->get_games());
        Player* root = buyer->get_allPlayers()->players_union(bought->get_allPlayers(), buyer->get_num_players(), 
                                        bought->get_num_players(), buyer->get_teamSpirit(), bought->get_teamSpirit());
        root->update_team(buyer);
        buyer->update_players(root);
    }
    else if (buyer->get_allPlayers() == nullptr && bought->get_allPlayers() != nullptr) {
        bought->get_allPlayers()->update_gamesPlayed(bought->get_games());
        buyer->update_players(bought->get_allPlayers());
        bought->get_allPlayers()->update_team(buyer);
    }
    else if (buyer->get_allPlayers() != nullptr && bought->get_allPlayers() == nullptr) {
        buyer->get_allPlayers()->update_gamesPlayed(buyer->get_games());
    }
    //Merge the internal fields of the two teams
    buyer->teams_unite(*bought);
    //Delete the bought team from the system:
    bought->update_players(nullptr);
    remove_team(teamId2);
    //Fix the location of the united team in the teams by ability tree:
    m_teamsByAbility.remove(teamId1, prevAbility);
    try {
        m_teamsByAbility.insert(buyer, teamId1, buyer->get_ability());
    }
    catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}


//-------------------------------------------Helper Functions----------------------------------------------

bool world_cup_t::check_player_exists(int playerId)
{
    int arrayIndex = hash_function(playerId);
    try {
        m_playersHashTable[arrayIndex]->search_specific_id(playerId);
    }
    catch (const NodeNotFound& e) {
        return false;
    }
    return true;
}


bool world_cup_t::check_player_kicked_out(int playerId)
{
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return true;
    }
    //Union-find algorithm - compress path from player to root
    tmpPlayer->find();
    //Check if the player was disqualified by checking if their root's team is nullptr
    if ((tmpPlayer->get_parent() == nullptr && tmpPlayer->get_team() == nullptr) || \
            (tmpPlayer->get_parent() != nullptr && tmpPlayer->get_parent()->get_team() == nullptr)) {
        return true;
    }
    return false;
}


void world_cup_t::enlarge_hash_table()
{
    int newSize = ((m_currentHashSize + 1 ) * 2 ) - 1;
    Tree<GenericNode<Player*>, Player*>** newTable = new Tree<GenericNode<Player*>, Player*>*[newSize];
    for (int i = 0; i < newSize; i++) {
        try {
            newTable[i] = new Tree<GenericNode<Player*>, Player*>();
        }
        catch (const std::bad_alloc& e) {
            delete[] newTable;
            throw e;
        }
    }
    int arrayIndex = 0;
    Player** all_players = new Player*[m_currentHashSize];
    for (int i = 0; i < m_currentHashSize; i++) {
        all_players[i] = nullptr;
    }
    Player** tmpPlayers = all_players;
    for (int i = 0; i < m_currentHashSize; i++) {
        m_playersHashTable[i]->get_all_data(all_players);
        while (*all_players != nullptr) {
            arrayIndex = (*all_players)->get_playerId() % newSize;
            newTable[arrayIndex]->insert(*all_players, (*all_players)->get_playerId());
            all_players++;
        }
    }
    delete[] tmpPlayers;
    Tree<GenericNode<Player*>, Player*>** tmpTable = m_playersHashTable;
    m_playersHashTable = newTable;
    destroy_old_hash_table(tmpTable);
    m_currentHashSize = newSize;
}


void world_cup_t::destroy_old_hash_table(Tree<GenericNode<Player*>, Player*>** tmpTable)
{
    for (int i = 0; i < m_currentHashSize; i++) {
        delete tmpTable[i];
    }
    delete[] tmpTable;
}


void world_cup_t::insert_player_hash_table(Player* tmpPlayer)
{
    int arrayIndex = hash_function(tmpPlayer->get_playerId());
    m_playersHashTable[arrayIndex]->insert(tmpPlayer, tmpPlayer->get_playerId());
}


int world_cup_t::hash_function(int id)
{
    return id % m_currentHashSize;
}