#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "Constants.h"

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

/** @addtogroup MoveTypedefs
 * @brief Typedefs for moves
 * They typedefs map a Move to a shared pointer pointing to it
 * @{
 */
struct Move;
struct BasicMove;
struct DieToFieldMove;
struct PassMove;
struct ToolCardMove;
struct RelocateDieMove;
struct PlacingDieMove;

using move_t = Move*;
using move_c = std::vector<move_t>;

using pass_move_t         = PassMove*;
using die_to_field_move_t = DieToFieldMove*;

using tool_card_move_t = ToolCardMove*;
using tc_move_c        = std::vector<tool_card_move_t>;
using relocate_move_t  = RelocateDieMove*;

using placing_die_move_t = PlacingDieMove*;

using move_cache_t = std::vector<placing_die_move_t>;
/** @}*/

class EvalState;
using eval_state_t = std::unique_ptr<EvalState>;
using eval_state_c = std::vector<eval_state_t>;

using ID_t = size_t;

struct BoardConfig;
using board_config_t = BoardConfig*;

class Board;
class BoardContext;
using wpc_t       = std::shared_ptr<const BoardContext>;
using wpc_c       = std::vector<wpc_t>;
using wpc_cache_t = std::unordered_map<board_config_t, wpc_c>;

using board_t = std::shared_ptr<Board>;

class BoardField;
using board_field_t = BoardField*; // std::unique_ptr<BoardField>;

class Game;
using game_t = std::unique_ptr<Game>;

struct BoardFieldRestrictions;
using restriction_t = std::unique_ptr<BoardFieldRestrictions>;

class DiceSupply;
using supply_t = std::unique_ptr<DiceSupply>;

class PlayerInformation;
using player_t = std::unique_ptr<PlayerInformation>;
using player_c = std::vector<player_t>;

struct BoardIndex;
using board_data_t = std::vector<std::vector<board_field_t>>;

class ToolCard;
using tc_t = std::unique_ptr<ToolCard>;
using tc_c = std::vector<tc_t>;

class GameContextBuilder;
using cached_game_ctx_builder_t          = std::shared_ptr<GameContextBuilder>;
using cached_game_ctx_builder_c          = std::vector<cached_game_ctx_builder_t>;
using game_ctx_builder_by_player_count_t = std::unordered_map<size_t, cached_game_ctx_builder_c>;

class ToolCardView;
using tc_view_t = std::unique_ptr<ToolCardView>;
using tc_view_c = std::vector<tc_view_t>;

struct AI_PlayerConfig;
using ai_player_config_t  = std::unique_ptr<AI_PlayerConfig>;
using ai_player_config_c  = std::vector<ai_player_config_t>;
using ai_player_config_pc = std::vector<AI_PlayerConfig*>;

class AI_Player;
using ai_player_t = std::unique_ptr<AI_Player>;
using ai_player_c = std::vector<ai_player_t>;

struct BoardConfigSubpageInformation;
using board_config_subpage_info_t = std::unique_ptr<BoardConfigSubpageInformation>;

struct GeneralConfigSubpageInformation;
using general_config_subpage_info_t = std::unique_ptr<GeneralConfigSubpageInformation>;

using move_by_index_c = std::vector<std::pair<move_t, size_t>>;
#endif // TYPEDEFS_H
