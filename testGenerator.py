import random
import string
import os


def testGenerator():
    for i in range(10, 25):
        listTeams = [-1]
        listPlayers = [-1]
        spirit = [1, 2, 3, 4, 5]
        lenT = 0
        lenP = 0
        with open("inFiles/test" + str(i) + ".in", 'w') as file:
            for j in range(40):
                if listTeams[0] < 0:
                    lenT = len(listTeams) - 1
                else:
                    lenT= 0
                if listPlayers[0] < 0:
                    lenP = len(listPlayers) - 1
                num = random.randint(0, 10)
                if num == 0: 
                    teamID = random.randint(0, 2147483647)
                    file.writelines(["add_team ", str(teamID), "\n"])
                    if lenT == 0:
                        listTeams.pop(0)
                    listTeams.append(teamID)
                elif num == 1:
                    file.writelines(["remove_team ", str(listTeams.pop(random.randint(0, lenT))), "\n"])
                    if len(listTeams) == 0:
                        listTeams.append(-1)
                elif num == 2:
                    playerID = random.randint(0, 2147483647)
                    random.shuffle(spirit)
                    file.writelines(["add_player ", str(playerID), " ", str(listTeams[random.randint(0, lenT)]), " ",  str(spirit[0]), ",", str(spirit[1]), ",", str(spirit[2]), ",", str(spirit[3]), ",", str(spirit[4]), " ", str(random.randint(0, 2147483647)), " ", str(random.randint(0, 2147483647)), " ", str(random.randint(0, 2147483647)), " ", str(bool(random.randint(0, 1))).lower(), "\n"])
                    if lenP == 0:
                        listPlayers.pop(0)
                    listPlayers.append(playerID)
                elif num == 3:
                    file.writelines(["play_match ", str(listTeams[random.randint(0, lenT)]), " ", str(listTeams[random.randint(0, lenT)]), "\n"])
                elif num == 4:
                    file.writelines(["num_played_games_for_player ", str(listPlayers[random.randint(0, lenP)]), "\n"])
                elif  num == 5:
                    file.writelines(["add_player_cards ", str(listPlayers[random.randint(0, lenP)]), " ", str(random.randint(0, 2147483647)), "\n"])
                elif num == 6:
                    file.writelines(["get_player_cards ", str(listPlayers[random.randint(0, lenP)]), "\n"])
                elif num == 7:
                    file.writelines(["get_team_points ", str(listTeams[random.randint(0, lenT)]), "\n"])
                elif num ==  8:
                    file.writelines(["get_ith_pointless_ability ", str(random.randint(0, lenT)), "\n"])
                elif num == 9:
                    file.writelines(["get_partial_spirit ", str(listPlayers[random.randint(0, lenP)]), "\n"])
                elif num == 10:
                    file.writelines(["buy_team ", str(listTeams[random.randint(0, lenT)]), " ", str(listTeams[random.randint(0, lenT)]), "\n"])
    pass


def main():
    testGenerator()

if __name__ == "__main__":
    main()