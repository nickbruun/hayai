#include <hayai.hpp>
#include "delivery_man.hpp"

BENCHMARK(DeliveryMan, DeliverPackage, 10, 100)
{
    DeliveryMan(1).DeliverPackage(100);
}

BENCHMARK(DeliveryMan, DISABLED_DeliverPackage, 10, 10000)
{
    DeliveryMan(1).DeliverPackage(10000);
}
