#include <iostream>
#include <vector>
using namespace std;

struct IRat
{
    virtual void rat_enters(IRat* sender) = 0;
    virtual void rat_dies(IRat* sender) = 0;
    virtual void notify(IRat* target) = 0;
};

struct Game
{
    vector<IRat*> rats;
    virtual void fire_rat_enters(IRat* sender)
    {
      for (auto rat : rats) rat->rat_enters(sender);
    }
    virtual void fire_rat_dies(IRat* sender)
    {
      for (auto rat : rats) rat->rat_dies(sender);
    }
    virtual void fire_notify(IRat* target)
    {
      for (auto rat : rats) rat->notify(target);
    }
};

struct Rat : IRat
{
    Game& game;
    int attack{1};

    Rat(Game &game) : game(game)
    {
      game.rats.push_back(this);
      game.fire_rat_enters(this);
    }

    ~Rat() { game.fire_rat_dies(this); }

    void rat_enters(IRat *sender) override {
      if (sender != this)
      {
        ++attack;
        game.fire_notify(sender);
      }
    }

    void rat_dies(IRat *sender) override {
      --attack;
    }

    void notify(IRat *target) override {
      if (target == this) ++attack;
    }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, SingleRatTest)
    {
      Game game;
      Rat rat{game};
      ASSERT_EQ(1, rat.attack);
    }

    TEST_F(Evaluate, TwoRatTest)
    {
      Game game;
      Rat rat{game};
      Rat rat2{game};
      ASSERT_EQ(2, rat.attack);
      ASSERT_EQ(2, rat2.attack);
    }

    TEST_F(Evaluate, ThreeRatsOneDies)
    {
      Game game;
      Rat rat{game};
      ASSERT_EQ(1, rat.attack);

      Rat rat2{game};
      ASSERT_EQ(2, rat.attack);
      ASSERT_EQ(2, rat2.attack);

      {
        Rat rat3{game};

        ASSERT_EQ(3, rat.attack);
        ASSERT_EQ(3, rat2.attack);
        ASSERT_EQ(3, rat3.attack);
      }

      ASSERT_EQ(2, rat.attack);
      ASSERT_EQ(2, rat2.attack);
    }

}  // namespace

// ERROR ON UDEMY'S END, POST AGAIN

int main(int ac, char* av[])
{
  //::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}