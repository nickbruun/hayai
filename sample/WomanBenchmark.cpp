#include <hayai.hpp>
#include "Woman.hpp"

BENCHMARK(Woman, MakeSomeCoffee, 10, 100)
{
    Woman::MakeSomeCoffee(10,
                          UnitCup);
}
