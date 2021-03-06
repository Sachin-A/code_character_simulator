/**
 * @file attack_rules.h
 * Implementation file of the Attack Strategies and Utilities for a Single Actor Unit per tick
 */
#ifndef AI_ATTACK_RULES_H
#define AI_ATTACK_RULES_H

#include <memory>
#include "player_state_handler/player_state_handler.h"

namespace ai {

/**
 * Class for attack rules for guard strategies and utilities
 */
class AttackRules {

	/**
	 * The formation generally used for attack
	 */
	state::FormationMaker *formation;
public:

	/**
	 * Constructor for attack rules
	 */
	AttackRules();

	/**
	 * The attack strategy for the unit at every tick
	 *
	 * @param[in]  unitId              The unit identifier
	 * @param[in]  state               The state
	 * @param      groupUtilityHolder  The group utility holder
	 * @param[in]  sortedEnemies       The sorted enemies
	 * @param[in]  aiLevel             The ai level
	 */
	void Strategy (
		state::act_id_t unitId,
		std::shared_ptr<state::PlayerStateHandler> state,
		int &groupUtilityHolder,
		std::vector<std::pair<int64_t, int>> sortedEnemies,
		int aiLevel
	);

	/**
	 * The utility function or the transition specifier for the Attack state
	 *
	 * @param[in]  unitId  The unit identifier
	 *
	 * @return     int var (1-GoToAtack, 2-GoToRetreat, 3-GoToExplore, 4-GoToGuard)
	 */
	int Utility(int &groupUtilityHolder);
	
};

}
#endif // AI_ATTACK_RULES_H
