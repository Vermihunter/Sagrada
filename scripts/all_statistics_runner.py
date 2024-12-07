#!/usr/bin/env python3

import argparse
import json
import sys
from datetime import datetime
from sagrada_tournament import *
from move_statistics_visualizer import *

THESIS_IMG_OUTPUT_DIR_KEY = "OutputDir"

TOURNAMENT_CONFIG_GAME_COUNT_KEY = "GameCount"
TOURNAMENT_CONFIG_TOURNAMENT_RESULT_KEY = "TournamentResultDir"
TOURNAMENT_CONFIG_PLAYERS_KEY = "Players"
TOURNAMENT_CONFIG_OTHER_OPTIONS_KEY = "OtherOptions"
TOURNAMENT_CONFIG_TOURNAMENTS_KEY = "Tournaments"

PLOT_OUTPUT_PATH_KEY = "RelativePath"

RANDOM_PLAYERS_BRANCHING_FACTOR_PLOT_KEY = "RandomAgentsBranchingFactorPlotData"
MINIMAX_PLAYER_BRANCHING_FACTOR_PLOT_KEY= "MinimaxAgentBranchingFactorPlotData"
RULES_BASED_PLAYERS_BRANCHING_FACTOR_PLOT_KEY = "RulesBasedBothStrategiesBranchingFactorPlotData"
MCTS_PLAYERS_BRANCHING_FACTOR_PLOT_KEY = "MCTSBranchingFactorPlotData"
HEURISTIC_FILTER_BRANCHING_FACTOR_PLOT_KEY = "HeuristicFilterShowcase"
MCTS_EXPLORATION_FIRST_KEY = "MCTSExplorationFactorPlayoutFirst"
MCTS_EXPLORATION_RB_ONLY_DTFM_KEY = "MCTSExplorationFactorPlayoutRulesBasedOnlyDTFM"
MCTS_EXPLORATION_RB_ALL_MOEVS_KEY ="MCTSExplorationFactorPlayoutRulesBasedAllMoves"
MCTS_EXPLORATION_MINIMAX_KEY = "MCTSExplorationFactorPlayoutMinimax1"
FINAL_RESULTS_KEY = "FinalResultsTournamentsDataKey"
DETERMINISTIC_FINAL_RESULTS_KEY = "DeterministicResultsTournamentsDataKey"


def task_wrapper(func, args):
    """
    Wrapper function to call the task function with arguments.
    """
    return func(*args)

class TaskManager:
    def __init__(self, max_workers=None):
        self.max_workers = max_workers or multiprocessing.cpu_count()
        self.pool = multiprocessing.Pool(processes=self.max_workers)

    def run_tasks(self, tasks):
        """
        Execute a list of tasks.
        :param tasks: A list of tuples, each containing a task function and its arguments.
        """
        results = self.pool.starmap(task_wrapper, tasks)
        return results
    
TASKS = [
    []
]

def tournament_config_from_json(tournament_json_config, verbose: bool) ->TournamentArgs:
    tournament_args = []
    for tournament_config in tournament_json_config[TOURNAMENT_CONFIG_TOURNAMENTS_KEY]:
        tournament_args.append(TournamentArgs(
            DEFAULT_SEED, 
            tournament_config[TOURNAMENT_CONFIG_GAME_COUNT_KEY], 
            tournament_config[TOURNAMENT_CONFIG_TOURNAMENT_RESULT_KEY],
            tournament_config[TOURNAMENT_CONFIG_PLAYERS_KEY],
            "default",
            tournament_config[TOURNAMENT_CONFIG_OTHER_OPTIONS_KEY] if TOURNAMENT_CONFIG_OTHER_OPTIONS_KEY in tournament_config else "",
            verbose
        ))

    return tournament_args

def get_branching_factor_plot_tasks_for(data: json, verbose: bool, outputDir: str, agentName: str, showFilteredForPlayers: list, mode: str, dontShowPlayers: list):
    tournaments = [(run_tournaments, [
        tournament_config.gameCount, tournament_config.seed, tournament_config.outputPath, 
        tournament_config.players, tournament_config.gameMode, tournament_config.otherOptions, verbose]) 
        for tournament_config in tournament_config_from_json(data[agentName], verbose)
    ]
    
    # tournaments.append((visualize_data, [
    #     [f"{TOURNAMENT_RESULT_DIR}/{tournamentConfig[TOURNAMENT_CONFIG_TOURNAMENT_RESULT_KEY]}" for tournamentConfig in data[agentName][TOURNAMENT_CONFIG_TOURNAMENTS_KEY]], 
    #     mode,
    #     f"{outputDir}/{data[agentName][PLOT_OUTPUT_PATH_KEY]}", 
    #     True,
    #     showFilteredForPlayers,
    #     dontShowPlayers
    # ]))

    return tournaments

def main():
    parser = argparse.ArgumentParser( 
        description ='Re-runs all the experiments that are part of the Thesis\' text and re-generates all the plots'
    )
    
    parser.add_argument("-i", help = "The input config file that contains all the paths and configs required for re-generating the process", required = True)
    parser.add_argument("-v", help = "Makes the output verbose", action='store_true', default = False)
    args = parser.parse_args()

    if not args.i.lower().endswith('.json'):
        print("The config file must be in JSON format!", file=sys.stderr)
        exit(1)

    with open(args.i, 'r') as file:
        # Load the JSON data
        data = json.load(file)

    outputDir = data[THESIS_IMG_OUTPUT_DIR_KEY]

    tasks = [
        
        #*get_branching_factor_plot_tasks_for(data, args.v, outputDir, FINAL_RESULTS_KEY, [], MODE_SEPARATE, []),
        *get_branching_factor_plot_tasks_for(data, args.v, outputDir, MCTS_EXPLORATION_FIRST_KEY, [], MODE_SEPARATE, []),
        *get_branching_factor_plot_tasks_for(data, args.v, outputDir, MCTS_EXPLORATION_RB_ONLY_DTFM_KEY, [], MODE_SEPARATE, []),
        *get_branching_factor_plot_tasks_for(data, args.v, outputDir, MCTS_EXPLORATION_RB_ALL_MOEVS_KEY, [], MODE_SEPARATE, []),
        *get_branching_factor_plot_tasks_for(data, args.v, outputDir, MCTS_EXPLORATION_MINIMAX_KEY, [], MODE_SEPARATE, []),
        #*get_branching_factor_plot_tasks_for(data, args.v, outputDir, RANDOM_PLAYERS_BRANCHING_FACTOR_PLOT_KEY, [], MODE_SEPARATE, ["First"]),
        # *get_branching_factor_plot_tasks_for(data, args.v, outputDir, HEURISTIC_FILTER_BRANCHING_FACTOR_PLOT_KEY, ["MCTS70,expl=0.60,worlds=3,playout=RulesBased_ONLYDTFM", "Minimax2,worlds=4"], MODE_SEPARATE, []),
        #*get_branching_factor_plot_tasks_for(data, args.v, outputDir, MCTS_PLAYERS_BRANCHING_FACTOR_PLOT_KEY, [], MODE_SEPARATE, ["Minimax3,worlds=1"])
        # *get_branching_factor_plot_tasks_for(data, args.v, outputDir, MINIMAX_PLAYER_BRANCHING_FACTOR_PLOT_KEY, ["Minimax2,worlds=4"], MODE_SEPARATE, []),
        # *get_branching_factor_plot_tasks_for(data, args.v, outputDir, RULES_BASED_PLAYERS_BRANCHING_FACTOR_PLOT_KEY, [], MODE_SEPARATE, [])
        #*get_branching_factor_plot_tasks_for(data, args.v, outputDir, DETERMINISTIC_FINAL_RESULTS_KEY, [], MODE_SEPARATE, [])
    ]


    for task_f, task_args in tasks:
        start = datetime.now()
        print(f"Starting task \"{task_f.__name__}\" with arguments {task_args[2]} at {start.strftime('%H:%M:%S')}")
        task_f(*task_args)
        task_duration = datetime.now() - start
        print(f"Task duration: {str(task_duration)}")

if __name__ == "__main__":
    main()