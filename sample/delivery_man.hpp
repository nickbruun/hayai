#include <cstddef>
#include <iostream>

#ifndef __DELIVERY_MAN
#define __DELIVERY_MAN
/// Delivery man.
class DeliveryMan
{
public:
    /// Deliver a package.

    /// @param distance Distance the package has to travel.
    void DeliverPackage(std::size_t distance)
    {
        // Waste some clock cycles here.
        std::size_t largeNumber = 10000u * distance / _speed;
        volatile std::size_t targetNumber;
        while (largeNumber--)
            targetNumber = largeNumber;
    }


    /// Initialize a delivery man instance.

    /// @param speed Delivery man speed from 1 to 10.
    DeliveryMan(std::size_t speed)
        :   _speed(speed)
    {

    }
private:
    std::size_t _speed; ///< Delivery man speed from 1 to 10.
};
#endif
