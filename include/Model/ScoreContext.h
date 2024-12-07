#ifndef SCORE_CONTEXT_H
#define SCORE_CONTEXT_H

#include <memory>

/**
 * @brief A score configuration that is used for the given game
 *
 */
struct ScoreContext
{
    ScoreContext() = default;
    ScoreContext(const ScoreContext&) = default;

    ScoreContext(int _pointsPerUnusedFavorToken, int _minusPointsPerEmptyField,
                 int _pointsPerProcValue)
        : pointsPerUnusedFavorToken(_pointsPerUnusedFavorToken)
        , minusPointsPerEmptyField(_minusPointsPerEmptyField)
        , pointsPerProcValue(_pointsPerProcValue)
    {
    }

    const int pointsPerUnusedFavorToken;
    const int minusPointsPerEmptyField;
    const int pointsPerProcValue;

    auto clone() { return std::make_unique<ScoreContext>(*this); }
};

using score_ctx_t = std::unique_ptr<ScoreContext>;

#endif // SCORE_CONTEXT_H