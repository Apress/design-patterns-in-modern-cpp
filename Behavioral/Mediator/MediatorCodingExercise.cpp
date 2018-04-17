#include <iostream>
#include <vector>
using namespace std;

struct IParticipant
{
    virtual void notify(IParticipant* sender, int value) = 0;
};

struct Mediator
{
    vector<IParticipant*> participants;
    void say(IParticipant* sender, int value)
    {
      for (auto p : participants)
        p->notify(sender, value);
    }
};

struct Participant : IParticipant
{
    int value{0};
    Mediator& mediator;

    Participant(Mediator &mediator) : mediator(mediator)
    {
      mediator.participants.push_back(this);
    }

    void notify(IParticipant *sender, int value) override {
      if (sender != this)
        this->value += value;
    }

    void say(int value)
    {
      mediator.say(this, value);
    }
};

#include "gtest/gtest.h"
#include "helpers/iohelper.h"

#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, LongMediationTest) {
      Mediator m;
      Participant p1{m}, p2{m};

      ASSERT_EQ(0, p1.value);
      ASSERT_EQ(0, p2.value);

      p1.say(2);

      ASSERT_EQ(0, p1.value);
      ASSERT_EQ(2, p2.value);

      p2.say(4);

      ASSERT_EQ(4, p1.value);
      ASSERT_EQ(2, p2.value);
    }

}  // namespace
