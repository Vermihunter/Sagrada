#include "MCTSNode.h"

MCTS_Node::MCTS_Node(game_t _game, MCTS_Node* _parent,
                     const MCTSPlayerContext& _playerCtx)
    : game(std::move(_game))
    , playerCtx(_playerCtx)
    , parent(_parent)
    , visitCount(0)
    , winCount(0)
{
    possibleMoves = std::make_unique<move_by_index_c>();
    if (!game->get_curr_state()->hasEnded) {
        add_filtered_moves();
    }
}

uct_t MCTS_Node::get_uct()
{
    return (winCount / (visitCount * 5)) +
           playerCtx.explorationConstant *
               std::sqrt(std::log(parent->visitCount) / visitCount);
}

MCTS_Node& MCTS_Node::expand()
{
    if (children.size() >= possibleMoves->size()) {
        return *this;
    }

    auto clonedGame = game->clone();
    move_t& moveToExpand = (*possibleMoves)[children.size()].first;
    move_t selectedMove = moveToExpand->clone(*game->get_ctx()->supply,
                                              *clonedGame->get_ctx()->supply);
    clonedGame->move_request(selectedMove);

    children.push_back(
        std::make_unique<MCTS_Node>(std::move(clonedGame), this, playerCtx));
    return *children.back();
}

void MCTS_Node::update(eval_state_c& scores)
{
    if (parent && scores[0]->get_player()->ID ==
                      (*parent->possibleMoves)[0].first->playerId) {
        update_win_count(scores);
    }

    ++visitCount;

    if (parent) {
        parent->update(scores);
    }
}

game_t MCTS_Node::simulate_game()
{
    auto clonedGame = game->clone();
    auto& fasterGame = *clonedGame;

    ai_player_c aiPlayers;
    for (size_t i = 0; i < fasterGame.get_ctx()->players.size(); ++i) {
        AIPlayerContext aiCtx {.ID = i,
                               .game = fasterGame,
                               .privateObjectiveCard =
                                   fasterGame.get_ctx()->players[i]->PrOC,
                               .gameDeterministic = true};
        aiPlayers.push_back(playerCtx.playoutStrategy->create_player(aiCtx));
    }

    while (!fasterGame.get_curr_state()->hasEnded) {
        move_t chosenMove =
            aiPlayers[fasterGame.get_curr_state()->player_on_move_id()]
                ->make_next_move();
        fasterGame.move_request(chosenMove);
    }

    return clonedGame;
}

move_t MCTS_Node::get_most_visited_move()
{
    auto mostVisitedNodeCmp = [](const mcts_node_t& lhs,
                                 const mcts_node_t& rhs) {
        if (lhs->visitCount != rhs->visitCount) {
            return lhs->visitCount < rhs->visitCount;
        }

        return lhs->winCount < rhs->winCount;
    };

    auto mostVisitedChild = std::max_element(children.cbegin(), children.cend(),
                                             mostVisitedNodeCmp);
    int mostVisitedMoveIndex =
        std::distance(children.cbegin(), mostVisitedChild);
    return (*possibleMoves)[mostVisitedMoveIndex].first;
}

void MCTS_Node::add_filtered_moves()
{
    PlayerInformation& currPlayer =
        *game->get_ctx()->players[game->get_curr_state()->player_on_move_id()];
    Board& board = *currPlayer.board;
    move_c allMoves = game->possible_moves();
    std::unique_ptr<move_by_index_c> backupMoves =
        std::make_unique<move_by_index_c>();

    filter_moves(*game, allMoves, *possibleMoves, *backupMoves);

    if (possibleMoves->empty()) {
        possibleMoves = std::move(backupMoves);
    }
    else {
        free_all_with_ind(backupMoves->begin(), backupMoves->end());
    }

    std::sort(possibleMoves->begin(), possibleMoves->end(),
              MoveHeuristicCMP {board, currPlayer.PrOC->get_color()});
    children.reserve(possibleMoves->size());
}

void MCTS_Node::update_win_count(eval_state_c& scores)
{
    eval_state_t& firstScore = scores[0];
    eval_state_t& secondScore = scores[1];

    int firstTotalScore = firstScore->get_total_points();
    int secondTotalScore = secondScore->get_total_points();

    ++winCount;
    const static std::array<int, 4> pointDiffBorders {5, 10, 15, 20};

    for (auto&& pointDiffBorder : pointDiffBorders) {
        if (firstTotalScore - pointDiffBorder < secondTotalScore) {
            break;
        }

        ++winCount;
    }
}

MCTS_Node::~MCTS_Node()
{
    free_all_with_ind(possibleMoves->begin(), possibleMoves->end());
}