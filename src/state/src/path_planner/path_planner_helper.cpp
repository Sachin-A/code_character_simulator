#include "path_planner/path_planner_helper.h"
#include "actor/actor.h"

namespace state {

FormationMaker::FormationMaker(): formation_id(0) {};

bool IsValidFormation(
	FormationMaker * formation_maker,
	int64_t formation_size
) {
	auto formation = formation_maker->ReturnFormation(formation_size);
	return (formation.size() == formation_size
		&& formation[0] == physics::Vector2D(0,0));
}

PathPlannerHelper::PathPlannerHelper() : leader(NULL) {}
PathPlannerHelper::PathPlannerHelper(std::shared_ptr<Actor> self) :
	self(self), leader(NULL), is_path_planning(false) {}

void PathPlannerHelper::SetPath(
	int64_t formation_id,
	physics::Vector2D relative_position,
	bool is_leader,
	std::shared_ptr<Actor> leader
) {
	this->formation_id = formation_id;
	this->relative_position = relative_position;
	if (is_leader == false) {
		this->leader = leader;
	}
	else { this->leader = NULL; }
	auto self = this->self.lock();
	is_path_planning = true;
}

void PathPlannerHelper::FinishPath() {
	auto self = this->self.lock();
	self->SetVelocity(physics::Vector2D(0, 0));
	is_path_planning = false;
}

int64_t PathPlannerHelper::GetFormationId() {
	return formation_id;
}

bool PathPlannerHelper::IsLeader() {
	if (is_path_planning == true && leader == NULL) {
		return true;
	}
	return false;
}

void PathPlannerHelper::UpdateLeader(
	std::shared_ptr<Actor> new_leader
) {
	leader = new_leader;
}

void PathPlannerHelper::MakeLeader() {
	leader = NULL;
}

void PathPlannerHelper::UpdateRelativePosition(
	physics::Vector2D relative_position
) {
	this->relative_position = relative_position;
}

physics::Vector2D PathPlannerHelper::GetDestination() {
	auto self = this->self.lock();
	physics::Vector2D to_dest;
	if (IsLeader()) {
		to_dest = leader_destination;
	}
	else {
		to_dest = leader->GetPosition() + relative_position;
	}
	return to_dest;
}

bool PathPlannerHelper::IsPathPlanning() {
	return is_path_planning;
}

bool PathPlannerHelper::IsInFormation() {
	if (IsLeader()) {
		return true;
	}

	auto self = this->self.lock();
	if (self->GetPosition().distance(
			leader->GetPosition() + relative_position
		) <= self->GetSize() * 2) {
		return true;
	}
	else {
		return false;
	}
}

void PathPlannerHelper::Update(
	std::vector<std::shared_ptr<Actor> > &sorted_allies,
	std::vector<std::shared_ptr<Actor> > &sorted_enemies,
	physics::Vector2D destination,
	float speed
) {
	auto self = this->self.lock();
	this->leader_destination = destination;
	auto to_dest = GetDestination() - self->GetPosition();
	if (to_dest.magnitude() != 0) {
		to_dest = to_dest / to_dest.magnitude();
	}
	self->SetVelocity(to_dest * speed);
}

void PathPlannerHelper::MergeWithBuffer(
	const PathPlannerHelper& path_planner_helper,
	std::vector<std::shared_ptr<Actor> > actors
) {
	if (!self.expired()) {
		self = actors[self.lock()->GetId()];
	}
	if (leader) {
		leader = actors[leader->GetId()];
	}
}

}
