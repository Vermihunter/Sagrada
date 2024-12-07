#!/usr/bin/env python3

import multiprocessing
import subprocess
import pandas as pd
import statsmodels
from statsmodels.stats.proportion import *
import shutil
import os
from player_statistics import PlayerStatistics, construct_for_tournament
from game_result import GameResult
from tournaments_config import *

# CONFIG
TOURNAMENT_EXE_PATH = "./build_opt/tournament"
TOURNAMENT_RESULT_DIR = "./tournament_results"
DEFAULT_SEED = 779


class TournamentArgs:
    def __init__(self, seed: int, gameCount: int, outputPath: str, players: list, gameMode: str, otherOptions: str = "", verbose = True):
        self.seed = seed
        self.gameCount = gameCount
        self.outputPath = outputPath
        self.players = players
        self.gameMode = gameMode
        self.otherOptions = otherOptions
        self.verbose = verbose

    def __str__(self):
        return f"seed = {self.seed}, game count = {self.gameCount}"


def run_tournament(args: TournamentArgs):
    run_args = f'{TOURNAMENT_EXE_PATH} -n {args.gameCount} -s {args.seed} -csv {args.outputPath} -p {" ".join(args.players)} -mode {args.gameMode} {args.otherOptions}'
    if args.verbose:
        print(run_args)

    subprocess.call(run_args, shell=True)

def combine_csv_files(files: list) -> pd.DataFrame:
    combined_data = pd.DataFrame()

    # Loop through the CSV files and concat their contents to the combined_data DataFrame
    for file in files:
        if not os.path.exists(file):
            continue

        df = pd.read_csv(file)
        combined_data = pd.concat([combined_data, df], ignore_index=True)

    return combined_data


def combine_results(allConfigs: list, gameCount: int, csvDirName: str):
    tournament_game_subresults_files = [f'{config[0].outputPath}/{GAME_RESULT_CSV_FILENAME}' for config in allConfigs]
    tournament_game_substatistics_files = [f'{config[0].outputPath}/{GAME_STATISTICS_CSV_FILENAME}' for config in allConfigs]
    tournament_game_subplayerstatistics_files = [f'{config[0].outputPath}/{PLAYER_STATISTICS_CSV_FILENAME}' for config in allConfigs]
    tournament_game_move_statistics_files = [f'{config[0].outputPath}/{MOVE_STATISTICS_CSV_FILENAME}' for config in allConfigs]

    # saving all the game results into one file
    combined_game_subresults_data = combine_csv_files(tournament_game_subresults_files)
    combined_game_subresults_data.to_csv(f'{TOURNAMENT_RESULT_DIR}/{csvDirName}/{GAME_RESULT_CSV_FILENAME}', index=False)

    combined_move_statistics_data = combine_csv_files(tournament_game_move_statistics_files)
    if len(combined_move_statistics_data) > 0:
        combined_move_statistics_data.to_csv(f'{TOURNAMENT_RESULT_DIR}/{csvDirName}/{MOVE_STATISTICS_CSV_FILENAME}', index=False)

    
    player_statistics, result_df = construct_for_tournament(tournament_game_subplayerstatistics_files, combined_game_subresults_data, gameCount)

    # Save the result to a new CSV file
    result_df.to_csv(f'{TOURNAMENT_RESULT_DIR}/{csvDirName}/{PLAYER_STATISTICS_CSV_FILENAME}', index=False, float_format='%.4f')
    
    # getting to winner to calculate wilson's score interval
    winner_player_type, winner_player_stat = max(player_statistics.items(), key=lambda item: item[1].winCount)
    wilson_lower_bound, wilson_upper_bound = proportion_confint(winner_player_stat.winCount, gameCount, 0.02, 'wilson')

    game_statistics_df = pd.DataFrame(
        {
            'Wilson interval lower bound in %': [wilson_lower_bound * 100],
            'Wilson interval upper bound in %': [wilson_upper_bound * 100],
            'Wilson interval confidence level': [98],
            'Winner player type': [winner_player_type]
        }
    )

    for config in allConfigs:
        if config[0].outputPath == f'{TOURNAMENT_RESULT_DIR}/{csvDirName}':
            continue

        shutil.rmtree(config[0].outputPath, ignore_errors=False, onerror=None)

    game_statistics_df.to_csv(f'{TOURNAMENT_RESULT_DIR}/{csvDirName}/{GAME_STATISTICS_CSV_FILENAME}', index=False, float_format='%.4f')


    

def run_tournaments(gameCount: int, seed: int, csvDir: str, players: list, gameMode: str, otherOptions: str, verbose: bool):
    if not os.path.exists(TOURNAMENT_EXE_PATH):
        print(f"Tournament exe not found on path {TOURNAMENT_EXE_PATH}")
        return

    thread_count = min(gameCount, multiprocessing.cpu_count())
    gamesPerThread = int(gameCount / thread_count)

    run_args = [[TournamentArgs(seed + i * gamesPerThread, gamesPerThread, f'{TOURNAMENT_RESULT_DIR}/{csvDir}/tmp_subtournament_{i + 1}', players, gameMode, otherOptions, verbose)] for i in range(thread_count)]
    currIncr = 0
    for ind in range(thread_count):
        if ind != 0:
            run_args[ind][0].seed += currIncr
            
        if ind < (gameCount % thread_count):
            run_args[ind][0].gameCount += 1
            currIncr += 1

    processes = [multiprocessing.Process(target = run_tournament, args = (run_args[i])) for i in range(thread_count)]

    for process in processes:
        process.start()

    for process in processes:
        process.join()
    
    combine_results(run_args, gameCount, csvDir)