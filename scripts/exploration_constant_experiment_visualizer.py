#!/usr/bin/env python3

import os
import re
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from tournaments_config import PLAYER_STATISTICS_CSV_FILENAME
from player_statistics import PLAYER_STATISTICS_WIN_COUNT_COL


# def visualize_exploration_constant_experiment_results(inputTournamentDir: str, noShow: bool, savePath: str):
#     pattern = r'w(\d+)_it(\d+)_expl(-?\d+\.\d+)'

#     it_worldCount_pairs = set()
#     winCountsByConfig = {}
#     for dir in os.listdir(inputTournamentDir):
#         match = re.search(pattern, dir)

        
#         worlds = match.group(1)
#         iterations = match.group(2)
#         expl = match.group(3)

#         it_worldCount_pairs.add((iterations, worlds))

#         playerStatisticsFileName = os.path.join(inputTournamentDir, dir, PLAYER_STATISTICS_CSV_FILENAME)
#         playerStatistics = pd.read_csv(playerStatisticsFileName)
#         if expl not in winCountsByConfig:
#             winCountsByConfig[expl] = [(worlds, playerStatistics.iloc[0][PLAYER_STATISTICS_WIN_COUNT_COL])]
#         else:
#             winCountsByConfig[expl].append((worlds, playerStatistics.iloc[0][PLAYER_STATISTICS_WIN_COUNT_COL]))


        

#     plt.xlabel("Iteration count / Determinizing world count")
#     plt.ylabel("Win count in 100 games")
    
#     x_data = [f"it={it}\nworlds={worldCount}" for it, worldCount in sorted(it_worldCount_pairs, key = lambda x: int(x[1]))]

#     legend = []
    
    

#     highest_win_count_by_expl = []
#     y_datas = []
#     for expl, dataByExpl in sorted(winCountsByConfig.items(), key = lambda x: x):
#         sortedDataByExpl = sorted(dataByExpl, key = lambda x: int(x[0]))
#         y_data = [winCount for worlds, winCount in sortedDataByExpl]
#         print(f"Expl={expl} -> {y_data}")
#         if len(highest_win_count_by_expl) == 0: 
#             highest_win_count_by_expl = [(expl,winCount) for worlds, winCount in sortedDataByExpl]
#         else:
#             ind = 0
#             for pastExpl, pastWinCount in highest_win_count_by_expl:
#                 if pastWinCount < sortedDataByExpl[ind][1]:
#                     highest_win_count_by_expl[ind] = (expl, sortedDataByExpl[ind][1])
#                 ind += 1


#         y_datas.append(y_data)
#         #print(f"Expl={expl} -> {sorted(dataByExpl, key = lambda x: int(x[0]))}")
#         #print(y_data)
#         legend.append(f"Exploration constant = {expl}")
#         plt.plot(x_data, y_data)

#     for i, (expl, v) in enumerate(highest_win_count_by_expl):
#         plt.text(i, v / 2, f"Expl\n{expl}", ha='center', va='center')

#     print(f"Best data: {highest_win_count_by_expl}")
#     average_config = np.mean(y_datas, axis=0)
#    # legend.append("Average config")
#     plt.bar(x_data, [winCount for expl, winCount in highest_win_count_by_expl])
#    # plt.plot(x_data, average_config)

#     plt.legend(legend)

#     plt.gcf().set_size_inches(15, 9)

#     if len(savePath) > 0:
#         plt.savefig(savePath)

#     if not noShow:
#         plt.show()
#     else:
#         plt.close()


def visualize_exploration_constant_experiment_results(inputTournamentDirs: list, noShow: bool, savePath: str):
    pattern = r'w(\d+)_it(\d+)_expl(-?\d+\.\d+)'
    
    playoutStrategies = ["First", "Rules-based ONLY DTFM", "Rules-based ALL MOVES", "Minimax"]
    currSubplotInd = 0
    for inputTournamentDir in inputTournamentDirs:
     #   ax = plt.subplot(currSubplotInd)
        it_worldCount_pairs = set()
        winCountsByConfig = {}
        for dir in os.listdir(inputTournamentDir):
            match = re.search(pattern, dir)

            
            worlds = match.group(1)
            iterations = match.group(2)
            expl = match.group(3)

            it_worldCount_pairs.add((iterations, worlds))

            playerStatisticsFileName = os.path.join(inputTournamentDir, dir, PLAYER_STATISTICS_CSV_FILENAME)
            playerStatistics = pd.read_csv(playerStatisticsFileName)
            if expl not in winCountsByConfig:
                winCountsByConfig[expl] = [(worlds, playerStatistics.iloc[0][PLAYER_STATISTICS_WIN_COUNT_COL])]
            else:
                winCountsByConfig[expl].append((worlds, playerStatistics.iloc[0][PLAYER_STATISTICS_WIN_COUNT_COL]))


            

        plt.xlabel("Iteration count / Determinizing world count")
        plt.ylabel("Win count in 100 games")
        
        x_data = [f"it={it}\nworlds={worldCount}" for it, worldCount in sorted(it_worldCount_pairs, key = lambda x: int(x[1]))]
        plt.title(f"Playout {playoutStrategies[currSubplotInd]}")
        legend = []
        
        

        highest_win_count_by_expl = []
        y_datas = []
        for expl, dataByExpl in sorted(winCountsByConfig.items(), key = lambda x: x):
            sortedDataByExpl = sorted(dataByExpl, key = lambda x: int(x[0]))
            y_data = [winCount for worlds, winCount in sortedDataByExpl]
            print(f"Expl={expl} -> {y_data}")
            if len(highest_win_count_by_expl) == 0: 
                highest_win_count_by_expl = [(expl,winCount) for worlds, winCount in sortedDataByExpl]
            else:
                ind = 0
                for pastExpl, pastWinCount in highest_win_count_by_expl:
                    if pastWinCount < sortedDataByExpl[ind][1]:
                        highest_win_count_by_expl[ind] = (expl, sortedDataByExpl[ind][1])
                    ind += 1


            y_datas.append(y_data)
            #print(f"Expl={expl} -> {sorted(dataByExpl, key = lambda x: int(x[0]))}")
            #print(y_data)
            legend.append(f"Exploration constant = {expl}")
            #plt.plot(x_data, y_data)

        for i, (expl, v) in enumerate(highest_win_count_by_expl):
            plt.text(i, v / 2, f"Expl\n{expl}", ha='center', va='center')

        print(f"Best data: {highest_win_count_by_expl}")
        average_config = np.mean(y_datas, axis=0)
    # legend.append("Average config")
        plt.bar(x_data, [winCount for expl, winCount in highest_win_count_by_expl])
    # plt.plot(x_data, average_config)

        #plt.legend(legend)

        plt.gcf().set_size_inches(15, 9)

        if len(savePath) > 0:
            plt.savefig(f"{savePath}_{playoutStrategies[currSubplotInd]}.pdf")

        if not noShow:
            plt.show()
        else:
            plt.close()
        
        currSubplotInd += 1



visualize_exploration_constant_experiment_results([sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]], False, "mcts_expl_worldcount") #"doc/Thesis/img/mcts_experiment"
