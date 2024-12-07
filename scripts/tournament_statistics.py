#!/usr/bin/env python3

import argparse
from prettytable import PrettyTable
from statsmodels.stats.proportion import *
import os
from player_statistics import *
from tournament_result import *
from game_result import *
from tournaments_config import *
from player_statistics import *

def collect_data_from_dir(base_tournament_result_dir: str) -> TournamentResults:
    tournamentResults = TournamentResults()
    for tournamentDir in os.listdir(base_tournament_result_dir):
        item_path = os.path.join(base_tournament_result_dir, tournamentDir)

        # Game results
        gameResultPath = os.path.join(item_path, GAME_RESULT_CSV_FILENAME)
        gameResultDf = pd.read_csv(gameResultPath)

        # Player statistics
        player_statistics_path = os.path.join(item_path, PLAYER_STATISTICS_CSV_FILENAME)
        _, result_df = construct_for_tournament([player_statistics_path], gameResultDf, len(gameResultDf))

        # Game statistics
        gameStatisticsFile = os.path.join(item_path, GAME_STATISTICS_CSV_FILENAME)
        df = pd.read_csv(gameStatisticsFile)
        winner = df.iloc[0, 3]

        tournamentResults.add_tournament_result(TournamentResult(result_df, gameResultDf, winner))

    return tournamentResults

def main():
    parser = argparse.ArgumentParser( 
        prog = 'tournament_statistics',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description ='Runs statistics on tournaments'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournament results", required = True)
    
    args = parser.parse_args()

    tournamentResults = collect_data_from_dir(args.i)

    table = PrettyTable()
    table.field_names = ["Player name", TOURNAMENTS_WON_KEY, TOTAL_GAMES_WON_KEY, TOTAL_GAME_PLAYED_KEY, "Confidence interval"]
    sorted_statistics = dict(sorted(tournamentResults.player_statistics.items(), key=lambda item: (item[1][TOURNAMENTS_WON_KEY], item[1][TOTAL_GAMES_WON_KEY]), reverse=True))
    for player_name, player_results in sorted_statistics.items():
        wilson_lower_bound, wilson_upper_bound = proportion_confint(player_results[TOTAL_GAMES_WON_KEY], player_results[TOTAL_GAME_PLAYED_KEY], 0.02, 'wilson')
        table.add_row([player_name, player_results[TOURNAMENTS_WON_KEY], player_results[TOTAL_GAMES_WON_KEY], player_results[TOTAL_GAME_PLAYED_KEY], f"{wilson_lower_bound * 100:.2f} - {wilson_upper_bound* 100:.2f}"])

    print(table)

    agentNicknames = {
        "Minimax1,worlds=2000": "Mini1,2000",
        "Minimax2,worlds=200": "Mini2,200",
        "Minimax3,worlds=20": "Mini3,20",
        "Minimax4,worlds=2": "Mini4,2",
        "RulesBased_ONLYDTFM": "RB_OD",
        "RulesBased_ALLMOVES": "RB_AM",
        "MCTS270,expl=1.00,worlds=20,playout=First": "MCTS270_First",
        "MCTS360,expl=1.80,worlds=15,playout=First": "MCTS360_First",
        "MCTS135,expl=1.40,worlds=6,playout=RulesBased_ONLYDTFM": "MCTS135_OD",
        "First": "First",
        "Random": "Random"
    }

    playersToLookFor = [ "Minimax1,worlds=2000", "Minimax2,worlds=200", "Minimax3,worlds=20", "Minimax4,worlds=2"]
    playersToLookFor = [ "MCTS270,expl=1.00,worlds=20,playout=First", "MCTS360,expl=1.80,worlds=15,playout=First", "MCTS135,expl=1.40,worlds=6,playout=RulesBased_ONLYDTFM"]

    for opponent in sorted_statistics:
        print(f"\multirow{{2}}{{*}}{{\\textbf{{{agentNicknames[opponent]}}}}}", end="")
        secondRow = ""
        for player in playersToLookFor:
            
            if opponent == player:
                print(" &", end="")
                secondRow += " &"
                continue
            
            correspondingTournament = list(filter(lambda x: (x[0] == opponent and x[1] == player) or (x[1] == opponent and x[0] == player), tournamentResults.tournaments))
            winCount = (correspondingTournament[0][2].gameResults["winner"] == player).sum()
            wilson_lower_bound, wilson_upper_bound = proportion_confint(winCount, len(correspondingTournament[0][2].gameResults), 0.02, 'wilson')

            print(f" & {len(correspondingTournament[0][2].gameResults)}", end="")
            secondRow += f" & {format(wilson_lower_bound*100, '.1f')}\%-{format(wilson_upper_bound*100, '.1f')}\%"

        print(" \\\\")
        print(f"{secondRow} \\\\ \\hline")


    table = PrettyTable()
    agents = "", *[agentNicknames[playerName] for playerName in sorted_statistics]
    #print(tournamentResults.tournaments)
    table.field_names = [*agents]
    for player1_name in sorted_statistics:
        row = [agentNicknames[player1_name]]
        for player2_name in sorted_statistics:
            if player1_name == player2_name:
                row.append("X")
                continue

            correspondingTournament = list(filter(lambda x: (x[0] == player1_name and x[1] == player2_name) or (x[1] == player1_name and x[0] == player2_name), tournamentResults.tournaments))
            winCount = (correspondingTournament[0][2].gameResults["winner"] == player1_name).sum()
            
            wilson_lower_bound, wilson_upper_bound = proportion_confint(winCount, len(correspondingTournament[0][2].gameResults), 0.02, 'wilson')
            #print(f"Player {player1_name} has won {winCount} games against {player2_name}")
            row.append(f"{len(correspondingTournament[0][2].gameResults)}-[{format(wilson_lower_bound * 100, '.1f')}-{format(wilson_upper_bound * 100,'.1f')}]")
        table.add_row(row)


    #print(table)  

if __name__ == '__main__':
    main()