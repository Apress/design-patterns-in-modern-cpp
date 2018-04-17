#include <iostream>
#include <string>
#include <typeindex>
#include <map>
#include <functional>
using namespace std;

struct GameObject;
void collide(GameObject& first, GameObject& second);

struct GameObject
{
	virtual ~GameObject() = default;
	virtual type_index type() const = 0;

	virtual void collide(GameObject& other) { ::collide(*this, other); }
};

template <typename T> struct GameObjectImpl : GameObject
{
	type_index type() const override
	{
		return typeid(T);
	}
};

struct Planet : GameObjectImpl<Planet> {};
struct Asteroid : GameObjectImpl<Asteroid> {};
struct Spaceship : GameObjectImpl<Spaceship> {};
struct ArmedSpaceship : Spaceship
{
	type_index type() const override {
		return typeid(ArmedSpaceship); // required for collision to function
	}
}; // model limitation

void spaceship_planet() { cout << "spaceship lands on planet\n"; }
void asteroid_planet() { cout << "asteroid burns up in atmosphere\n"; }
void asteroid_spaceship() { cout << "asteroid hits and destroys spaceship\n"; }
void asteroid_armed_spaceship() { cout << "spaceship shoots asteroid\n"; }

map<pair<type_index,type_index>, void(*)(void)> outcomes{
	{{typeid(Spaceship), typeid(Planet)}, spaceship_planet},
	{{typeid(Asteroid),typeid(Planet)}, asteroid_planet},
	{{typeid(Asteroid),typeid(Spaceship)}, asteroid_spaceship},
	{{typeid(Asteroid), typeid(ArmedSpaceship)}, asteroid_armed_spaceship}
};

void collide(GameObject& first, GameObject& second)
{
	auto it = outcomes.find({ first.type(), second.type() });
	if (it == outcomes.end())
	{
		it = outcomes.find({ second.type(), first.type() });
		if (it == outcomes.end())
		{
			cout << "objects pass each other harmlessly\n";
			return;
		}
	}
	it->second();
}

int main__(int argc, char* argv[])
{
	ArmedSpaceship spaceship;
	Asteroid asteroid;
	Planet planet;

	collide(planet, spaceship);
	collide(planet, asteroid);
	collide(spaceship, asteroid);
	collide(planet, planet);

	cout << "Member collision:\n";
	planet.collide(asteroid);

	// but this won't work
	spaceship.collide(planet);

	getchar();
	return 0;
}
