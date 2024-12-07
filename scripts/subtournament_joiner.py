#!/usr/bin/env python3

from sagrada_tournament import *
import os


def main():
    gamecount = 96
    tournamentArgs = []
    csvdir = "tournament_results/04_02_expl_const_experiment/"
    for dir in os.listdir(csvdir):
        #tournamentArgs.append()
        #print(dir)
        fullPath = f"{csvdir}/{dir}"
        runArgs = [[TournamentArgs(0,gamecount, f"{fullPath}/tmp_subtournament_{i}", [], "")] for i in range(1,17)]
        print(runArgs[0][0].outputPath)
        combine_results(runArgs, 96, f"04_02_expl_const_experiment/{dir}")
   
    
    




if __name__ == "__main__":
    main()