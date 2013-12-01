#include <hayai.hpp>
#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "delivery_man.hpp"

class FastDeliveryManFixture
    :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->FastDeliveryMan = new DeliveryMan(10);
    }

    virtual void TearDown()
    {
        delete this->FastDeliveryMan;
    }

    DeliveryMan* FastDeliveryMan;
};

/*
 * Note _F suffix in macro name.
 */
BENCHMARK_P_F(FastDeliveryManFixture, DeliverPackage, 10, 100,
              (std::size_t distance))
{
    FastDeliveryMan->DeliverPackage(distance);
}

BENCHMARK_P_INSTANCE(FastDeliveryManFixture, DeliverPackage, (1));
BENCHMARK_P_INSTANCE(FastDeliveryManFixture, DeliverPackage, (10));
BENCHMARK_P_INSTANCE(FastDeliveryManFixture, DeliverPackage, (100));
