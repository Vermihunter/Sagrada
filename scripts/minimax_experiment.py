#!/usr/bin/env python3

import itertools
import json
import shutil
import sagrada_tournament

def main():
    experimentGameCount = 150
    experimentPlayers = ["first", "minimax-depth=2,config_file=minimax_config.json"]
    experimentSeed = 779
    experimentDir = "reworked_backend_experiment"
    gameMode = "default"

    OpponentInfluencingFactor = [ 3.5 + x / 10 for x in range(10 + 1)] # ~4 double
    PuocCompletablePower = [ 1.75 + x / 10 for x in range(5)] # ~2 double
    MinusPointsPerUncompletableField = [ 120 + x * 10 for x in range(10 + 1)] # ~150 int
    CompletedPointsWeight = [ 20 ] # ~20 int
    MinusPointsPerUncompletablePuocPoints = [4] # ~4 int

    exp_ind = 0
    for config in itertools.product(OpponentInfluencingFactor, PuocCompletablePower, MinusPointsPerUncompletableField, CompletedPointsWeight, MinusPointsPerUncompletablePuocPoints):
        
        print(f"{exp_ind}: {config}")
        
        config_file_name = "minimax_config.json"
        tournament_dir = f"{experimentDir}/tournament_{exp_ind}"
        
        json_config = {
            "OpponentInfluencingFactor": config[0],
            "PuocCompletablePower": config[1],
            "MinusPointsPerUncompletableField": config[2],
            "CompletedPointsWeight": config[3],
            "MinusPointsPerUncompletablePuocPoints": config[4],
        }

        with open(config_file_name, "w") as file:
            file.write(json.dumps(json_config))

        
        sagrada_tournament.run_tournaments(experimentGameCount, experimentSeed, tournament_dir, experimentPlayers, gameMode)

        shutil.move(config_file_name, f"tournament_results/{tournament_dir}")
        exp_ind = exp_ind + 1

if __name__ == "__main__":
    main()