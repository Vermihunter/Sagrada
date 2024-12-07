#!/usr/bin/env python3

from sagrada_tournament import *
import itertools

GAME_COUNT = 100
START_SEED = 779
GAME_MODE = "default"
OUTPUT_DIR = "04_03_mcts_strength_experiment"
GAME_OPTIONS = ""


MIN_EXPLORATION = 0.5
EXPLORATION_COUNT = 50

MCTS_EXPLORATION_PLAYERS = [f"mcts-it=70,expl={format(MIN_EXPLORATION + float(explr_ind) * 0.1, '.2f')},worlds=4,playout=rules-based-strategy=only_dtfm" for explr_ind in range(EXPLORATION_COUNT)]

DETERMINISTIC_PLAYER_TYPES = [
     # Minimax
    "minimax-depth=1",
    "minimax-depth=2",
    "minimax-depth=3",
    "minimax-depth=4",

    # MCTS -> Minimax
    "mcts-it=50,playout=minimax-depth=1",
    "mcts-it=70,playout=minimax-depth=1",
    "mcts-it=100,playout=minimax-depth=1",
    "mcts-it=120,playout=minimax-depth=1",
    "mcts-it=150,playout=minimax-depth=1",
    "mcts-it=200,playout=minimax-depth=1",
    "mcts-it=400,playout=minimax-depth=1",

    # MCTS -> rules based only dtfm
    "mcts-it=50,playout=rules-based-strategy=only_dtfm",
    "mcts-it=70,playout=rules-based-strategy=only_dtfm",
    "mcts-it=100,playout=rules-based-strategy=only_dtfm",
    "mcts-it=120,playout=rules-based-strategy=only_dtfm",
    "mcts-it=150,playout=rules-based-strategy=only_dtfm",
    "mcts-it=200,playout=rules-based-strategy=only_dtfm",
    "mcts-it=400,playout=rules-based-strategy=only_dtfm",

    # MCTS -> rules based all movs
    "mcts-it=50,playout=rules-based-strategy=all_moves",
    "mcts-it=70,playout=rules-based-strategy=all_moves",
    "mcts-it=100,playout=rules-based-strategy=all_moves",
    "mcts-it=120,playout=rules-based-strategy=all_moves",
    "mcts-it=150,playout=rules-based-strategy=all_moves",
    "mcts-it=200,playout=rules-based-strategy=all_moves",
    "mcts-it=400,playout=rules-based-strategy=all_moves",

]

EXPLR_CONST = "2.0"

PLAYER_TYPES1 = [
    #"first",
    #"random",

    
    "minimax-depth=1,worlds=1",
    "minimax-depth=2,worlds=1",
    "minimax-depth=2,worlds=2",
    "minimax-depth=2,worlds=4",
    "minimax-depth=2,worlds=16",
    "minimax-depth=2,worlds=32",

    "minimax-depth=3,worlds=1",
    "minimax-depth=3,worlds=2",
    "minimax-depth=3,worlds=3",
    "minimax-depth=3,worlds=4",
    "minimax-depth=3,worlds=8",
    "minimax-depth=4,worlds=1",
    "minimax-depth=4,worlds=4",
    "minimax-depth=4,worlds=6",
#   "minimax-depth=5,worlds=1",
#    "minimax-depth=5,worlds=4",

    f"mcts-it=50,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=50,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=50,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=70,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=70,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=70,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",

    f"mcts-it=100,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=120,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=120,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=120,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=1,playout=minimax-depth=1",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=3,playout=minimax-depth=1",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=5,playout=minimax-depth=1",
    #"mcts-it=100,playout=minimax-depth=1",
    #"mcts-it=150,playout=minimax-depth=1",
    #"mcts-it=200,playout=minimax-depth=1",

    f"mcts-it=75,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=75,expl={EXPLR_CONST},worlds=4,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=75,expl={EXPLR_CONST},worlds=8,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",


    f"mcts-it=75,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=75,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=75,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=100,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    

    f"mcts-it=125,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=125,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=125,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    
    f"mcts-it=150,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=150,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=all_moves",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=all_moves",
    f"mcts-it=400,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=all_moves",
    

    #"mcts-it=100,playout=rules-based-strategy=all_moves",
    #"mcts-it=150,playout=rules-based-strategy=all_moves",
    #"mcts-it=200,playout=rules-based-strategy=all_moves",

    f"rules-based-strategy=only_dtfm",
    f"rules-based-strategy=all_moves",
]

PLAYER_TYPES1 = [
    f"minimax-depth=4,worlds=4"
]

PLAYER_TYPES2 = [
   # f"mcts-it=70,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
   # f"mcts-it=70,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=70,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=70,expl={EXPLR_CONST},worlds=10,playout=rules-based-strategy=only_dtfm",
    
    
    f"mcts-it=125,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=125,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=125,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=125,expl={EXPLR_CONST},worlds=10,playout=rules-based-strategy=only_dtfm",
    
    f"mcts-it=200,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=200,expl={EXPLR_CONST},worlds=10,playout=rules-based-strategy=only_dtfm",
    
    f"mcts-it=300,expl={EXPLR_CONST},worlds=1,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=3,playout=rules-based-strategy=only_dtfm",
    f"mcts-it=300,expl={EXPLR_CONST},worlds=5,playout=rules-based-strategy=only_dtfm"
]

def main():
    ind = 0
    jump_ind = 0
    for player1, player2 in itertools.product(PLAYER_TYPES1, PLAYER_TYPES2):
        # if player1 >= player2:
        #     continue

        if ind < jump_ind:
            ind += 1
            continue

        output_path = f'{OUTPUT_DIR}/{player1}_vs_{player2}'
        print(f"Running tournament {player1} vs {player2}")
        run_tournaments(GAME_COUNT, START_SEED, output_path, [player1, player2], GAME_MODE, GAME_OPTIONS, False)

    # for mcts_player in MCTS_EXPLORATION_PLAYERS:
    #     player2 = "minimax-depth=2,worlds=8"
    #     output_path = f'{OUTPUT_DIR}/{mcts_player}_vs_{player2}'
    #     print(f"Running tournament {mcts_player} vs {player2}")
    #     run_tournaments(GAME_COUNT, START_SEED, output_path, [mcts_player, player2], GAME_MODE, GAME_OPTIONS, False)
        
    


if __name__ == "__main__":
    main()