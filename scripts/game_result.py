
CSV_SEED_COL_NAME = "seed"
CSV_PLAYER1_NAME_COL_NAME = "Player 1"
CSV_PLAYER1_SCORE_COL_NAME = "Player 1 score"
CSV_PLAYER2_NAME_COL_NAME = "Player 2"
CSV_PLAYER2_SCORE_COL_NAME = "Player 2 score"
CSV_WINNER_COL_NAME = "winner"
CSV_TC1_COL_NAME = "TC1"
CSV_TC2_COL_NAME = "TC2"
CSV_TC3_COL_NAME = "TC3"
CSV_PUOC1_COL_NAME = "PUOC1"
CSV_PUOC2_COL_NAME = "PUOC2"
CSV_PUOC3_COL_NAME = "PUOC3"

GAME_RESULT_FILE_NAME = "game_results.csv"

CSV_GAME_INFO_COL_IND_GAME_SEED = 0
CSV_GAME_INFO_COL_IND_PLAYER1_NAME = 1
CSV_GAME_INFO_COL_IND_PLAYER1_SCORE = 2
CSV_GAME_INFO_COL_IND_PLAYER2_NAME = 3
CSV_GAME_INFO_COL_IND_PLAYER2_SCORE = 4
CSV_GAME_INFO_COL_IND_WINNER = 5
CSV_GAME_INFO_COL_IND_TC0IND = 6
CSV_GAME_INFO_COL_IND_TC1IND = 7
CSV_GAME_INFO_COL_IND_TC2IND = 8
CSV_GAME_INFO_COL_IND_PUOC0_IND = 9
CSV_GAME_INFO_COL_IND_PUOC1_IND = 10
CSV_GAME_INFO_COL_IND_PUOC2_IND = 11

class GameResult:
     def from_list(csvAttributes: list):
        res = GameResult()
        res.gameSeed = int(csvAttributes[CSV_GAME_INFO_COL_IND_GAME_SEED])
        res.player1Name = csvAttributes[CSV_GAME_INFO_COL_IND_PLAYER1_NAME]
        res.player1Score = int(csvAttributes[CSV_GAME_INFO_COL_IND_PLAYER1_SCORE])
        res.player2Name = csvAttributes[CSV_GAME_INFO_COL_IND_PLAYER2_NAME]
        res.player2Score = int(csvAttributes[CSV_GAME_INFO_COL_IND_PLAYER2_SCORE])
        res.winner = csvAttributes[CSV_GAME_INFO_COL_IND_WINNER]
        
        if len(csvAttributes) > CSV_GAME_INFO_COL_IND_TC0IND:
            res.tc0Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_TC0IND])
            res.tc1Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_TC1IND])
            res.tc2Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_TC2IND])
            res.puoc0Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_PUOC0_IND])
            res.puoc1Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_PUOC1_IND])
            res.puoc2Index = int(csvAttributes[CSV_GAME_INFO_COL_IND_PUOC2_IND])
            
        return res
     
     def from_map(row):
        res = GameResult()
        res.gameSeed = int(row[CSV_SEED_COL_NAME])
        res.player1Name = row[CSV_PLAYER1_NAME_COL_NAME]
        res.player1Score = int(row[CSV_PLAYER1_SCORE_COL_NAME])
        res.player2Name = row[CSV_PLAYER2_NAME_COL_NAME]
        res.player2Score = int(row[CSV_PLAYER2_SCORE_COL_NAME])
        res.winner = row[CSV_WINNER_COL_NAME]
        
        if len(row) > CSV_GAME_INFO_COL_IND_TC0IND:
            res.tc0Index = int(row[CSV_TC1_COL_NAME])
            res.tc1Index = int(row[CSV_TC2_COL_NAME])
            res.tc2Index = int(row[CSV_TC3_COL_NAME])
            res.puoc0Index = int(row[CSV_PUOC1_COL_NAME])
            res.puoc1Index = int(row[CSV_PUOC2_COL_NAME])
            res.puoc2Index = int(row[CSV_PUOC3_COL_NAME])
            
        return res

