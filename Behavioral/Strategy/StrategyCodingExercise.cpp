#include <iostream>
#include <vector>
#include <complex>
#include <tuple>
using namespace std;

struct DiscriminantStrategy
{
    virtual double calculate_discriminant(double a, double b, double c) = 0;
};

struct OrdinaryDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) override {
      return b*b - 4*a*c;
    }
};

struct RealDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) override {
      double result = b*b - 4*a*c;
      return result >= 0 ? result : numeric_limits<double>::quiet_NaN();
    }
};

class QuadraticEquationSolver
{
    DiscriminantStrategy& strategy;
public:
    QuadraticEquationSolver(DiscriminantStrategy &strategy) : strategy(strategy) {}

    tuple<complex<double>, complex<double>> solve(double a, double b, double c)
    {
      complex<double> disc{strategy.calculate_discriminant(a,b,c), 0};
      auto root_disc = sqrt(disc);
      return {
          (-b+root_disc) / (2*a),
          (-b-root_disc) / (2*a) };
    };
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, PositiveTestOrdinaryStrategy)
    {
      OrdinaryDiscriminantStrategy strategy;
      QuadraticEquationSolver solver{strategy};
      auto results = solver.solve(1,10,16);
      ASSERT_EQ(complex<double>(-2,0), get<0>(results));
      ASSERT_EQ(complex<double>(-8,0), get<1>(results));
    }

    TEST_F(Evaluate, PositiveTestRealStrategy)
    {
      RealDiscriminantStrategy strategy;
      QuadraticEquationSolver solver{strategy};
      auto results = solver.solve(1,10,16);
      ASSERT_EQ(complex<double>(-2,0), get<0>(results));
      ASSERT_EQ(complex<double>(-8,0), get<1>(results));
    }

    TEST_F(Evaluate, NegativeTestOrdinaryStrategy)
    {
      OrdinaryDiscriminantStrategy strategy;
      QuadraticEquationSolver solver{strategy};
      auto results = solver.solve(1,4,5);
      ASSERT_EQ(complex<double>(-2,1), get<0>(results));
      ASSERT_EQ(complex<double>(-2,-1), get<1>(results));
    }

    TEST_F(Evaluate, NegativeTestRealStrategy)
    {
      RealDiscriminantStrategy strategy;
      QuadraticEquationSolver solver{strategy};
      auto results = solver.solve(1,4,5);
      auto x1 = get<0>(results);
      auto x2 = get<1>(results);
      ASSERT_TRUE(isnan(x1.real()));
      ASSERT_TRUE(isnan(x2.real()));
      ASSERT_TRUE(isnan(x1.imag()));
      ASSERT_TRUE(isnan(x2.imag()));
    }

}  // namespace

// ERROR ON UDEMY'S END, POST AGAIN

int main(int ac, char* av[])
{
  //::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}