#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import RadioButtons
import re
import argparse
from tournament_statistics import *

DATA_SOURCE_MEANING_RADIO_BUTTON_COORDINATES = [0.35, 0.85, 0.3, 0.15]
POSSIBLE_DATA_SOURCES = [TOURNAMENTS_WON_KEY, TOTAL_GAMES_WON_KEY]

MINIMAX_NAME = "minimax"
MINIMAX_DETERMINISTIC_NAME = "Minimax_d"
MINIMAX_NON_DETERMINISTIC_NAME = "Minimax"

MCTS_NAME = "mcts"
MCTS_DETERMINISTIC_NAME = "MCTS_d"
MCTS_NON_DETERMINISTIC_NAME = "MCTS"

DET_NAME_IND = 0
NON_DET_NAME_IND = 1
PLAYER_NAMES = {
    MINIMAX_NAME: [MINIMAX_DETERMINISTIC_NAME, MINIMAX_NON_DETERMINISTIC_NAME],
    MCTS_NAME: [MCTS_DETERMINISTIC_NAME, MCTS_NON_DETERMINISTIC_NAME]
}


DEFAULT_NON_DETERMINISTIC_PLAYER = MINIMAX_NON_DETERMINISTIC_NAME
DEFAULT_DETERMINISTIC_PLAYER = MINIMAX_DETERMINISTIC_NAME

NUMBER_REGEX = r"(\d+)"
FLOAT_REGEX = r"(\d+(\.\d+)?)"
ANYTHING_REGEX = r"(.*)"

REGEX_KEY = "regex"
ATTRIBUTE_KEY = "attributes"

DEPTH_KEY = "depth"
WORLDS_KEY = "worlds"

ITERATIONS_KEY = "iterations"
EXPL_KEY = "expl"
PLAYOUT_KEY = "playout"


players = {
    MINIMAX_DETERMINISTIC_NAME: {
        REGEX_KEY: r"Minimax{}$",
        ATTRIBUTE_KEY: {
            DEPTH_KEY: NUMBER_REGEX
        }
    },

    MINIMAX_NON_DETERMINISTIC_NAME: {
        REGEX_KEY: r"Minimax{},worlds={}$",
        ATTRIBUTE_KEY: {
            DEPTH_KEY: NUMBER_REGEX,
            WORLDS_KEY: NUMBER_REGEX
        }
    },

    MCTS_DETERMINISTIC_NAME: {
        REGEX_KEY: r"MCTS{},expl={},playout={}$",
        ATTRIBUTE_KEY: {
            ITERATIONS_KEY: NUMBER_REGEX,
            EXPL_KEY: FLOAT_REGEX,
            PLAYOUT_KEY: ANYTHING_REGEX
        }
    },

    MCTS_NON_DETERMINISTIC_NAME: {
        REGEX_KEY: r"MCTS{},expl={},worlds={},playout={}$",
        ATTRIBUTE_KEY: {
            ITERATIONS_KEY: NUMBER_REGEX,
            EXPL_KEY: FLOAT_REGEX,
            WORLDS_KEY: NUMBER_REGEX,
            PLAYOUT_KEY: ANYTHING_REGEX
        }
    }
}


def player_configurator(args):
    player_name_ind = DET_NAME_IND if args.d else NON_DET_NAME_IND
    player_name = PLAYER_NAMES[args.subcommand][player_name_ind]
    player_attributes = players[player_name][ATTRIBUTE_KEY]
    
    attr_present = 0
    for attr_name in player_attributes:
        if getattr(args, attr_name) is not None:
            player_attributes[attr_name] = getattr(args, attr_name)
            attr_present += 1
        else:
            attr_changing = attr_name
            
    if attr_present != len(player_attributes.keys()) - 1:
        raise AttributeError("Every attribute except for one must be fixed for the statistics")

    return player_attributes, player_name, attr_changing
            
            

def on_data_meaning_source_changed(label, ax, changing_attribute_name, player_statistics_data):
    sorted_data = sorted(player_statistics_data, key=lambda x: x[2])
    # Sample data
    x = [changing_attribute for  _, _, changing_attribute in sorted_data]#  [1, 2, 3, 4, 5]
    y = [player_attributes[label] for _, player_attributes, _ in sorted_data]
     
    # Create a line plot    
    ax.clear()
    ax.plot(x, y)

    # Add labels     
    ax.set_xlabel(changing_attribute_name)
    ax.set_ylabel(label)
    
    plt.draw()
    
    

def main():
    parser = argparse.ArgumentParser( 
        prog = 'tournament_statistics',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description ='Displays the results of a given player with one attribute changing'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournament results", required = True)
    parser.add_argument("-d", help = "Flag marking the tournaments deterministic in the input directory", default = False, action='store_true')

    subparsers = parser.add_subparsers(title="Player configs", help = "", dest="subcommand")

    minimax_parser = subparsers.add_parser(MINIMAX_NAME, help="Minimax attribute value configurator")
    minimax_parser.add_argument(f"-{DEPTH_KEY}", type=int, help="Depth of the minimax search")
    minimax_parser.add_argument(f"-{WORLDS_KEY}", type=int, help="Determinizing world count")

    mcts_parser = subparsers.add_parser(MCTS_NAME, help="MCTS attribute value configurator")
    mcts_parser.add_argument(f"-{ITERATIONS_KEY}", type=int, help="MCTS iteration count")
    mcts_parser.add_argument(f"-{EXPL_KEY}", type=str, help="Exploration constant fixer")
    mcts_parser.add_argument(f"-{WORLDS_KEY}", type=int, help="Determinizing world count")
    mcts_parser.add_argument(f"-{PLAYOUT_KEY}", type=str, help="Playout strategy of the player")

    args = parser.parse_args()

    if args.subcommand is None:
        print("A player must be configured")
        parser.print_help()
        exit(1)

    player_search_attributes, player_name, changing_attribute_name = player_configurator(args)
    
    # Construct the regex that filters the players to be visualized
    base_regex = players[player_name][REGEX_KEY]
    player_regex = re.compile(base_regex.format(*player_search_attributes.values()))

    tournamentResults = collect_data_from_dir(args.i)

    # Filter the players according to the choice to visualize
    filtered_players_statistics = []
    for filtered_player_name, filtered_player_statistics in tournamentResults.player_statistics.items():
        match = re.match(player_regex, filtered_player_name)
        if match:
            filtered_players_statistics.append([filtered_player_name, filtered_player_statistics, match.group(1)])
            
        
        
   # Create a figure and axis
    fig, ax = plt.subplots()
    
    # Changing the selected attribute to visualize the title in a more user friendly way
    player_search_attributes[changing_attribute_name] = "{}"
    player_regex = re.compile(base_regex.format(*player_search_attributes.values()))
    
    fig.canvas.manager.set_window_title(player_regex.pattern[:-1])
    
    # Default config
    on_data_meaning_source_changed(POSSIBLE_DATA_SOURCES[1], ax, changing_attribute_name, filtered_players_statistics)
    
   # data_meaning_sources = RadioButtons(plt.axes(DATA_SOURCE_MEANING_RADIO_BUTTON_COORDINATES), POSSIBLE_DATA_SOURCES, active=0)
   # data_meaning_sources.on_clicked(lambda label: on_data_meaning_source_changed(label, ax, changing_attribute_name, filtered_players_statistics))
    
    plt.show()

    

if __name__ == "__main__":
   main()



