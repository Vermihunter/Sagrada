#include "MCTSPlayer.h"

MCTSPlayer::MCTSPlayer(const AIPlayerContext& _ctx, mcts_context_t _playerCtx)
    : AI_Player(_ctx), playerCtx(std::move(_playerCtx))
{
}

ID_t MCTSPlayer::choose_window_pattern_card(wpc_it_t iBegin, wpc_it_t iEnd)
{
	return 0;
}

move_t MCTSPlayer::make_next_move()
{
	// If the game is deterministic, we pass the actual clone of the game and
	// use all the iterations on that one This way we choose simply the move
	// that belongs to the most visited node
	if (ctx.gameDeterministic) {
		auto root = get_root_from_world(ctx.game.clone());
		return root->get_most_visited_move()->clone(*root->get_game()->get_ctx()->supply,
		                                            *ctx.game.get_ctx()->supply);
	}

	// Otherwise, we create the worlds and run the same amount of iterations in
	// all of them
	std::vector<game_t> worlds = create_worlds(playerCtx->determinizingWorldCount.value());

	// World root, visit count, win count
	std::vector<std::tuple<move_t, double, double>> moves;
	std::unique_ptr<MCTS_Node>                      moveSourceNode;
	for (auto&& world : worlds) {
		auto  worldNodePtr = get_root_from_world(std::move(world));
		auto& worldNode    = *worldNodePtr;

		for (size_t i = 0; i < worldNode.get_children().size(); ++i) {
			if (moves.size() == i) {
				moves.push_back({worldNode.get_moves()[i].first,
				                 worldNode.get_children()[i]->get_visit_count(),
				                 worldNode.get_children()[i]->get_win_count()});
			}
			else {
				std::get<1>(moves[i]) += worldNode.get_children()[i]->get_visit_count();
				std::get<2>(moves[i]) += worldNode.get_children()[i]->get_win_count();
			}
		}

		if (!moveSourceNode) {
			moveSourceNode = std::move(worldNodePtr);
		}
	}

	// Sorting the move information by vote count
	auto moveByVoteCmp = [](const auto& lhs, const auto& rhs) {
		if (std::get<1>(lhs) != std::get<1>(rhs)) {
			return std::get<1>(lhs) > std::get<1>(rhs);
		}

		return std::get<2>(lhs) > std::get<2>(rhs);
	};

	std::sort(moves.begin(), moves.end(), moveByVoteCmp);
	return std::get<0>(moves[0])->clone(*moveSourceNode->get_game()->get_ctx()->supply,
	                                    *ctx.game.get_ctx()->supply);
}

std::unique_ptr<MCTS_Node> MCTSPlayer::get_root_from_world(game_t gameWorld)
{
	/** Algorithm */
	auto root = std::make_unique<MCTS_Node>(std::move(gameWorld), nullptr, *playerCtx);

	for (size_t i = 0; i < playerCtx->iterationCount; ++i) {
		MCTS_Node&   selectedNode        = select(*root);
		MCTS_Node&   expandedNode        = selectedNode.expand();
		game_t       simulatedGame       = expandedNode.simulate_game();
		eval_state_c simulatedGameResult = simulatedGame->evaluate();
		expandedNode.update(simulatedGameResult);
	}

	return root;
}

std::vector<game_t> MCTSPlayer::create_worlds(int worldCount)
{
	std::vector<game_t> worlds;

	for (int i = 0; i < worldCount; ++i) {
		worlds.push_back(ctx.game.clone_with_pseudo_random_future(ctx.ID, 73 + i));
	}

	return worlds;
}

MCTS_Node& MCTSPlayer::select(MCTS_Node& _root)
{
	auto nodesByHighestUCT = [](mcts_node_t& lhs, mcts_node_t& rhs) {
		return lhs->get_uct() < rhs->get_uct();
	};

	MCTS_Node* currNode = &_root;
	while (currNode->is_fully_expanded() && !currNode->is_leaf()) {
		auto& currNodeChildren = currNode->get_children();
		currNode = &(**std::max_element(currNodeChildren.begin(), currNodeChildren.end(),
		                                nodesByHighestUCT));
	}

	return *currNode;
}

MCTSPlayer::~MCTSPlayer() {}