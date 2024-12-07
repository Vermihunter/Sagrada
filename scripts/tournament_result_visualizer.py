#!/usr/bin/env python3

import argparse
import matplotlib.pyplot as plt
from tournament_statistics import collect_data_from_dir
from player_statistics import *


# Initial index for player selection
current_player_index = 0


def main():
    parser = argparse.ArgumentParser( 
        prog = 'tournament_result_visualizer',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description ='Visualize results of a group of tournaments'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournament results", required = True)
    
    args = parser.parse_args()

    tournamentResults = collect_data_from_dir(args.i)

    results = {}
    for tournamentResult in tournamentResults.tournaments:
        [[player1Name, player1WinCount], [player2Name, player2WinCount]] = [[row[PLAYER_STATISTICS_AI_TYPE_COL], int(row[PLAYER_STATISTICS_WIN_COUNT_COL])] for ind, row in tournamentResult.playerStatistics.iterrows()]

        if player1Name not in results:
            results[player1Name] = {player2Name: player1WinCount}
        else:
            results[player1Name][player2Name] = player1WinCount

        #
        if player2Name not in results:
            results[player2Name] = {player1Name: player2WinCount}
        else:
            results[player2Name][player1Name] = player2WinCount


    # Get list of player names
    player_names = list(results.keys())

    # Function to create and update bar plot for the selected player
    def update_plot(player_index):
        player_name = player_names[player_index]
        wins_against_opponents = {opponent: wins for opponent, wins in results[player_name].items()}

        # Create a bar plot
        plt.clf()
        plt.bar(wins_against_opponents.keys(), wins_against_opponents.values(), color='skyblue')

        # Adding labels and title
        plt.xlabel('Opponent AI Players')
        plt.ylabel('Number of Wins')
        plt.title(f'{player_name}\'s Results Against Opponents')

        # Show plot
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        plt.draw()

    # Function to handle key press events
    def on_key(event):
        global current_player_index
        if event.key == 'right':
            current_player_index = (current_player_index + 1) % len(player_names)
        elif event.key == 'left':
            current_player_index = (current_player_index - 1) % len(player_names)
        update_plot(current_player_index)

    # Initialize the plot
    update_plot(current_player_index)

    # Connect the key press event handler
    plt.gcf().canvas.mpl_connect('key_press_event', on_key)

    # Show the plot
    plt.show()




if __name__ == "__main__":
    main()