#ifndef TEAMS_H
#define TEAMS_H

class Player;

#include "Player.h"
#include "AVLTree.h"
#include <memory>
#include "wet2util.h"
#include "AVLMultiTree.h"
#include "GenericNode.h"

/*
* Class Team
* This class is used to represent a team in the world_cup system.
*/
class Team {
public:

    /*
    * Default constructor of Team class - creates an empty team with no players
    * @param - the team's ID, the team's initial points
    * @return - A new instance of Team
    */
    Team(const int teamID);

    /*
    * Default constructor of Team class
    * @param - none
    * @return - A new instance of Team with default values
    */
    Team();

    /*
    * Copy Constructor of Team class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor.
    */
    Team(const Team& other) = delete;

    /*
    * Assignment Operator of Team class
    * Used in the knockout_winner function in world_cup
    * Copies the numerical fields of Team without copying the trees Team contains
    */
    Team& operator=(const Team& other);

    /*
    * Destructor of Team class
    * @param - none
    * @return - void
    */
    ~Team();

    /*
    * Return the team's points
    * @param - none
    * @return - the team's points
    */
    int get_points() const;

    /*
    * Return the number of cards the team has received
    * @param - none
    * @return - the number of cards
    */
    int get_cards() const;

    /*
    * Return the team's goals
    * @param - none
    * @return - the number of goals
    */
    int get_goals() const;

    /*
    * Return the number of players the team contains
    * @param - none
    * @return - the number of players
    */
    int get_num_players() const;

    /*
    * Return the number of games the team has played
    * @param - none
    * @return - the number of games
    */
    int get_games() const;

    /*
    * Return the team's top player (top scorer)
    * @param - none
    * @return - a pointer to the team's top scorer
    */
    const Player* get_top_scorer() const;

    /*
    * Return the team's ID number
    * @param - none
    * @return - the team's ID number
    */
    int get_teamID() const;

    /*
    * Return the team's ID number
    * @param - none
    * @return - the team's ID number
    */
    Player* get_allPlayers() const;

    /*
    * Return the team's ID number
    * @param - none
    * @return - the team's ID number
    */
    permutation_t get_teamSpirit() const; //can we make this pointer or reference or something and then const to avoid copying?  

    int get_ability() const;

    int get_score() const;

    int get_spirit_strength() const;

    /*
    * Update the team ID of the players in the team
    * @param - a pointer to the team the players now belong to
    * @return - void
    */
    void update_team_id(Team* team);

    void set_teamPlayers(Player* players);

    /*
    * Checks whether or not a team has enough players and goalkeepers to be considered valid
    * @param - none
    * @return - bool, whether or not the team is valid
    */
    bool is_valid() const;

    /*
    * Increase the number of games a team has played by 1 (used for play_match in world_cup)
    * @param - none
    * @return - void
    */
    void add_game();

    /*
    * Updates the number of points the team has after winning a match
    * @param - none
    * @return - void
    */
    void update_points_won();

    /*
    * Updates the number of points the team has after tying in a match
    * @param - none
    * @return - void
    */
    void update_points_tie();

    /*
    * Increases the number of goals the team has
    * @param - the number of goals to be added
    * @return - void
    */
    void update_num_goals(const int goals);

    /*
    * Updates the number of cards the team has
    * @param - the number of cards to be added
    * @return - void
    */
    void update_num_cards(const int cards);

    /*
    * Updates the top player (top scorer) of the team
    * @param - none
    * @return - void
    */
    void update_top_player();

    /*
    * Updates the closest right team
    * @param - the new closest right team
    * @return - void
    */
    void update_closest_right(Team* team2);

    /*
    * Updates the closest left team
    * @param - the new closest left team
    * @return - void
    */
    void update_closest_left(Team* team1);

    /*
    * Helper function for add_player in world_cup:
    * Adds the player to the team and updates the team's stats accordingly
    * @param - a pointer to the player to be added, its ID, goals, cards, and whether it is a goalkeeper
    * @return - StatusType
    */
    StatusType add_player(Player* player, const int id, const permutation_t& spirit, const int ability, const int cards, const bool goalkeeper);

    /*
    * Helper function for remove_player in world_cup:
    * Removes the player from the team and updates the team's stats accordingly
    * @param - the player's ID, goals, cards, and whether it is a goalkeeper
    * @return - void
    */
    void remove_player(const int playerID, const int goals, const int cards, const bool goalKeeper);

    /*
    * Helper function for update_player_stats in world_cup:
    * Removes a player from the playersByScore tree of its team 
    * @param - the player's ID, number of goals, and number of cards
    * @return - void
    */
    void remove_player_by_score(const int id, const int goals, const int cards);

    /*
    * Helper function for update_player_stats in world_cup:
    * Inserts a player to the playersByScore tree of its team 
    * @param - a pointer to the updated player, the player's ID, number of goals, and number of cards
    * @return - void
    */
    void insert_player_by_score(Player* player, const int id, const int goals, const int cards);

    /*
    * Helper function for update_player_stats in world_cup:
    * Update the total team stats based on the changed player stats
    * @param - the number of goals and cards to be added to the total
    * @return - void
    */
    void update_team_stats(const int goals, const int cards);

    /*
    * Helper function for unite_teams in world_cup:
    * Updates the new team's total numerical fields and merges the trees of the players in the previous teams
    * @param - pointers to the previous teams
    * @return - void
    */
    void unite_teams(Team* team1, Team* team2);

    /*
    * Helper function for unite_teams in world_cup:
    * Merges two arrays ordered by ID's of pointers to players into one united array.
    * @param - the array, pointers to arrays of the previous teams, lengths of each array
    * @return - void
    */
    void mergeByID(Player** nPlayers, Player** players1, Player** players2, const int len1, const int len2);

    /*
    * Helper function for unite_teams in world_cup:
    * Merges two arrays ordered by score, goals, and ID of pointers to players into one united array.
    * @param - the array, pointers to arrays of the previous teams, lengths of each array
    * @return - void
    */
    void mergeByScore(Player** nPlayers, Player** players1, Player** players2, const int len1, const int len2);

    /*
    * Helper function for get_all_players in world_cup:
    * Inserts all of the players into a given array
    * @param - a pointer to the first term in an array
    * @return - void
    */
    void get_all_team_players(int* const output);

    /*
    * Helper function for get_closest_player in world_cup:
    * Finds the closest player according to the closest player pointers
    * @param - the player's ID
    * @return - the ID of the closest player
    */
    int get_closest_team_player(const int playerId);

    /*
    * Helper function for knockout in world_cup:
    * Counts the number of qualified teams, starting with the first team that is in range (this*), until reaching 
    * maxTeamID
    * @param - the maximum team ID of the range
    * @return - the number of qualified teams in the given range
    */
    int knockout_count(const int maxTeamID);

    /*
    * Helper function for knockout in world_cup:
    * Inserts a copy of each qualified team in the range into a given array 
    * @param - the array to be filled and the maximum team ID of the range
    * @return - void
    */
    void knockout_insert(Team* teams, const int maxID);

    /*
    * Helper function for knockout in world_cup:
    * Unites teams based on winner or loser after knockout matches.
    * Unites only the numerical fields, without uniting the player trees.
    * @param - a reference to the winning and losing teams
    * @return - void
    */
    void knockout_unite(Team& winner, Team& loser);

private:

    /*
     * The internal fields of Team:
     *   The team's ID
     *   The number of points the team has
     *   The number of players the team has
     *   The number of goalkeepers the team has
     *   The number of goals the team has scored
     *   The number of cards the team has
     *   The number of games the team has played
     *   The total player ability of the players in the team
     */
    int m_id;
    int m_points;
    int m_numPlayers;
    int m_numGoalkeepers;
    int m_numGoals;
    int m_numCards;
    int m_numGames;
    int m_totalAbility;
    permutation_t m_teamSpirit;
    Player* m_players;

};

#endif //TEAMS_H