// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #2.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A2_H_
#define WORLDCUP23A2_H_

#include "wet2util.h"
#include "Player.h"
#include "Teams.h"
#include "AVLTree.h"
#include "GenericNode.h"
#include "AVLMultiTree.h"
#include "ComplexNode.h"
#include <memory.h>
#include <iostream>

class world_cup_t {
private:
    //Total number of players in world cup (including inactive players)
    int m_numTotalPlayers;
    //Total number of teams in the world cup
    int m_numTeams;
    //Current hash table size
    int m_currentHashSize;
    //Hash table of AVL trees of players
    Tree<GenericNode<Player*>, Player*>** m_playersHashTable;
    //Tree of shared pointers of the type team, with all the teams in the game sorted by their ID
    Tree<GenericNode<Team*>, Team*> m_teamsByID;
    //Tree of shared pointers of the type team, with all the teams in the game.
    //It is sorted by their overall players' ability + points, then the team's spiritual "power", and then the teams' ID
    MultiTree<Team*> m_teamsByAbility;

    //-------------------------------------------Helper Functions----------------------------------------------

    /*
    * Check if player exists in hash table (inactive or active)
    * @return - true if player already exists, else return false
    */
    bool check_player_exists(int playerId);

    /*
    * Check if player exists in hash table (inactive or active)
    * @return - true if player already exists, else return false
    */
    bool check_player_kicked_out(int playerId);

    /*
    * When the number of players = size of table, the table needs to be enlarged to ensure time complexity of O(1)
    *   for inserting + searching for players
    * @return - none
    */
    void enlarge_hash_table();

    /*
    * Deallocate memory of previous hash table
    * @return - none
    */
    void destroy_old_hash_table(Tree<GenericNode<Player*>, Player*>** tmpTable);

    /*
    * Insert new player to hash table
    * @return - none
    */
    void insert_player_hash_table(Player* tmpPlayer);

    /*
    * Calculate placement of player in hash table according to the hash function
    * @return - integer that is equal to the player's index in the hash table
    */
    int hash_function(int id);

public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId,
	                      const permutation_t &spirit, int gamesPlayed,
	                      int ability, int cards, bool goalKeeper);
	
	output_t<int> play_match(int teamId1, int teamId2);
	
	output_t<int> num_played_games_for_player(int playerId);
	
	StatusType add_player_cards(int playerId, int cards);
	
	output_t<int> get_player_cards(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	output_t<int> get_ith_pointless_ability(int i);
	
	output_t<permutation_t> get_partial_spirit(int playerId);
	
	StatusType buy_team(int teamId1, int teamId2);
	
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
