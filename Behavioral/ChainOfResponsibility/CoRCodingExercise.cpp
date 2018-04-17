#include <iostream>
#include <vector>
using namespace std;

struct Creature;
struct Game
{
  vector<Creature*> creatures;
};

struct StatQuery
{
  enum Statistic { attack, defense } statistic;
  int result;
};

struct Creature
{
protected:
  Game& game;
  int base_attack, base_defense;

public:
  Creature(Game &game, int base_attack, int base_defense) : game(game), base_attack(base_attack),
                                                            base_defense(base_defense) {}

  virtual int get_attack() = 0;
  virtual int get_defense() = 0;
  virtual void query(void* source, StatQuery& sq) = 0;
};

class Goblin : public Creature
{
  int get_statistic(StatQuery::Statistic stat)
  {
    StatQuery q{stat,0};
    for (auto c : game.creatures)
      c->query(this, q);
    return q.result;
  }
public:
  Goblin(Game &game, int base_attack, int base_defense) : Creature(game, base_attack, base_defense) {}

  Goblin(Game &game) : Creature(game, 1, 1) {}

  int get_attack() override {
    return get_statistic(StatQuery::attack);
  }

  int get_defense() override {
    return get_statistic(StatQuery::defense);
  }

  void query(void *source, StatQuery &sq) override {
    if (source == this)
    {
      switch (sq.statistic)
      {
        case StatQuery::attack:
          sq.result += base_attack;
          break;
        case StatQuery::defense:
          sq.result += base_defense;
          break;
      }
    } else
    {
      if (sq.statistic == StatQuery::defense)
      {
        sq.result++;
      }
    }
  }
};

class GoblinKing : public Goblin
{
public:
  GoblinKing(Game &game) : Goblin(game, 3, 3) {}

  void query(void *source, StatQuery &sq) override {
    if (source != this && sq.statistic == StatQuery::attack)
    {
      sq.result++;
    }
    else Goblin::query(source, sq);
  }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace
{
  class Evaluate : public testing::Test
  {
  public:
  };

  TEST_F(Evaluate, ManyGoblinsTest)
  {
    Game game;
    Goblin goblin{game};
    game.creatures.push_back(&goblin);

    cout << "Checking that a baseline goblin is a 1/1...\n";

    ASSERT_EQ(1, goblin.get_attack());
    ASSERT_EQ(1, goblin.get_defense());

    cout << "Adding a second goblin, now they should be 1/2...\n";
    Goblin goblin2{game};
    game.creatures.push_back(&goblin2);

    ASSERT_EQ(1, goblin.get_attack());
    ASSERT_EQ(2, goblin.get_defense());

    cout << "Adding a goblin king, now a goblin should be 2/3...\n";
    GoblinKing goblin3{game};
    game.creatures.push_back(&goblin3);

    ASSERT_EQ(2, goblin.get_attack());
    ASSERT_EQ(3, goblin.get_defense());
  }

} // namespace

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}