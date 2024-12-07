#ifndef REPLACE_DIE_WITH_RANDOM_FROM_BAGTC_H
#define REPLACE_DIE_WITH_RANDOM_FROM_BAGTC_H

#include "Die.h"
#include "DieToFieldMove.h"
#include "ToolCard.h"

/**
 * @brief Implementation of the Flux Remover Tool card
 *
 * The user selects a die from the supply and replaces it with a random die
 * from the bag. The player can choose the value of the new die from the bag
 * and must place the die to his board if possible. If the die would not be
 * placeable with any of the values, the die has to be put into the supply.
 */
class ReplaceDieWithRandomFromBagTC : public ToolCard
{
  public:
    /** Default constructor */
    ReplaceDieWithRandomFromBagTC();
    /** Default destructor */
    virtual ~ReplaceDieWithRandomFromBagTC();

    virtual ToolCardUseFeedback use(tool_card_move_t m, Game& g) override;

    virtual void append_possible_moves(ToolCardAppendContext& ctx) override;

    virtual std::unique_ptr<ToolCard> clone() override
    {
        return std::make_unique<ReplaceDieWithRandomFromBagTC>(*this);
    }

    virtual ToolCardUseFeedback undo(tool_card_move_t& m, Game& g) override;

    struct DieToChangeMove : ToolCardMove
    {
        DieToChangeMove(ID_t _playerId, size_t _ind, Die* die,
                        bool _wasPlaceable = false)
            : ToolCardMove(_playerId, _ind)
            , currentlyUsedDie(die)
            , dieInBag(nullptr)
            , wasPlaceable(_wasPlaceable)
        {
        }

        virtual ToolCardMultipleMoveInformation consists_of_multiple_submoves()
            override
        {
            return ToolCardMultipleMoveInformation {
                .isSingleMoveToolCard = false,
                .followUpMoveExists = wasPlaceable};
        }

        virtual void print(std::ostream& os) const override
        {
            os << "ReplaceDieWithRandomFromBagToolCard::DieChangingMove: "
                  "[playerID: "
               << playerId << ", original die: " << currentlyUsedDie;

            if (dieInBag) {
                os << ", die from the bag: " << dieInBag;
            }

            os << "]";
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            auto mv = new ReplaceDieWithRandomFromBagTC::DieToChangeMove(
                playerId, ind, nullptr, wasPlaceable);
            mv->currentlyUsedDie =
                get_corresponding_die(currentlyUsedDie, from, to);
            mv->dieInBag = (dieInBag)
                               ? get_corresponding_die(dieInBag, from, to)
                               : nullptr;
            return mv;
        }

        virtual bool contains_randomness() const override { return true; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            ReplaceDieWithRandomFromBagTC::DieToChangeMove*
                otherDieSwappingMove = dynamic_cast<
                    ReplaceDieWithRandomFromBagTC::DieToChangeMove*>(otherMove);
            if (!otherDieSwappingMove) {
                return false;
            }

            return *currentlyUsedDie ==
                       *otherDieSwappingMove->currentlyUsedDie &&
                   ((dieInBag == nullptr &&
                     otherDieSwappingMove->dieInBag == nullptr) ||
                    (dieInBag != nullptr &&
                     otherDieSwappingMove->dieInBag != nullptr &&
                     *dieInBag == *otherDieSwappingMove->dieInBag)) &&
                   wasPlaceable == otherDieSwappingMove->wasPlaceable;
        }

        Die* currentlyUsedDie;
        Die* dieInBag; // undo operation
        bool wasPlaceable;
    };

    struct ValueChangerAndPlacerMove : ToolCardMove
    {
        ValueChangerAndPlacerMove(ID_t _playerId, size_t _ind, value_t val,
                                  value_t _prevVal, Die* _dieToPlace,
                                  const BoardIndex& _placeInd)
            : ToolCardMove(_playerId, _ind)
            , newVal(val)
            , prevVal(_prevVal)
            , placingMove(_playerId, _dieToPlace, _placeInd, DTFM_MODE_DEFAULT)
        {
        }

        virtual void print(std::ostream& os) const override
        {
            os << "ReplaceDieWithRandomFromBagToolCard::ValueChoosingMove: "
                  "[playerID: "
               << playerId << ", die value changed from " << size_t(prevVal)
               << " to " << size_t(newVal) << " - placing "
               << move_t(&placingMove) << "]";
        }

        virtual move_t clone(const DiceSupply& from,
                             const DiceSupply& to) override
        {
            Die* correspondingDie =
                get_corresponding_die(placingMove.die, from, to);
            return new ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove(
                playerId, ind, newVal, prevVal, correspondingDie,
                placingMove.boardIndices);
        }

        virtual die_to_field_move_t get_placing_submove() override
        {
            return &placingMove;
        }

        virtual bool contains_randomness() const override { return false; }
        virtual bool is_relocating() const override { return false; }

        virtual bool equals(const move_t& otherMove) override
        {
            ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove*
                otherDiePlacingMove = dynamic_cast<
                    ReplaceDieWithRandomFromBagTC::ValueChangerAndPlacerMove*>(
                    otherMove);
            if (!otherDiePlacingMove) {
                return false;
            }

            return newVal == otherDiePlacingMove->newVal &&
                   prevVal == otherDiePlacingMove->prevVal &&
                   placingMove.equals_to_dtfm(otherDiePlacingMove->placingMove);
        }

        value_t newVal;             // value to change the die's value to
        value_t prevVal;            // for undo operation
        DieToFieldMove placingMove; // placing the die
    };

  private:
    using placing_move_t = ValueChangerAndPlacerMove*;
    using replace_move_t = DieToChangeMove*;

    void replace_die_move_part(replace_move_t mv, Game& g);

    // NEW IMPL
    void append_valid_placing_moves(ToolCardAppendContext& ctx);
    void cache_placing_moves_for(Die* die, value_t diePrevVal,
                                 ToolCardAppendContext& ctx);
    void cache_valid_placing_moves(ToolCardAppendContext& ctx);
    void append_die_changing_moves(ToolCardAppendContext& ctx);
    void get_corresponding_moves_from_cache(ToolCardAppendContext& ctx);
    void append_die_placing_moves(ToolCardAppendContext& ctx);

    bool move_exists(replace_move_t& replacingMove, Game& g);
};

#endif // REPLACE_DIE_WITH_RANDOM_FROM_BAGTC_H
