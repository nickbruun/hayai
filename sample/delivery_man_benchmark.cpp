#include <hayai.hpp>
#include "delivery_man.hpp"

BENCHMARK(DeliveryMan, DeliverPackage, 10, 100)
{
    DeliveryMan(1).DeliverPackage(100);
}
