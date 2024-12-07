#!/usr/bin/env python3

import argparse
import os
import pandas as pd
from statsmodels.stats.proportion import *
from tournaments_config import *
from player_statistics import PLAYER_STATISTICS_WIN_COUNT_COL
from tournament_runner import *
from game_result import CSV_SEED_COL_NAME

def run_final_results(inputDir: str, steps: int, maxGameCount: int, confMin: float):

    for tournamentDir in os.listdir(inputDir):
        players = tournamentDir.split("_vs_", 1)
        
        fullTournamentDir = os.path.join(inputDir, tournamentDir)
        playerStatisticsFileName = os.path.join(fullTournamentDir, PLAYER_STATISTICS_CSV_FILENAME)
        gameResultsFileName = os.path.join(fullTournamentDir, GAME_RESULT_CSV_FILENAME)
    
        while True:
            playerStatistics = pd.read_csv(playerStatisticsFileName)

            firstPlayerWinCount = playerStatistics.iloc[0][PLAYER_STATISTICS_WIN_COUNT_COL]
            secondPlayerWinCount = playerStatistics.iloc[1][PLAYER_STATISTICS_WIN_COUNT_COL]

            totalGameCount = firstPlayerWinCount + secondPlayerWinCount

            first_wilson_lower_bound, first_wilson_upper_bound = proportion_confint(firstPlayerWinCount, totalGameCount, 0.02, 'wilson')
            second_wilson_lower_bound, second_wilson_upper_bound = proportion_confint(secondPlayerWinCount, totalGameCount, 0.02, 'wilson')

            gameResults = pd.read_csv(gameResultsFileName)
            maxSeed = gameResults[CSV_SEED_COL_NAME].max()
            
            if first_wilson_lower_bound < confMin and second_wilson_lower_bound < confMin and totalGameCount < maxGameCount:
                print(f"Running {steps} games between {players} {firstPlayerWinCount}({format(first_wilson_lower_bound*100,'.1f')}-{format(first_wilson_upper_bound*100,'.1f')}) - {secondPlayerWinCount}({format(second_wilson_lower_bound*100,'.1f')}-{format(second_wilson_upper_bound*100,'.1f')}) max seed = {maxSeed}")
            
                tmpDirName = f"{tournamentDir}_tmp"
                run_tournaments(steps, maxSeed + 5, tmpDirName, players, "default", "-b", False)
                tmpConfigs = [
                    [TournamentArgs(0, 0, fullTournamentDir, [], "", "", False)],
                    [TournamentArgs(0, 0, f"{TOURNAMENT_RESULT_DIR}/{tmpDirName}", [], "", "", False)],
                ]
                combine_results(tmpConfigs, totalGameCount + steps, f"final_results_large/{tournamentDir}")
            else:
                print(f"Finished - {players}")
                break

        
        

def main():
    parser = argparse.ArgumentParser( 
        description ='Displays a diagram about move statistics from a directory full of tournament results'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournaments results", required = True)
    parser.add_argument("-step", help="Game count that each iteration adds", type=int, default=160)
    parser.add_argument("-max_games", help="Upper bound for the total number of games", default=2222)
    parser.add_argument("-conf_min",help="The minimum required confidence interval lower bound to achieve", default=0.5)

    args = parser.parse_args()

    run_final_results(args.i, args.step, args.max_games, args.conf_min)

if __name__ == "__main__":
    main()