project('sagrada', 'cpp', default_options : ['cpp_std=c++20'])
CXX='gcc'

# Dependencies
# GTK dependencies - GUI
gtkdep = dependency('gtk+-3.0')
gtkmmdep = dependency('gtkmm-3.0')

# JSON dependencies - Serialization
json_proj = subproject('nlohmann_json')
json_dep = json_proj.get_variable('nlohmann_json_dep')

# GoogleTest - Unit testing
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

# CMD line argument parsing
argparse_proj = subproject('argparse')
argparse_dep = argparse_proj.get_variable('argparse_dep')


# Controller
logger_src = ['src/Controller/Logger/Logger.cpp', 'src/Controller/Logger/Date.cpp']
logger_inc = 'include/Controller/Logger'

controller_src = ['src/Controller/InputHandlers/BoardTextFileReader.cpp', 'src/Controller/SagradaGameCMDLineParser.cpp', 'src/Controller/LocalPlayerController.cpp', 'src/Controller/OnlyAIPlayerController.cpp']
controller_inc = ['include/Controller', 'include/Controller/InputHandlers']


# Model
board_inc = ['include/Model/Board', 'include/Model/Board/BoardType']
board_src = ['src/Model/Board/BoardFieldNeighborCache.cpp','src/Model/Board/OverallBoardState.cpp','src/Model/Board/BoardConfig.cpp', 'src/Model/Board/Board.cpp', 'src/Model/Board/BoardField.cpp', 'src/Model/Board/BoardType/RectangularBoardType.cpp', 'src/Model/Board/BoardContext.cpp']

puoc_inc = ['include/Model/PublicObjectiveCards', 'include/Model/PublicObjectiveCards/Types']
puoc_src = ['src/Model/PublicObjectiveCards/PublicObjectiveCard.cpp','src/Model/PublicObjectiveCards/PublicObjectiveCardPatternState.cpp']

tc_inc = 'include/Model/ToolCards'
tc_src = ['src/Model/ToolCards/RelocatingTC.cpp','src/Model/ToolCards/ChangeDieValueTC.cpp', 'src/Model/ToolCards/MoveTwiceInARowTC.cpp', 'src/Model/ToolCards/PutDieWithoutNeighborTC.cpp', 'src/Model/ToolCards/RelocateExactlyTwoDiceTC.cpp', 'src/Model/ToolCards/RelocateMaxTwoSameColorDiceTC.cpp', 'src/Model/ToolCards/ReplaceDieFromSupplyToRoundTrackTC.cpp', 'src/Model/ToolCards/ReplaceDieWithRandomFromBagTC.cpp', 'src/Model/ToolCards/RerollAllDiceTC.cpp', 'src/Model/ToolCards/RerollDieTC.cpp', 'src/Model/ToolCards/ReverseDieValueTC.cpp', 'src/Model/ToolCards/ToolCard.cpp']

model_inc = [board_inc, puoc_inc, tc_inc, 'include/Model', 'include/Model/Game', 'include/Model/Moves']
model_src = [board_src, puoc_src, tc_src, 'src/Model/Game/GameCache.cpp','src/Model/BasicMoveHandler.cpp', 'src/Model/Game/GameCacheInitializer.cpp','src/Model/Attributes.cpp', 'src/Model/Moves/RelocateDieMove.cpp', 'src/Model/DiceSupply.cpp', 'src/Model/Die.cpp', 'src/Model/Game/Game.cpp','src/Model/Game/GameState.cpp','src/Model/Game/GameContext.cpp','src/Model/Game/GameInfo.cpp','src/Model/Game/GameContextBuilder.cpp', 'src/Model/RoundTrack.cpp', 'src/Model/EvalState.cpp' ] #  'src/Model/PrivateObjectiveCard.cpp',


# Config
config_src = ['src/Config/DefaultAIPlayerConfig.cpp']

# AI Players
ai_src = ['src/AIPlayers/MCTSPlayer.cpp', 'src/AIPlayers/MCTSNode.cpp', 'src/AIPlayers/MCTSPlayerConfig.cpp','src/AIPlayers/RulesBasedPlayer.cpp', 'src/AIPlayers/RulesBasedPlayerConfig.cpp','src/AIPlayers/FirstPlayer.cpp', 'src/AIPlayers/RandomPlayer.cpp', 'src/AIPlayers/AI_Player.cpp', 'src/AIPlayers/DefinedAIPlayers.cpp','src/AIPlayers/MinimaxPlayer.cpp', 'src/AIPlayers/MinimaxPlayerConfig.cpp']
ai_inc =  'include/AIPlayers'

# Tournament
tournament_src = ['src/Tournament/Tournament.cpp', 'src/Tournament/PlayerStatistics.cpp', 'src/Tournament/DefaultPointMapper.cpp']
tournament_inc = ['include/Tournament']

# View
tc_view_src = ['src/View/ToolCards/ToolCardView.cpp','src/View/ToolCards/ToolCardViewContainer.cpp', 'src/View/Images/ToolCardImages.cpp']
tc_view_inc = ['include/View/ToolCards']

puoc_view_src = ['src/View/Images/PublicObjectiveCardImages.cpp']
puoc_view_inc = ['include/View/PublicObjectiveCards']

proc_view_inc = ['include/View/PrivateObjectiveCards']

page_src = ['src/View/Pages/TournamentPage/TournamentStatisticsPage.cpp','src/View/Pages/GamePlayingPage/GamePlayingPageContext.cpp','src/View/Pages/CustomConfigPage/CustomConfigPage.cpp']
page_inc = ['include/View/Pages','include/View/Pages/TournamentPage','include/View/Pages/WPCChoosingPage','include/View/Pages/HomePage', 'include/View/Pages/GamePlayingPage', 'include/View/Pages/LocalPlayerGameConfigPage', 'include/View/Pages/SimulationPage', 'include/View/Pages/CustomConfigPage']

view_src = [page_src, tc_view_src, puoc_view_src, 'src/View/Image.cpp','src/View/Pages/GameConfigurationWidgets.cpp','src/View/RoundTrackView.cpp','src/View/ViewContext.cpp','src/View/SagradaWindow.cpp','src/View/BoardFieldView.cpp','src/View/Digits.cpp','src/View/RoundNumberView.cpp','src/View/ScoreBoard.cpp','src/View/PlayerOrderDisplayer.cpp','src/View/RectangularBoardView.cpp','src/View/Pages/CustomConfigPage/AllSubpages.cpp','src/View/Pages/TournamentPage/TournamentGameResult.cpp','src/View/Pages/GamePlayingPage/SimulationGamePlayingPage.cpp','src/View/Pages/TournamentPage/TournamentResultPage.cpp','src/View/Pages/TournamentPage/TournamentPage.cpp','src/View/Pages/TournamentPage/TournamentConfigPage.cpp','src/View/Pages/CommonGameConfigElements.cpp','src/View/Pages/GamePlayingPage/LocalGamePlayingPage.cpp','src/View/Pages/GamePlayingPage/LocalGamePlayingPageContext.cpp','src/View/Pages/HomePage/LocalPlayerGameTypeChoosingSubpage.cpp','src/View/Pages/HomePage/HomePage.cpp','src/View/Pages/GamePlayingPage/GamePlayingPage.cpp','src/View/Pages/HomePage/LocalPlayerGameConfigurationSubpage.cpp','src/View/Pages/WPCChoosingPage/WPCChoosingPage.cpp','src/View/Pages/CustomConfigPage/BoardConfigSubpage.cpp','src/View/Pages/WPCChoosingPage/WPCChoosingPage.cpp','src/View/Pages/SimulationPage/SimulationConfigPage.cpp','src/View/Pages/SimulationPage/SimulationConfigPageBase.cpp']
view_inc = [page_inc, tc_view_inc, puoc_view_inc, proc_view_inc, 'include/View', 'include/View/Images']

sagrada_json_inc = ['include/SagradaJson']
signallers_inc = ['include/Signallers']

sagrada_src = [view_src, tournament_src]
sagrada_inc = [view_inc, tournament_inc,  signallers_inc]

# Model shared library
model_lib = static_library('model', 
    [model_src, logger_src, controller_src, ai_src], 
    include_directories: [model_inc, logger_inc, sagrada_json_inc, controller_inc, ai_inc],
    dependencies: [json_dep, argparse_dep]
)

executable('sagrada', 
    [sagrada_src, 'src/View/main.cpp'], 
    include_directories: [sagrada_inc, model_inc, logger_inc, sagrada_json_inc, controller_inc, ai_inc], 
    dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
    link_with: model_lib
)

# executable('wpc_saver', 
#     [sagrada_src, 'src/View/wpc_png_saver_main.cpp'], 
#     include_directories: [sagrada_inc, model_inc, logger_inc, sagrada_json_inc, controller_inc, ai_inc], 
#     dependencies: [gtkdep, gtkmmdep, json_dep, argparse_dep],
#     link_with: model_lib
# )

# Tournament exe
all_tournament_src = [logger_src, ai_src, controller_src, tournament_src, 'src/Tournament/main.cpp']
executable('tournament', 
    all_tournament_src, 
    include_directories: [logger_inc, ai_inc, model_inc, controller_inc, tournament_inc, sagrada_json_inc, signallers_inc],
    dependencies: [json_dep, argparse_dep],
    link_with: model_lib
)


# Unit tests
test_inc = ['tests/GameCloneTests','tests/ToolCardTests', 'tests']

e = executable('tests',
    include_directories: [test_inc, logger_inc, ai_inc, model_inc, controller_inc, sagrada_json_inc, signallers_inc], 
    sources: ['tests/main.cpp'],
    dependencies: [gtest_dep, gmock_dep, json_dep, argparse_dep],
    link_with: model_lib)    

test('gtest tests', e)
