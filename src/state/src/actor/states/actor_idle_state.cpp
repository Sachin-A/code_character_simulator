#include "actor/states/actor_idle_state.h"
#include "actor/states/actor_path_planning_state.h"
#include "actor/states/actor_attack_state.h"
#include "actor/states/actor_dead_state.h"
#include "actor/actor.h"
#include "path_planner/path_planner_helper.h"

namespace state {

void ActorIdleState::Enter(Actor * actor) {}

std::unique_ptr<ActorState> ActorIdleState::Update(
	Actor * actor,
	float delta_time
) {
	if (actor->GetHp() <= 0) {
		return std::unique_ptr<ActorState>(new ActorDeadState());
	}
	else if (actor->GetAttackTarget() != nullptr) {
		return std::unique_ptr<ActorState>(new ActorAttackState());
	}
	else if (actor->CanPathPlan() && actor->GetPathPlannerHelper()->IsPathPlanning()) {
		return std::unique_ptr<ActorState>(
			new ActorPathPlanningState()
		);
	}

	return nullptr;
}

void ActorIdleState::Exit(Actor * actor) {}

std::unique_ptr<ActorState> ActorIdleState::Clone() {
	return std::unique_ptr<ActorState>(new ActorIdleState(*this));
}

}
