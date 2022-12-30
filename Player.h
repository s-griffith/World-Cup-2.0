#ifndef PLAYER_H
#define PLAYER_H

class Team;

//#include "Teams.h"
#include "wet2util.h"
#include "Teams.h"
#include <memory>

/*
* Class Player
* This class is used to represent a player in the world_cup system.
*/
class Player {
public:

    /*
    * Constructor of Player class
    * @param name - The player's ID, the number of games, goals and cards of the player,
    *      and whether the player is a goalkeeper
    * @return - A new instance of Player.
    */
    Player(const int playerId, const int gamesPlayed, const int ability, const int cards, const bool goalKeeper,
           const permutation_t& spirit, const permutation_t& partialSpirit, Team* tmpTeam, Player* parent);

    /*
    * Copy Constructor and Assignment Operator of Player class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    * Additionally, we ensure that internal fields are not initialized as garbage values by deleting the default
    *   constructor.
    */
    Player() = delete;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;

    /*
    * Destructor of Player class
    * @param - none
    * @return - void
    */
    ~Player();

    /*
    * Return the player's ID
    * @return - integer of the player's ID
    */
    int get_playerId() const;

    /*
    * Return the number of games the player played
    * @return - integer of the number of games the player played
    */
    int get_gamesPlayed() const;

    int get_ability() const;

    /*
    * Return the number of cards the player received
    * @return - integer of the number of cards the player received
    */
    int get_cards() const;

    /*
    * Return if the player is a goalkeeper
    * @return - bool, with true if the player is a goalkeeper, otherwise false
    */
    bool get_goalkeeper() const;

    /*
    * Return the spirit of the current player
    * @return - spirit, of class permutation_t
    */
    permutation_t& get_spirit();

    /*
    * Return the partial spirit of the current player
    * @return - partial spirit, of class permutation_t
    */
    permutation_t& get_partialSpirit();

    /*
    * Return the team ID of the team that the player belongs to
    * @return - int, the team ID of the player
    */
    Team* get_team();

    /*
    * Return the parent of the current player
    * @return - pointer to player
    */
    Player* get_parent();

    void detach();

    /*
    * Update the number of games the player played
    * @param - the number of new games the player played
    * @return - void
    */
    void update_gamesPlayed(const int gamesPlayed);

    /*
    * Update the number of cards the player received
    * @param - the number of new cards the player received
    * @return - void
    */
    void update_cards(const int cardsReceived);

    /*
    * Update the team that the player plays for
    * @param - shared pointer to the team that the player plays for
    * @return - void
    */
    void update_team(Team* tmpTeam);

    /*
    * Compress the path between the player and the root.
    * @return - none.
    */
    void find();

    /*
    * While compressing the path from the player to root in find, the number of games played are updated.
    *       The games of the players between the current player and the root will be added to the current player.
    * @return - none
    */
    int find_update_games(Player* tmpPlayer);

    /*
    * While compressing the path from the player to root in find, the partial spirits are updated.
    *       The games of the players between the current player and the root will be added to the current player.
    * @return - none
    */
    permutation_t find_update_partial_spirit(Player* tmpPlayer);

    /*
    * Compress the path from the player to the root of the upside down tree.
    * @return - pointer to the root player of the upside-down tree.
    */
    Player* find_update_parents(Player* tmpPlayer);

    /*
    * Unite between the two team players.
    * @return - root of new player upside down tree.
    */
    Player* players_union(Player* otherTeam, int currentNumPlayers, int otherNumPlayers, \
            permutation_t current_team_spirit, permutation_t other_team_spirit);

private:


    /*
     * The internal fields of Player:
     *   The player's ID
     *   The number of games the player has played
     *   The player's ability
     *   The number of cards the player has
     *   Whether or not the player is a goalkeeper
     *   The player's spirit
     *   The player's partial spirit (it's teams spirit only including the players that joined before the player + the current player's spirit)
     *   A pointer to the player's team
     *   A pointer to player's parent
     */    
    int m_playerId;
    int m_gamesPlayed;
    int m_ability;
    int m_cards;
    bool m_goalkeeper;
    permutation_t m_spirit;
    permutation_t m_partialSpirit;
    Team* m_team;
    Player* m_parent;
};


#endif //PLAYER_H