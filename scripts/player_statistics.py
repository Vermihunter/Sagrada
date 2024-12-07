import pandas as pd

PLAYER_STATISTICS_AI_TYPE_COL = "AI type"
PLAYER_STATISTICS_WIN_COUNT_COL = "Win count"
PLAYER_STATISTICS_PLACEMENT_POINTS_COL = "Placement points"
PLAYER_STATISTICS_AVG_POINTS_COL = "Average points"
PLAYER_STATISTICS_TOTAL_MOVE_MS_COL = "Total move time in ms"
PLAYER_STATISTICS_TOTAL_MOVE_COUNT_COL = "Total move count"
PLAYER_STATISTICS_AVG_MOVE_MS_COL = "Average move time in ms"

class PlayerStatistics:
    def __init__(self, winCount: int, placementPoints: int, totalMoveTimeInMs: int, totalMoveCount: int):
        self.winCount = winCount
        self.placementPoints = placementPoints
        self.totalMoveTimeInMs = totalMoveTimeInMs
        self.totalMoveCount = totalMoveCount

    def __add__(self, other):
        if not isinstance(other, PlayerStatistics):
            raise TypeError()
        
        return PlayerStatistics(
            self.winCount + other.winCount,
            self.placementPoints + other.placementPoints,
            (self.totalMoveTimeInMs + other.totalMoveTimeInMs) / 2,
            self.totalMoveCount + other.totalMoveCount
        )
    
    def from_csv(csv_row):
        win_count = csv_row[PLAYER_STATISTICS_WIN_COUNT_COL] 
        placement_points = csv_row[PLAYER_STATISTICS_PLACEMENT_POINTS_COL]
        total_move_in_ms = csv_row[PLAYER_STATISTICS_TOTAL_MOVE_MS_COL]
        total_move_count = csv_row[PLAYER_STATISTICS_TOTAL_MOVE_COUNT_COL]
        return PlayerStatistics(win_count, placement_points, total_move_in_ms, total_move_count)


def construct_for_tournament(tournament_game_playerstatistics_files: list, gameResults: pd.DataFrame, gameCount: int):
    # extracting the player statistics from all the sub-tournament results
    player_statistics = {}
    for player_stat_file in tournament_game_playerstatistics_files:
        df = pd.read_csv(player_stat_file)

        for _ind, row in df.iterrows():
            ai_type = row[PLAYER_STATISTICS_AI_TYPE_COL]
            player = PlayerStatistics.from_csv(row)
            
            # Update the player_sums dictionary
            if ai_type in player_statistics:
                player_statistics[ai_type] += player
            else:
                player_statistics[ai_type] = player
    
    for i in range(len(player_statistics)):
        player_name_column_ind = 1 + i * 2
        player_score_column_ind = 1 + i * 2 + 1
        player_name = gameResults.iloc[0, player_name_column_ind]
        total_score = gameResults.iloc[:, player_score_column_ind].sum()
        player_statistics[player_name].averagePoints = (total_score / gameCount)


    # Create a new DataFrame from the player_wincounts, player_sums, and player_averages dictionaries
    result_df = pd.DataFrame(
        {
            PLAYER_STATISTICS_AI_TYPE_COL: list(player_statistics.keys()), 
            PLAYER_STATISTICS_WIN_COUNT_COL: [stat.winCount for stat in player_statistics.values()],
            PLAYER_STATISTICS_PLACEMENT_POINTS_COL: [stat.placementPoints for stat in player_statistics.values()],
            PLAYER_STATISTICS_AVG_POINTS_COL: [stat.averagePoints for stat in player_statistics.values()],
            PLAYER_STATISTICS_TOTAL_MOVE_MS_COL: [stat.totalMoveTimeInMs for stat in player_statistics.values()],
            PLAYER_STATISTICS_TOTAL_MOVE_COUNT_COL: [stat.totalMoveCount for stat in player_statistics.values()],
            PLAYER_STATISTICS_AVG_MOVE_MS_COL: [float(stat.totalMoveTimeInMs / stat.totalMoveCount) for stat in player_statistics.values()]
        }
    )

    return player_statistics, result_df