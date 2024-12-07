#!/usr/bin/env python3

import os
import csv
import argparse
import sys
from prettytable import PrettyTable

def main():
    parser = argparse.ArgumentParser( 
        description ='Evaluates the results of multiple tournaments using different minimax configs ranking them by best to worst providing the average points'
    )
    
    parser.add_argument("-i", help = "The input directory containing all the tournaments results", required = True)
    parser.add_argument("-o", help = "The output file where the result is printed")
    parser.add_argument("-top", help = "Determines the best N configs to print", default = 30, type = int)
    
    args = parser.parse_args()

    # Directory containing tournament directories
    input_directory_path = args.i

    collected_data = []

    # Iterate through the tournament directories
    for tournament_dir in os.listdir(input_directory_path):
        
        tournament_dir_path = os.path.join(input_directory_path, tournament_dir)
        stats_file = os.path.join(tournament_dir_path, 'player_statistics.csv')

        if os.path.isfile(stats_file):
            # Read the 3rd column value from player_statistics.csv
            with open(stats_file, 'r') as csv_file:
                csv_reader = csv.reader(csv_file)
                next(csv_reader)  # Read and ignore the header row
                first_row = next(csv_reader)
                minimax_average_points = first_row[3]

            collected_data.append([tournament_dir, minimax_average_points])


    # Sort in descending order
    sorted_list = sorted(collected_data, key=lambda x: x[1], reverse=True)

    if len(sorted_list) > args.top:
        sorted_list = sorted_list[:args.top]
    
    # Initialize the table with column headers
    table = PrettyTable(['Rank','Tournament Directory', 'Minimax average points'])

    ind = 1
    for tournamentDir, minimaxAveragePoints in sorted_list:
        table.add_row([ind, tournamentDir, minimaxAveragePoints])
        ind += 1

    # Print the sorted table
    output_stream = open(args.o, "w") if args.o else sys.stdout
    print(table, file = output_stream)
    
    

if __name__ == "__main__":
    main()