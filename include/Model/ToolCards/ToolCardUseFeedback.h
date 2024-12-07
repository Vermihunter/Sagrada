#ifndef TOOL_CARD_USE_FEEDBACK_H
#define TOOL_CARD_USE_FEEDBACK_H

/**
 * @brief A feedback representing the state after a successful Tool card move
 *
 * Some tool cards don't shift the rounds after successful usage, so both the
 * flags are necessary. One of these Tool cards are RerollAllDiceTC
 */
struct ToolCardUseFeedback
{
    bool shiftRound;
    bool tcUsageFinished;
    bool extractCost;
};

#endif // TOOL_CARD_USE_FEEDBACK_H