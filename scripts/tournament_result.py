#!/usr/bin/env python3

import pandas as pd
from game_result import *

TOURNAMENTS_WON_KEY = "Tournament wins"
TOTAL_GAMES_WON_KEY = "Total games won"
TOTAL_GAME_PLAYED_KEY = "Games played"

class TournamentResult:
    def __init__(self, playerStatistics: pd.DataFrame, gameResults: pd.DataFrame, winner: str) -> None:
        self.playerStatistics = playerStatistics
        self.gameResults = gameResults
        self.winner = winner

    def __str__(self):
        return f"Winner of the tournament {self.winner} - {self.playerStatistics}"
    

class TournamentResults:
    def __init__(self):
        self.player_statistics = {}
        self.tournaments = []
        
    def _process_player(self, playerName: str, tournamentResult: TournamentResult):
        additional_tournament_win_count = 1 if tournamentResult.winner == playerName else 0
        games_won = (tournamentResult.gameResults["winner"] == playerName).sum()
        games_played = len(tournamentResult.gameResults)

        if playerName not in self.player_statistics:
            self.player_statistics[playerName] = {
                TOURNAMENTS_WON_KEY: additional_tournament_win_count,
                TOTAL_GAMES_WON_KEY: games_won,
                TOTAL_GAME_PLAYED_KEY: games_played,
            }
        else:
            self.player_statistics[playerName][TOURNAMENTS_WON_KEY] += additional_tournament_win_count
            self.player_statistics[playerName][TOTAL_GAMES_WON_KEY] += games_won
            self.player_statistics[playerName][TOTAL_GAME_PLAYED_KEY] += games_played


    
    def add_tournament_result(self, tournamentResult: TournamentResult):
        
        player1_name = tournamentResult.gameResults.loc[1, "Player 1"]
        player2_name = tournamentResult.gameResults.loc[1, "Player 2"]
        self.tournaments.append([player1_name, player2_name, tournamentResult])

        self._process_player(player1_name, tournamentResult)
        self._process_player(player2_name, tournamentResult)