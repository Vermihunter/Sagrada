#ifndef I_TOURNAMENT_PRINTER_H
#define I_TOURNAMENT_PRINTER_H

#include "Game.h"
#include "Move.h"
#include "TournamentGameInformation.h"
#include "TournamentStatistics.h"

#include <vector>

struct TournamentConfig;

/**
 * @brief Interface for printing tournament results
 */
class ITournamentPrinter {
  public:
	ITournamentPrinter(const TournamentConfig& _tournamentConfig)
	    : tournamentConfig(_tournamentConfig)
	{
	}

	virtual ~ITournamentPrinter() = default;

	virtual void print_game_result(const TournamentGameInformation& gameInfo, game_result_t& result,
	                               int usedSeed)                       = 0;
	virtual void print_overall_statistics(const TournamentStatistics&) = 0;
	virtual void print_move_statistics(const move_c& moves, Game& g, size_t gameNumber,
	                                   AI_PlayerConfig* playerConfig)  = 0;

  protected:
	const TournamentConfig& tournamentConfig;
};

#endif // I_TOURNAMENT_PRINTER_H