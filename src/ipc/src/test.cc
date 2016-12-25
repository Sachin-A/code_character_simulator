#include "actor/swordsman.h"
#include "actor/tower.h"
#include "actor/arrow.h"
#include "actor/archer.h"
#include "bits/stdc++.h"
#include "path_planner/graph.h"
#include "path_planner/path_planner.h"
#include "path_planner/path_planner_helper.h"
#include "player_state_handler/player_state_handler.h"
#include "state.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include "main_driver.h"
#include "player1.h"
#include "player2.h"
#include <unistd.h>

using namespace std;
using namespace state;
using namespace physics;
using namespace player;
using namespace player1;
using namespace player2;
using namespace drivers;

const int te_size = 20;

class MyFormation : public FormationMaker {
public:
	MyFormation()
		: FormationMaker()
	{
	}
	vector<Vector2D> ReturnFormation(int64_t formation_size)
	{
		vector<Vector2D> f(formation_size);
		for (int i = 0; i < formation_size; i++)
			f[i] = Vector2D(i * 30, i);
		return f;
	}
};

void AddActor(vector<shared_ptr<Actor> >& actors, Actor* actor)
{
	shared_ptr<Actor> act(actor);
	act->AddPathPlanner(PathPlannerHelper(act));
	actors.push_back(act);
}

void AddActor(vector<shared_ptr<Actor> >& actors, shared_ptr<Actor> actor)
{
	actor->AddPathPlanner(PathPlannerHelper(actor));
	actors.push_back(actor);
}

string ActorTypeToString(ActorType type) {
	switch(type) {
		case ActorType::ARCHER		: return "archer";
		case ActorType::ARROW		: return "arrow";
		case ActorType::BASE		: return "base";
		case ActorType::FLAG		: return "flag";
		case ActorType::KING		: return "king";
		case ActorType::SCOUT		: return "scout";
		case ActorType::SWORDSMAN	: return "sword";
		case ActorType::TOWER		: return "tower";
	}
}

pair<State, vector<vector<shared_ptr<Actor> > > > MakeState(Terrain terrain) {
	vector<std::shared_ptr<Actor> > actors1;
	vector<std::shared_ptr<Actor> > actors2;
	vector<std::shared_ptr<Actor> > arch;
	vector<vector<shared_ptr<Actor> > > sorted_actors;
	vector<vector<shared_ptr<Swordsman> > > swordsmen;
	vector<shared_ptr<Swordsman> > sw;

	vector<shared_ptr<Base> > bases(2);
	vector<shared_ptr<Flag> > flags(2);
	vector<shared_ptr<King> > kings(2);

	for (int i = 6; i < 26; i++) {
		if (i < 10) {
			Actor* actor = new Tower(i, PLAYER1, 50, 100, 100, 10, 10, 0, 0, 0, Vector2D(99*(10-i), 99*(10-i)), Vector2D(0, 0), 1000, 40, 100, 100, 1000, 20, 300, 10);
			AddActor(actors1, actor);
		}
		else if (i < 15) {
			Actor* actor = new Archer(i, PLAYER2, 50, 100, 100, 10, 10, 0, 0, 0, Vector2D(99, 9), Vector2D(0, 0), 1000, 10, 1000, 10, 300, 10);
			AddActor(arch, actor);
		} else {
			Actor* actor = new Swordsman(i, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 388), Vector2D(0, 0), 100, 10, 10);
			AddActor(actors2, actor);
			sw.push_back(static_pointer_cast<Swordsman>(actors2.back()));
		}
	}
	vector<std::shared_ptr<Tower> > t;
	vector<vector<shared_ptr<Tower> > > towers;

	for (auto i: actors1) {
		shared_ptr<Tower> derived = static_pointer_cast<Tower>(i);
		// cout << derived->GetId() << endl;
		t.push_back(derived);
	}

	towers.push_back(t);
	towers.push_back(vector<std::shared_ptr<Tower> >());

	vector<std::shared_ptr<Archer> > a;
	vector<vector<shared_ptr<Archer> > > archers;

	for (auto i: arch) {
		shared_ptr<Archer> derived = static_pointer_cast<Archer>(i);
		// cout << derived->GetId() << endl;
		a.push_back(derived);
	}

	archers.push_back(vector<std::shared_ptr<Archer> >());
	archers.push_back(a);
	actors2.insert(actors2.end(), arch.begin(), arch.end());
	for (int i = 26; i < 36; i++) {
		Actor* actor = new Swordsman(i, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 388), Vector2D(0, 0), 10, 10, 10);
		AddActor(actors1, actor);
		sw.push_back(static_pointer_cast<Swordsman>(actors1.back()));
	}

	swordsmen.push_back(vector<shared_ptr<Swordsman> >());
	swordsmen.push_back(sw);

	flags[0] = shared_ptr<Flag>(new Flag(0, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2, 10));
	flags[1] = shared_ptr<Flag>(new Flag(1, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(100, 300), Vector2D(0, 0), 5, 10));

	kings[0] = shared_ptr<King>(new King(2, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(200, 20), Vector2D(0, 0), 2, 10));
	kings[1] = shared_ptr<King>(new King(3, PLAYER2, 10, 1000, 1000, 10, 10, 10, 0, 0, Vector2D(399, 10), Vector2D(0, 0), 2, 10));

	bases[0] = shared_ptr<Base>(new Base(4, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2, 10));
	bases[1] = shared_ptr<Base>(new Base(5, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(100, 100), Vector2D(0, 0), 2, 10));

	AddActor(actors1, flags[0]);
	AddActor(actors1, kings[0]);
	AddActor(actors1, bases[0]);
	AddActor(actors2, flags[1]);
	AddActor(actors2, kings[1]);
	AddActor(actors2, bases[1]);

	sorted_actors.push_back(actors1);
	sorted_actors.push_back(actors2);

	cout << "SIZE: " << archers[1].size() << "\n";

	State S(
		terrain,
		sorted_actors,
		kings,
		bases,
		flags,
		towers,
		vector<vector<shared_ptr<Scout> > >(2, vector<shared_ptr<Scout> >()),
		archers,
		swordsmen
	);
	return make_pair(S, sorted_actors);
}

vector<shared_ptr<Actor> > GetFlatActors(vector<vector<shared_ptr<Actor> > > sorted_actors) {
	auto actors = flatten(sorted_actors);
	std::sort(actors.begin(), actors.end(),
		[](const std::shared_ptr<Actor>& a1,
			const std::shared_ptr<Actor>& a2) {
			return a1->GetId() < a2->GetId();
		});
	return actors;
}

void PrintLos(Terrain terrain, PlayerId player_id) {
	for(int i = 0; i < terrain.GetRows(); i++) {
		for(int j = 0; j < terrain.GetRows(); j++) {
			cout << terrain.OffsetToTerrainElement(Vector2D(i,j)).GetLos(player_id) << ' ';
		}
		cout << endl;
	}
}

int main(int argc, char const* argv[])
{
	Terrain T(10);

	vector<vector<TerrainElement> > grid;

	for (int i = 0; i < 20; i++) {
		vector<TerrainElement> temp;
		for (int j = 0; j < 20; j++)
			if (j <= 10)
				temp.push_back(TerrainElement(PLAIN, Vector2D(i * te_size, j * te_size), te_size));
			else
				temp.push_back(TerrainElement(MOUNTAIN, Vector2D(i * te_size, j * te_size), te_size));
		grid.push_back(temp);
	}

	Terrain TT(grid);

	shared_ptr<FormationMaker> F(new MyFormation());

	vector<int64_t> weights({ 100, 200, 300 });
	list_act_id_t l({ 6, 7 });

	list_act_id_t s({ 13, 12 });

	auto thing = MakeState(TT);
	auto S = shared_ptr<State>(new State(thing.first));
	auto sorted_actors = thing.second;
	auto actors = GetFlatActors(sorted_actors);

	auto thing1 = MakeState(TT);
	auto S1 = shared_ptr<State>(new State(thing1.first));
	PlayerStateHandler PSH1(S1.get(), PLAYER1);
	auto sorted_actors1 = thing1.second;
	auto actors1 = GetFlatActors(sorted_actors1);

	auto thing2 = MakeState(TT);
	auto S2 = shared_ptr<State>(new State(thing2.first));
	PlayerStateHandler PSH2(S2.get(), PLAYER2);
	auto sorted_actors2 = thing2.second;
	auto actors2 = GetFlatActors(sorted_actors2);

	// S for main, S1 for player 1, S2 for player 2
	// S.MoveUnits(l, Vector2D(12 * te_size, 9 * te_size), F, weights);
	
	MainDriver driver(
		PlayerAi(shared_ptr<PlayerAiHelper>(new Player1())),
		PlayerAi(shared_ptr<PlayerAiHelper>(new Player2())),
		S, S1, S2
	);

	driver.Run();

	int bleep = 0;

	while (true) {
		// PrintLos(S2.GetTerrain(), PLAYER2);
		auto start_time = chrono::high_resolution_clock::now();
		ofstream file("output.txt");
		
		auto arrows = S->GetProjectiles();
		// cout << arrows.size() << endl;
		// for (auto arrow: arrows){
		// 	auto p = arrow->GetPosition();
		// 	cout << arrow->GetId() << p.x << p.y << endl;
		// }

		ipc::StateTransfer(S);

		}

		file << arr.dump() << endl;
		file.flush();
		auto end_time = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
		// cout << duration << endl;
		this_thread::sleep_for(chrono::milliseconds(max((long) 0, 30 - duration)));
		bleep += 30;
		if (bleep / 1000 >= 15) {
			break;
		}
	}

	driver.Stop();

	return 0;
}