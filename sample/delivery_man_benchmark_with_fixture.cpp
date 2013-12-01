#include <hayai.hpp>

#include "delivery_man.hpp"

class SlowDeliveryManFixture
    :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->SlowDeliveryMan = new DeliveryMan(1);
    }

    virtual void TearDown()
    {
        delete this->SlowDeliveryMan;
    }

    DeliveryMan* SlowDeliveryMan;
};

BENCHMARK_F(SlowDeliveryManFixture, DeliverPackage, 10, 100)
{
    SlowDeliveryMan->DeliverPackage(10);
}
