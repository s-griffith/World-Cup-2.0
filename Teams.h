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
    //Team& operator=(const Team& other);

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
    permutation_t get_teamSpirit() const;

    int get_ability() const;

    int get_score() const;

    int get_spirit_strength() const;

    void set_teamPlayers(Player* players);

    /*
    * Checks whether or not a team has enough goalkeepers to be considered valid
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
    * Updates the pointer to the upside-down tree of players in the team
    * @param - pointer to the updated players tree root
    * @return - void
    */
    void update_players(Player* player);

    /*
    * Helper function for add_player in world_cup:
    * Adds the player to the team and updates the team's stats accordingly
    * @param - a pointer to the player to be added, its ID, goals, cards, and whether it is a goalkeeper
    * @return - void
    */
    void add_player(Player* player, const int id, const permutation_t& spirit, const int ability, const int cards, const bool goalkeeper);

    /*
    * Helper function for remove_player in world_cup:
    * Removes the player from the team and updates the team's stats accordingly
    * @param - the player's ID, goals, cards, and whether it is a goalkeeper
    * @return - void
    */
    void remove_player(const int playerID, const int goals, const int cards, const bool goalKeeper);

    /*
    * Helper function for knockout in world_cup:
    * Unites teams based on winner or loser after knockout matches.
    * Unites only the numerical fields, without uniting the player trees.
    * @param - a reference to the winning and losing teams
    * @return - void
    */
    void teams_unite(Team& bought);

private:

    /*
     * The internal fields of Team:
     *   The team's ID
     *   The number of points the team has
     *   The number of players the team has
     *   The number of goalkeepers the team has
     *   The number of games the team has played
     *   The total player ability of the players in the team
     */
    int m_id;
    int m_points;
    int m_numPlayers;
    int m_numGoalkeepers;
    int m_numGames;
    int m_totalAbility;
    permutation_t m_teamSpirit;
    Player* m_players;

};

#endif //TEAMS_H