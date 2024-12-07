#!/usr/bin/env python3

#import matplotlib.pyplot as plt
import argparse
import os
import csv
import pandas as pd
from game_result import *
import matplotlib.pyplot as plt

MOVE_STATISTICS_FILE_NAME = "move_statistics.csv"
TOOL_CARD_COUNT = 12
ROUND_COUNT = 10
CSV_DELIMETER = ','

MODE_SEPARATE = "separate"
MODE_COMBINED = "combined"

COMBINED_AGENT_NAME = ""

TOTAL_MOVE_COUNT_IND = 0
MOVES_MADE_IND = 1
FILTERED_MOVE_COUNT_IND = 2

CSV_MOVE_COL_IND_GAMENUMBER  = 0
CSV_MOVE_COL_IND_ROUNDNUMBER = 1
CSV_MOVE_COL_IND_PLAYERTYPE  = 2
CSV_MOVE_COL_IND_DTFMCOUNT   = 3
CSV_MOVE_COL_IND_TC_0_COUNT  = 4
CSV_MOVE_COL_IND_TC_1_COUNT  = 5
CSV_MOVE_COL_IND_TC_2_COUNT  = 6
CSV_MOVE_COL_IND_TOTAL_COUNT = 7
CSV_MOVE_COL_IND_FILTERED    = 8


class MoveStatistics:
    def from_list(csvAttributes: list, gameInfos: list):
        res = MoveStatistics()
        res.gameNumber = int(csvAttributes[CSV_MOVE_COL_IND_GAMENUMBER])
        res.roundNumber = int(csvAttributes[CSV_MOVE_COL_IND_ROUNDNUMBER])
        res.playerType = csvAttributes[CSV_MOVE_COL_IND_PLAYERTYPE]
        res.dtfmCount = int(csvAttributes[CSV_MOVE_COL_IND_GAMENUMBER])
        res.tc0Count = int(csvAttributes[CSV_MOVE_COL_IND_TC_0_COUNT])
        res.tc1Count = int(csvAttributes[CSV_MOVE_COL_IND_TC_1_COUNT])
        res.tc2Count = int(csvAttributes[CSV_MOVE_COL_IND_TC_2_COUNT])
        res.totalMoveCount = int(csvAttributes[CSV_MOVE_COL_IND_TOTAL_COUNT])
        res.filteredMoveCount = int(csvAttributes[CSV_MOVE_COL_IND_FILTERED])
        res.gameInfo = gameInfos[res.gameNumber]
        return res

def show_plot(x_Data: list, y_Data: list):
    fig, ax = plt.subplots()  # Create a figure containing a single axes.
    ax.plot(x_Data, y_Data)  # Plot some data on the axes.
    plt.show()

def read_csv_file(fileName: str):
    rows = []
    with open(fileName, newline='') as csvfile:
        spamreader = csv.reader(csvfile, delimiter = CSV_DELIMETER, quotechar='"')
        for row in spamreader:
            rows.append(row)

    return rows




def collect_data_from_dirs(directories: list) -> list:
    moveStatistics = []
    playerMoves = {}
    for dirName in directories:
        print(f"Processing {dirName}")

        moveStatisticsFileName = os.path.join(dirName, MOVE_STATISTICS_FILE_NAME)
        if not os.path.isfile(moveStatisticsFileName):
            print(f"Error - file not found: {moveStatisticsFileName}")
            continue

        gameInformationFileName = os.path.join(dirName, GAME_RESULT_FILE_NAME)
        if not os.path.isfile(gameInformationFileName):
            print(f"Error - file not found: {gameInformationFileName}")
            continue


        game_info_csv_rows = read_csv_file(gameInformationFileName)
        gameInfos = [GameResult.from_list(csv_row) for csv_row in game_info_csv_rows[1:]]

        move_csv_rows = read_csv_file(moveStatisticsFileName)
        for move_csw_row in move_csv_rows[1:]:
            currMoveStatistics = MoveStatistics.from_list(move_csw_row, gameInfos)

            if currMoveStatistics.playerType not in playerMoves:
                playerMoves[currMoveStatistics.playerType] = [[0,0,0] for _ in range(ROUND_COUNT + 1)]

            playerMoves[currMoveStatistics.playerType][currMoveStatistics.roundNumber][TOTAL_MOVE_COUNT_IND] += currMoveStatistics.totalMoveCount
            playerMoves[currMoveStatistics.playerType][currMoveStatistics.roundNumber][MOVES_MADE_IND] += 1
            playerMoves[currMoveStatistics.playerType][currMoveStatistics.roundNumber][FILTERED_MOVE_COUNT_IND] += currMoveStatistics.filteredMoveCount

            moveStatistics.append(currMoveStatistics)


    return moveStatistics, playerMoves


def combine_player_results(playerStatistics):
    combined_results = {
        MODE_COMBINED: [[0,0,0] for i in range(ROUND_COUNT + 1)]
    }

    for playerType, roundInfo in playerStatistics.items():
        ind = 0
        for totalMoveCount, movesMade, filteredMoveCount in roundInfo[1:]:
            combined_results[MODE_COMBINED][ind + 1][TOTAL_MOVE_COUNT_IND] += totalMoveCount
            combined_results[MODE_COMBINED][ind + 1][MOVES_MADE_IND] += movesMade
            combined_results[MODE_COMBINED][ind + 1][FILTERED_MOVE_COUNT_IND] += filteredMoveCount
            
            ind += 1

    return combined_results

def visualize_data(inputDirs: list, mode: str, outputPath: str, noShow: bool, showFilteredForPlayers: list, dontShowPlayers: list):
    moveStatisticsData, playerStatistics = collect_data_from_dirs(inputDirs)
    
    if mode == MODE_COMBINED:
        playerStatistics = combine_player_results(playerStatistics)

    plt.xlabel("Round Number")
    plt.ylabel("Average Branching Factor")
    
    x_data = [i for i in range(1, ROUND_COUNT + 1)]
    plt.xticks(x_data)
    y_data = []
    legend = []
    for playerType, roundInfo in playerStatistics.items():
        if playerType in dontShowPlayers:
            continue

        y_data.append([totalMoveCount / movesMade for totalMoveCount, movesMade, _ in roundInfo[1:]])
        legend.append(f"{playerType} agent")
        plt.plot(x_data, y_data[-1])

        if playerType in showFilteredForPlayers:
            y_data.append([filteredMoveCount / movesMade for _, movesMade, filteredMoveCount in roundInfo[1:]])
            legend.append(f"{playerType} agent - filtered")
            plt.plot(x_data, y_data[-1])

    plt.legend(legend,fontsize='x-large')
    plt.gcf().set_size_inches(8, 4.8)


    if outputPath:
        plt.savefig(outputPath)

    if not noShow:
        plt.show()
    else:
        plt.close()


def main():
    parser = argparse.ArgumentParser( 
        description ='Displays a diagram about move statistics from a directory full of tournament results'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournaments results", required = True)
    parser.add_argument("-r", help = "Allows recursive checking i.e. allows to analyze a group of move statistics", default=False, action='store_true')
    parser.add_argument("-mode", help = F"Determines the mode. The mode \"{MODE_COMBINED}\" merges the results of all player's subresults and displays this merged result. The mode \"{MODE_SEPARATE}\" visualizes the subresult of every player separately", choices = [MODE_SEPARATE, MODE_COMBINED], default = MODE_SEPARATE)
    parser.add_argument("-o", help = "Output path for plot save")
    parser.add_argument("-no_show", help="Does not show the plot on the screen - used for saving only runs",action='store_true', default = False)
    parser.add_argument("-hide_agents",help="Does not visualize the branching factor of the listed players",default=[],nargs='+')
    parser.add_argument("-show_filtered", help="The list of players that will be the heuristic filtered move count shown for",default=[],nargs=2)
    args = parser.parse_args()

    directories_to_analyze = [args.i] if not args.r else [os.path.join(args.i, subdir) for subdir in os.listdir(args.i)]
    visualize_data(directories_to_analyze, args.mode, args.o, args.no_show, args.show_filtered, args.hide_agents)
   
    

if __name__ == "__main__":
    main()