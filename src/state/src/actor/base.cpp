#include "actor/base.h"

namespace state {

Base::Base(
		act_id_t id,
		PlayerId player_id,
		int64_t attack,
		int64_t hp,
		int64_t max_hp,
		int64_t max_speed,
		int64_t size,
		int64_t total_respawn_time,
		float time_to_respawn,
		int64_t time_spent_near_base,
		physics::Vector2D position,
		physics::Vector2D velocity,
		int64_t los_radius,
		int64_t attack_speed,
		int64_t base_poisoning_radius,
		int64_t base_poisoning_threshold
	):
	Actor(
		id,
		player_id,
		attack,
		hp,
		max_hp,
		max_speed,
		size,
		total_respawn_time,
		time_to_respawn,
		time_spent_near_base,
		position,
		velocity,
		los_radius,
		attack_speed,
		0,
		ActorType::BASE,
		false,
		false
	),
	base_poisoning_radius(base_poisoning_radius),
	base_poisoning_threshold(base_poisoning_threshold) {};

int64_t Base::GetBasePoisonPenalty(
	std::vector<std::vector<std::shared_ptr<Actor> > > actors
) {
	int64_t opponent_count = 0, player_count = 0;
	for (int i = 0; i <= LAST_PLAYER; i++) {
		for (auto actor : actors[i]) {
			if (actor->GetActorType() != ActorType::TOWER &&
			    actor->GetActorType() != ActorType::BASE &&
			    actor->GetActorType() != ActorType::FLAG &&
			    actor->GetPosition().distance(position) <
			    base_poisoning_radius) {
				if (actor->GetPlayerId() == player_id)
					player_count++;
				else
					opponent_count++;
			}
		}
	}

	int64_t penalty = player_count - opponent_count - base_poisoning_threshold;
	return penalty > 0 ? penalty : 0;
}

int64_t Base::GetBasePoisoningThreshold() {
	return base_poisoning_threshold;
}

int64_t Base::GetBasePoisoningRadius() {
	return base_poisoning_radius;
}

void Base::Update(float delta_time) {};

}
