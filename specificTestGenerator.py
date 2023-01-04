import random
import string
import os


def testGenerator():
    for i in range(32, 33):
        listTeams = [-1]
        listPlayers = [-1]
        numGames = []
        spirit = [1, 2, 3, 4, 5]
        lenT = 0
        lenP = 0
        with open("inFiles/test" + str(i) + ".in", 'w') as file:
            with open("outFilesDesired/output" + str(i) + ".out", 'w') as outFile:
                for j in range(1, 101):
                    if listTeams[0] < 0:
                        lenT = len(listTeams) - 1
                    else:
                        lenT= 0
                    if listPlayers[0] < 0:
                        lenP = len(listPlayers) - 1
                    file.writelines(["add_team ", str(j), "\n"])
                    if lenT == 0:
                        listTeams.pop(0)
                    listTeams.append(j)
                    outFile.writelines(["add_team: SUCCESS\n"])
                for j in range(1, 101):
                    if listTeams[0] < 0:
                        lenT = len(listTeams) - 1
                    else:
                        lenT= 0
                    if listPlayers[0] < 0:
                        lenP = len(listPlayers) - 1
                    playerID = j
                    random.shuffle(spirit)
                    games = str(random.randint(0, 2147483647))
                    file.writelines(["add_player ", str(playerID), " ", str(listTeams[random.randint(0, lenT)]), " ",  str(spirit[0]), ",", str(spirit[1]), ",", str(spirit[2]), ",", str(spirit[3]), ",", str(spirit[4]), " ", games, " ", str(random.randint(0, 2147483647)), " ", str(random.randint(0, 2147483647)), " ", str(bool(random.randint(0, 1))).lower(), "\n"])
                    if lenP == 0:
                        listPlayers.pop(0)
                    listPlayers.append(playerID)
                    numGames.append(games)
                    outFile.writelines(["add_player: SUCCESS\n"])
                for j in range (2, 101):
                    file.writelines(["buy_team ", str(1), " ", str(j), "\n"])
                    outFile.writelines(["buy_team: SUCCESS\n"])
                for j in range(1, 101):
                    file.writelines(["num_played_games_for_player ", str(j), "\n"])
                    outFile.writelines(["num_played_games_for_player: SUCCESS, " + str(numGames[j-1] + "\n")])
                

    pass


def main():
    testGenerator()

if __name__ == "__main__":
    main()