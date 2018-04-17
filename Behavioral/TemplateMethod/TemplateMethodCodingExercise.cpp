#include <iostream>
#include <vector>
#include <complex>
#include <tuple>
using namespace std;

struct Creature
{
    int attack, health;

    Creature(int attack, int health) : attack(attack), health(health) {}
};

struct CardGame
{
    vector<Creature> creatures;

    CardGame(const vector<Creature> &creatures) : creatures(creatures) {}

    // return -1 if there is no clear winner
    int combat(int creature1, int creature2)
    {
      Creature& first = creatures[creature1];
      Creature& second = creatures[creature2];
      hit(first, second);
      hit(second, first);
      bool first_alive = first.health > 0;
      bool second_alive = second.health > 0;
      if (first_alive == second_alive) return -1;
      return first_alive ? creature1 : creature2;
    }

    virtual void hit(Creature& attacker, Creature& other) = 0;
};

struct TemporaryCardDamageGame : CardGame
{
    TemporaryCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    void hit(Creature &attacker, Creature &other) override {
      auto old_health = other.health;
      other.health -= attacker.attack;
      if (other.health > 0) other.health = old_health;
    }
};

struct PermanentCardDamageGame : CardGame
{
    PermanentCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    void hit(Creature &attacker, Creature &other) override
    {
      other.health -= attacker.attack;
    }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, ImpasseTest)
    {
      Creature c1{1,2};
      Creature c2{1,2};
      TemporaryCardDamageGame game({c1,c2});
      ASSERT_EQ(-1, game.combat(0,1));
      ASSERT_EQ(-1, game.combat(0,1));
    }

    TEST_F(Evaluate, TemporaryMurderTest)
    {
      Creature c1{1,1};
      Creature c2{2,2};
      TemporaryCardDamageGame game({c1,c2});
      ASSERT_EQ(1, game.combat(0,1));
    }

    TEST_F(Evaluate, DoubleMurderTest)
    {
      Creature c1{2,2};
      Creature c2{2,2};
      TemporaryCardDamageGame game({c1,c2});
      ASSERT_EQ(-1, game.combat(0,1)) << "The expectation here is that two 2/2 creatures kill each other";
    }

    TEST_F(Evaluate, PermanentDamageDeathTest)
    {
      Creature c1{1,2};
      Creature c2{1,3};
      PermanentCardDamageGame game({c1, c2});
      ASSERT_EQ(-1, game.combat(0,1)) << "1/2 vs 1/3 should have no winner after first round of combat";
      ASSERT_EQ(1, game.combat(0,1)) << "1/1 vs 1/2 here, so winner should be = 1";
    }

}  // namespace

int main(int ac, char* av[])
{
  //::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}