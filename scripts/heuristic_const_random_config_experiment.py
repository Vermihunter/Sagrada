#!/usr/bin/env python3

import json
import shutil
import random
import argparse
import sagrada_tournament

def main():
    parser = argparse.ArgumentParser( 
        description ='Minimax hyperparameter experimenter - constructs configs from random hyperparameters and runs a tournament with that configuration'
    )
    

    parser.add_argument("-o", help = "The output file where the results are stored", required = True)
    parser.add_argument("-e", help = "The number of experiments to run", required=True, type=int)
    parser.add_argument("-n", help = "The number of games in each tournament", type=int, default=112)
    parser.add_argument("-s", help = "The starting sead of each tournament", type=int, default=779)
    parser.add_argument("-v", help = "Makes the output verbose", action='store_true', default = False)
    parser.add_argument("-p", help = "Other player(s) that compete against the minimax player", nargs='*', default=["first"])
    
    args = parser.parse_args()
    random.seed(779)

    config_file_name = "minimax_config.json"
    args.p.append(f"minimax-depth=2,worlds=8,config_file={config_file_name}")
    experimentPlayers = args.p

    for exp_ind in range(1, args.e + 1):
        OpponentInfluencingFactor = (random.random() * 10) + 0.01
        PuocCompletablePower = (random.random() * 10) + 0.01
        MinusPointsPerUncompletableField = random.randint(10, 1000)
        CompletedPointsWeight =  random.randint(1, 500)
        MinusPointsPerUncompletablePuocPoints = random.randint(1, 100)

        tournament_dir = f"{args.o}/tournament_{exp_ind}"
        
        json_config = {
            "OpponentInfluencingFactor": OpponentInfluencingFactor,
            "PuocCompletablePower": PuocCompletablePower,
            "MinusPointsPerUncompletableField": MinusPointsPerUncompletableField,
            "CompletedPointsWeight": CompletedPointsWeight,
            "MinusPointsPerUncompletablePuocPoints": MinusPointsPerUncompletablePuocPoints,
        }

        with open(config_file_name, "w") as file:
            file.write(json.dumps(json_config))

        
        sagrada_tournament.run_tournaments(args.n, args.s, tournament_dir, experimentPlayers, "", "", False)
        shutil.move(config_file_name, f"tournament_results/{tournament_dir}")

        if args.v:
            print(f"Tournament {exp_ind} has finished!")

if __name__ == "__main__":
    main()