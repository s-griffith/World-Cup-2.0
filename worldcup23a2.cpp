#include "worldcup23a2.h"

world_cup_t::world_cup_t() :
    m_numTotalPlayers(0),
    m_currentHashIndex(3),
    m_teamsByID()
{
    int currentSize = calculate_hash_size(m_currentHashIndex);
    m_playersHashTable = new Tree<GenericNode<Player*>, Player*>*[currentSize];
    for (int i = 0; i < currentSize; i++) {
        m_playersHashTable[i] = nullptr;
    }
}

world_cup_t::~world_cup_t()
{
    if (m_numTotalPlayers > 0) {
        int currentSize = calculate_hash_size(m_currentHashIndex);
        for (int i = 0; i < currentSize; i++) {
            if (m_playersHashTable[i] != nullptr) {
                m_playersHashTable[i]->erase_data();
                delete m_playersHashTable[i];
            }
        }
    }
    delete[] m_playersHashTable;
}

StatusType world_cup_t::add_team(int teamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	// TODO: Your code goes here
	return StatusType::FAILURE;
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
    Player* playerParent = tmpTeam->get_allPlayers(); //-------------------------------------------------------------------Add to teams
    //Correlate the player's games played with the total team games and the root player games played
    int playerNumGames = gamesPlayed - tmpTeam.get_games();  //-------------------------------------------------------------------Add to teams
    if (playerParent != nullptr) {
        playerNumGames -= parent->get_gamesPlayed();
    }
    //Add the player's partial spirit (it's team's spirit only including the players that joined before the current player)
    permutation_t partialSpirit = tmpTeam.get_teamSpirit(); //-------------------------------------------------------------------Add to teams + make sure operator= works
    if (playerParent != nullptr) {
        partialSpirit = playerParent.get_spirit().inv() * partialSpirit;
    }
    Player* tmpPlayer;
    try {
        tmpPlayer = new Player(playerId, player_num_games, ability, cards, goalKeeper, spirit, partialSpirit, tmpTeam, playerParent);
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    int current_size = calculate_hash_size(m_currentHashIndex);
    if (m_numTotalPlayers + 1 == current_size) {
        try {
            enlarge_hash_table();
        }
        catch (const std::bad_alloc& e) {
            delete tmpPlayer;
            return StatusType::ALLOCATION_ERROR;
        }
        m_currentHashIndex++;
    }
    try {
        insert_player_hash_table(tmpPlayer);
    }
    catch (const std::bad_alloc& e) {
        delete tmpPlayer;
        return StatusType::ALLOCATION_ERROR;
    }
    //If this is the first player in the team, update team pointer to it's players
    if (parent == nullptr) {
        tmpTeam->set_allPlayers(tmpPlayer); //-------------------------------------------------------------------Add to teams
    }
    //Remove the team from the tree sorted by player ability, and update the team's stats
    m_teamsByAbility.remove(tmpTeam->get_capablity(), tmpTeam->get_spirit_strength(), tmpTeam->get_teamID());//------------------------------------Add to teams + MultiTree
    tmpTeam->add_new_player_spirit(spirit); //-------------------------------------------------------------------Add to teams
    tmpTeam->add_new_player_ability(ability); //-------------------------------------------------------------------Add to teams
    //Re-insert the team from the tree sorted by player ability
    try {
        m_teamsByAbility.insert(tmpTeam->get_capablity(), tmpTeam->get_spirit_strength(), tmpTeam->get_teamID());//------------------------------------Add to teams + MultiTree
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
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	if (playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (!check_player_exists(playerId)) {
        return output_t<int>(StatusType::FAILURE);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    int gamesPlayed = 0;
    while (tmpPlayer != nullptr) {
        gamesPlayed += tmpPlayer->get_gamesPlayed();
        tmpPlayer = tmpPlayer->get_parent();
    }
    gamesPlayed += tmpPlayer->get_team()->get_games();
    return output_t<int>(gamesPlayed);
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!check_player_exists(playerId) || check_player_kicked_out(playerId)) {
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
    if (!check_player_exists(playerId)) {
        return output_t<int>(StatusType::FAILURE);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    return output_t<int>(tmpPlayer->get_cards());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if (playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (!check_player_exists(playerId) || check_player_kicked_out(playerId)) {
        return output_t<int>(StatusType::FAILURE);
    }
    int arrayIndex = hash_function(playerId);
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersHashTable[arrayIndex]->search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    permutation_t playerSpirit = tmpPlayer->get_partialSpirit();
    while (tmpPlayer->get_parent() != nullptr) {
        tmpPlayer = tmpPlayer->get_parent();
        playerSpirit = tmpPlayer->get_partialSpirit() * playerSpirit;
    }
    return output_t<permutation_t>(playerSpirit);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}


//-------------------------------------------Helper Functions----------------------------------------------

bool world_cup_t::check_player_exists(int playerId)
{
    int arrayIndex = hash_function(playerId);
    try {
        m_playersHashTable[arrayIndex]->search_specific_id(playerId);
    }
    catch (const NodeNotFound()& e) {
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
    catch (const NodeNotFound()& e) {}
    while (tmpPlayer->get_parent() != nullptr) {
        tmpPlayer = tmpPlayer->get_parent();
    }
    if (tmpPlayer->get_team() == nullptr) {
        return true;
    }
    return false;
}


int world_cup_t::calculate_hash_size(int index)
{
    int size = 1;
    for (int i=0; i < index; i++) {
        size *= 2;
    }
    return size - 1;
}


void world_cup_t::enlarge_hash_table()
{
    int currentSize = calculate_hash_size(m_currentHashIndex);
    int newSize = calculate_hash_size(m_currentHashIndex + 1);
    Tree<GenericNode<Player*>, Player*>** newTable = new Tree<GenericNode<Player*>, Player*>*[new_size];
    for (int i = 0; i < currentSize; i++) {
        newTable[i] = m_playersHashTable[i];
    }
    for (int i = currentSize; i < newSize; i++) {
        newTable[i] = nullptr;
    }
    Tree<GenericNode<Player*>, Player*>** tmpTable = m_playersHashTable;
    m_playersHashTable = newTable;
    destroy_old_hash_table(tmpTable);
}


void world_cup_t::destroy_old_hash_table(Tree<GenericNode<Player*>, Player*>** tmpTable)
{
    int currentSize = calculate_hash_size(m_currentHashIndex);
    for (int i = 0; i < currentSize; i++) {
        tmpTable[i] = nullptr;
    }
    delete[] tmpTable;
}


void world_cup_t::insert_player_hash_table(Player* tmpPlayer)
{
    int arrayIndex = hash_function(tmpPlayer->get_playerId());
    try {
        if (m_playersHashTable[arrayIndex] == nullptr) {
            m_playersHashTable[arrayIndex] = new Tree<GenericNode<Player*>, Player*>();
        }
        m_playersHashTable[arrayIndex]->insert(tmpPlayer, tmpPlayer->get_playerId());
    }
    catch (const std::bad_alloc& e) {
       throw e;
    }
}


int world_cup_t::hash_function(int id)
{
    return id % calculate_hash_size(m_currentHashIndex);
}