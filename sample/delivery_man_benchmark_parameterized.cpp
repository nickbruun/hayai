#include <hayai.hpp>

#include "delivery_man.hpp"

/*
 * Benchmarks can be parameterized. Here is example for
 * simple test without dedicated fixture.
 *
 * Last macro argument is just method parameters declaration.
 * Number of arguments is not limited, just make sure that
 * brackets around them are present.
 */
BENCHMARK_P(DeliveryMan, DeliverPackage, 10, 100,
            (std::size_t speed, std::size_t distance))
{
    DeliveryMan(speed).DeliverPackage(distance);
}

BENCHMARK_P_INSTANCE(DeliveryMan, DeliverPackage, (1, 10));
BENCHMARK_P_INSTANCE(DeliveryMan, DeliverPackage, (5, 10));
BENCHMARK_P_INSTANCE(DeliveryMan, DeliverPackage, (10, 10));
