#include <iostream>
#include <string>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost::signals2;


struct Query
{
  string creature_name;
  enum Argument { attack, defense } argument;
  int result;


  Query(const string& creature_name, const Argument argument, const int result)
    : creature_name(creature_name),
      argument(argument),
      result(result)
  {
  }
};

struct Game // mediator
{
  signal<void(Query&)> queries;
};

class Creature
{
  Game& game;
  int attack, defense;
public:
  string name;
  Creature(Game& game, const string& name, const int attack, const int defense)
    : game(game),
      attack(attack),
      defense(defense),
      name(name)
  {
  }
  
  // no need for this to be virtual
  int GetAttack() const
  {
    Query q{ name, Query::Argument::attack, attack };
    game.queries(q);
    return q.result;
  }

  friend ostream& operator<<(ostream& os, const Creature& obj)
  {
    return os
      << "name: " << obj.name
      << " attack: " << obj.GetAttack() // note here
      << " defense: " << obj.defense;
  }
};

class CreatureModifier
{
  Game& game;
  Creature& creature;
public:
  virtual ~CreatureModifier() = default;

  // there is no handle() function

  CreatureModifier(Game& game, Creature& creature)
    : game(game),
      creature(creature)
  {
  }
};

class DoubleAttackModifier : public CreatureModifier
{
  connection conn;
public:
  DoubleAttackModifier(Game& game, Creature& creature)
    : CreatureModifier(game, creature)
  {
    // whenever someone wants this creature's attack,
    // we return DOUBLE the value
    conn = game.queries.connect([&](Query& q)
    {
      if (q.creature_name == creature.name && 
        q.argument == Query::Argument::attack)
        q.result *= 2;
    });
  }

  ~DoubleAttackModifier()
  {
    conn.disconnect();
  }
};

// similar idea, but Query instead of Command
int main(int ac, char* av)
{
  Game game;
  Creature goblin{ game, "Strong Goblin", 2, 2 };

  cout << goblin << endl;

  {
    DoubleAttackModifier dam{ game, goblin };

    cout << goblin << endl;
  }

  cout << goblin << endl;

  getchar();
  return 0;
}
