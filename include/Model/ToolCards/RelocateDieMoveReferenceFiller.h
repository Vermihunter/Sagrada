#ifndef RELOCATE_DIE_MOVE_REFERENCE_FILLER_H
#define RELOCATE_DIE_MOVE_REFERENCE_FILLER_H

#include "DieToFieldMoveMode.h"
#include "RelocateDieMove.h"
#include "Typedefs.h"

class RelocateDieMoveReferenceFiller {
  public:
	static void AddRelocateMove(std::vector<RelocateDieMove>& dest, ID_t _playerId, size_t _ind,
	                            const BoardIndex& _from, const BoardIndex& _to,
	                            dtfm_mode_mask_t _modeMask)
	{
		dest.push_back(RelocateDieMove{_playerId, _ind, _from, _to, _modeMask});
	}
};

#endif // RELOCATE_DIE_MOVE_REFERENCE_FILLER_H