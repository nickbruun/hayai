#include <hayai.hpp>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include "Woman.hpp"

/*
 * Benchmarks can be parameterized. Here is example for
 * simple test without dedicated fixture.
 */

/*
 * Last macro argument is just method parameters declaration.
 * Number of arguments is not limited, just make sure that
 * brackets around them are present.
 */
BENCHMARK_P(Woman, MakeMoreCoffie, 10, 100, (std::size_t coffieCups))
{
    Woman::MakeSomeCoffee(coffieCups, UnitCup);
}

/*
 * Instantiate three tests to make 1, 2 and 4 cups of coffie.
 * Like true programmers :-)
 */
BENCHMARK_P_INSTANCE(Woman, MakeMoreCoffie, (1));
BENCHMARK_P_INSTANCE(Woman, MakeMoreCoffie, (2));
BENCHMARK_P_INSTANCE(Woman, MakeMoreCoffie, (4));

/*
 * But we can also use custom fixtures with parameterized benchmarks.
 */

/*
 * Base fixture class. Skip it, if you're a busy guy.
 */
class GentlemanFixture
    :   public ::hayai::Fixture
{
protected:
    Woman* PrettyGirl;
    int BankAccount;
public:
    virtual void SetUp()
    {
        PrettyGirl = new Woman(9);
        BankAccount = 30000;
        std::srand(std::time(0));
    }

    virtual void TearDown()
    {
        delete PrettyGirl;
    }

    bool BuyFlowers(Woman* girl, std::size_t times)
    {
        for (std::size_t i = 0; i < times; i++)
            BankAccount -= 100 * girl->getScore() * ((std::rand() % 2) + 1);

        return (std::rand() % 100) > 70;
    }

    bool Date(Woman* girl, std::size_t times)
    {
        for (std::size_t i = 0; i < times; i++)
        {
            BankAccount -= 100 * girl->getScore() * ((std::rand() % 5) + 1);
        }

        return (std::rand() % 100) > 70;
    }

    bool Marry(Woman* girl)
    {
        if (BankAccount <= 0)
            return false;
        girl->SpendSomeMoney();
        return true;
    }
}; /* Large fixture, gentlemans life is so complicated... */

/*
 * Note _F suffix in macro name. Hayai will pick up GentlemanFixture and use it.
 * Also note, that we use here two parameters.
 */
BENCHMARK_P_F(GentlemanFixture, TryToMarry, 10, 100, (std::size_t flowerTimes, std::size_t dateTimes))
{
    bool happy = false;

    if ((BuyFlowers(PrettyGirl, flowerTimes)) &&
        (Date(PrettyGirl, dateTimes)) &&
        (Marry(PrettyGirl)))
        // Be happy :-)
        happy = true;
}

/*
 * Instantiate tests to benchmark different marriage scenarios.
 */
BENCHMARK_P_INSTANCE(GentlemanFixture, TryToMarry, (1, 1));
BENCHMARK_P_INSTANCE(GentlemanFixture, TryToMarry, (3, 1));
BENCHMARK_P_INSTANCE(GentlemanFixture, TryToMarry, (1, 3));
