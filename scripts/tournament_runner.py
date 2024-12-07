#!/usr/bin/env python3

import argparse
import re
from sagrada_tournament import *


def validate_player_type(enteredPlayerType: str):
    playerTypeRegexes = [
        r"^random$",
        r"^first$",
        r"rules-based(?:-strategy=(all_moves|only_dtfm))$",
        r"^minimax-depth=(\d+)(?:,worlds=(\d+))?(?:,config_file=([^,]+))?$",
        r"^mcts-it=(\d+)(?:,expl=(\d+).(\d+))(?:,worlds=(\d+))?(?:,playout=(.+))?$"
    ]

    playerTypeCorrect = False
    for playerTypeRegex in playerTypeRegexes:
        if re.match(playerTypeRegex, enteredPlayerType):
            playerTypeCorrect = True
            break

    if not playerTypeCorrect:
        raise argparse.ArgumentTypeError(f"Invalid player type: '{enteredPlayerType}'")
    
    return enteredPlayerType

def main():
    example_text = """Example usage:
    ./tournament_runner.py -csv new_minimax_config_experiment -n 150 -p random minimax-depth=2,config_file=minimax_config.json
    ./tournament_runner.py -csv mcts_results -n 200 -p mcts-it=200,playout=minimax-depth=1 mcts-it=300,playout=rules-based
    ./tournament_runner.py -csv huge -n 10000 -s 1234 -p first random
    """

    parser = argparse.ArgumentParser( 
        prog = 'sagrada_tournament_runner',
        epilog = example_text,
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description ='Runs a tournament paralelly (divided into smaller tournaments)'
    )
    
    parser.add_argument("-csv", help = "The output directory for the .csv results", required = True)
    parser.add_argument("-n", help = "The number of games to run in the tournament", required = True, type = int)
    parser.add_argument("-s", help = "The starting seed for the tournament", default = DEFAULT_SEED, type = int)
    parser.add_argument("-m", help = "The mode of the game, default value is \"default\"", type = str, default = "default")
    parser.add_argument("-d", help = "Makes the game deterministic", action='store_true')
    parser.add_argument("-b", help = "Adds the -b flag to the tournament", action='store_true')
    parser.add_argument("-ms", help = "Adds the -ms flag to the tournament providing move statistics about the games", action='store_true')
    parser.add_argument("-v", help = "Makes the output verbose", action='store_true', default = False)
    parser.add_argument("-p", required = True, nargs='+', type = validate_player_type,
        help = "The player types of the tournament. Defined players: [\"random\", \"first\", \"rules-based\", \"minimax-depth=[UINT](,worlds=[UINT])(,config_file=[CONFIG_PATH.json])\", \"mcts-it=[UINT](,worlds=[UINT])(,playout=[PLAYOUT_STRATEGY])\")]")
    
    args = parser.parse_args()

    otherOptions = f'{("-d" if args.d else "")} {("-b" if args.b else "")} {("-ms" if args.ms else "")}'
    run_tournaments(args.n, args.s, args.csv, args.p, args.m, otherOptions, args.v)



if __name__ == '__main__':
    main()