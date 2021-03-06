#include <cstdint>
#include "actor/swordsman.h"

namespace state {

Swordsman::Swordsman(
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
		int64_t attack_range
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
		attack_range,
		ActorType::SWORDSMAN,
		true,
		true
	) {}

void Swordsman::Attack() {
	attack_target->Damage(attack);
}

void Swordsman::Update(float delta_time) {
	DecideState(delta_time);
	position = position + velocity * delta_time;
}

}
