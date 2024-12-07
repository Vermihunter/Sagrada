#include "EvalState.h"

#include "Game.h"

EvalState::EvalState(const player_t& _player, Game& g)
    : player(_player)
    , ctx(g.get_ctx()->scoreCtx)
    , puoc(g.get_ctx()->publicObjectiveCards)
    , emptyFieldMinusPoints(0)
    , unusedFavorTokenPoints(0)
    , privateObjectiveCardPoints(0)
    , totalPoints(0)
    , boardState(g, player->ID)

{
    auto& puocs = g.get_ctx()->publicObjectiveCards;

    evaluate_favor_token_points();
    evaluate_empty_fields();
    evaluate_private_objective_card_points();
    evaluate_public_objective_card_points(puocs);
}

void EvalState::evaluate_favor_token_points()
{
    unusedFavorTokenPoints =
        player->favorTokensLeft * ctx->pointsPerUnusedFavorToken;

    totalPoints += unusedFavorTokenPoints;
}

void EvalState::evaluate_empty_fields()
{
    emptyFieldMinusPoints = player->board->get_number_of_empty_fields() *
                            ctx->minusPointsPerEmptyField;

    totalPoints -= emptyFieldMinusPoints;
}

void EvalState::evaluate_private_objective_card_points()
{
    privateObjectiveCardPoints =
        player->board->get_total_dice_value_having_color(
            player->PrOC->get_color()) *
        ctx->pointsPerProcValue;

    totalPoints += privateObjectiveCardPoints;
}

void EvalState::evaluate_public_objective_card_points(puoc_c& PuOC)
{
    for (size_t i = 0; i < PuOC.size(); ++i) {
        puocPatterns.push_back(PuOC[i]->evaluate(boardState, player->board));
        totalPoints += puocPatterns[i].totalEarnedPoints;
    }
}

eval_state_t EvalState::for_player(const player_t& player, Game& g)
{
    return std::make_unique<EvalState>(player, g);
}

eval_state_c EvalState::for_players(Game& g)
{
    eval_state_c res;

    for (auto&& player : g.get_ctx()->players) {
        res.push_back(for_player(player, g));
    }

    std::sort(
        res.begin(), res.end(),
        [&](const eval_state_t& score1, const eval_state_t& score2) -> bool {
            if (score1->get_total_points() != score2->get_total_points()) {
                return score1->get_total_points() > score2->get_total_points();
            }

            if (score1->get_proc_points() != score2->get_proc_points()) {
                return score1->get_proc_points() > score2->get_proc_points();
            }

            return score1->get_player()->ID < score2->get_player()->ID;
        });

    return res;
}

///////////////////////////////////////////////////////////////////

ScoreByPlayerId::ScoreByPlayerId(ID_t _playerID) : playerID(_playerID) {}

bool ScoreByPlayerId::operator()(const eval_state_t& score)
{
    return score->get_player()->ID == playerID;
}