import itertools
import sagrada_tournament
import json
import shutil

OUTPUT_DIR = "MCTS_TC_TOURNAMENTS"
TOOL_CARD_COUNT = 12
TMP_GAME_CONFIG_PATH = "data/GameConfigurations"
EXPERIMENT_GAME_COUNT = 100
EXPERIMENT_PLAYERS = ["minimax-depth=2,config_file=tournament_results/randomized_cheating_new_build/tournament_3559/minimax_config.json", "mcts-it=300,playout=rules-based"]
EXPERIMENT_SEED = 779
experimentDir = "reworked_backend_experiment"
gameMode = "default"

def main():
    all_configs = [[]]
    for tc1 in range(TOOL_CARD_COUNT):
        all_configs.append([tc1])
        for tc2 in range(tc1 + 1, TOOL_CARD_COUNT):
            all_configs.append([tc1, tc2])
            for tc3 in range(tc2 + 1, TOOL_CARD_COUNT):
                all_configs.append([tc1, tc2, tc3])

    for config in all_configs:
        config_name = f'TCS_{"_".join(map(str, config))}'
        game_config = {
            "PuocConfig": {
                "puoc_contexts": [
                    {
                        "PuocId": 0,
                        "SatisfactionValue": 5
                    },
                    {
                        "PuocId": 1,
                        "SatisfactionValue": 6
                    },
                    {
                        "PuocId": 2,
                        "SatisfactionValue": 4
                    },
                    {
                        "PuocId": 3,
                        "SatisfactionValue": 5
                    },
                    {
                        "PuocId": 4,
                        "SatisfactionValue": 2
                    },
                    {
                        "PuocId": 5,
                        "SatisfactionValue": 2
                    },
                    {
                        "PuocId": 6,
                        "SatisfactionValue": 2
                    },
                    {
                        "PuocId": 7,
                        "SatisfactionValue": 5
                    },
                    {
                        "PuocId": 8,
                        "SatisfactionValue": 4
                    },
                    {
                        "PuocId": 9,
                        "SatisfactionValue": 1
                    }
                ],
                "cards_per_game": 3
            },
            "TcConfig": {
                "selectable_ids": config,
                "cards_per_game": len(config)
            },
            "PlayerCountRelatedInformation": [
                {
                    "PlayerCount": 2,
                    "DiceCount": 5
                },
                {
                    "PlayerCount": 3,
                    "DiceCount": 7
                },
                {
                    "PlayerCount": 4,
                    "DiceCount": 9
                }
            ],
            "DiceConfig": [
                {
                    "Color": "Blue",
                    "DiceCount": 18
                },
                {
                    "Color": "Red",
                    "DiceCount": 18
                },
                {
                    "Color": "Green",
                    "DiceCount": 18
                },
                {
                    "Color": "Yellow",
                    "DiceCount": 18
                },
                {
                    "Color": "Purple",
                    "DiceCount": 18
                }
            ],
            "ScoreContext": {
                "pointsPerUnusedFavorToken": 1,
                "minusPointsPerEmptyField": 1,
                "pointsPerProcValue": 1
            },
            "wpcChoicePerPlayer": 4,
            "NumberOfRounds": 10,
            "SelectableWPC": [
                "Lux Mundi", "Lux Astram", "Luz Celestial", "Fulgor del Cielo", "Shadow Thief", "Sun Catcher", "Bellesguard", "Batllo", "Sun's Glory", "Firelight", "Gravitas", "Water of Life", "Comitas", "Chromatic Splendor", "Aurora Sagradis", "Aurorae Magfinicus", "Firmitas", "Kaleidoscopic Dream", "Symphony of Light", "Virtus", "Via Lux", "Industria", "Fractal Drops", "Ripples of Light"
            ],
            "BoardConfig": {
                "BoardType": "Rectangle",
                "Width": 5,
                "Height": 4
            }
            
        }
        
        tournament_dir = f'{OUTPUT_DIR}/{config_name}'
        config_file_name = f'{TMP_GAME_CONFIG_PATH}/{config_name}.json'
        
        c = json.dumps(game_config)

        # Writing to sample.json
        with open(config_file_name, "w") as outfile:
            outfile.write(c)

        sagrada_tournament.run_tournaments(EXPERIMENT_GAME_COUNT, EXPERIMENT_SEED, tournament_dir, EXPERIMENT_PLAYERS, config_name)

        print(config_name)





if __name__ == "__main__":
    main()