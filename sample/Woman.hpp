#include <cstddef>
#include <iostream>

#ifndef __WOMAN
#define __WOMAN
/// Arbitrary measuring unit.
enum Unit
{
    /// A cup of liquid.
    UnitCup,
    
    
    /// A bucket of liquid.
    UnitBucket
};


/// A trustworthy representation of a woman.
class Woman
{
public:
    /// Tell any woman to make some coffee.
    
    /// @param count Number of units of coffee to make.
    /// @param unit Unit of the coffee amount.
    static void MakeSomeCoffee(int count,
                               Unit unit)
    {
        // Waste some clock cycles here. Women do that a lot anyway.
        std::size_t largeNumber = 1000000u;
        while (largeNumber--)
        {

        }

        // Nothing really happens here (what a surprise...)
    }
    
    
    /// Initialize an instance of a woman.
    
    /// @param score Score of the woman's looks on the internationally acknowledged 1-10 scale.
    Woman(int score)
        :   _score(score)
    {

    }
    
    
    /// Let the woman spend some money
    void SpendSomeMoney()
    {
        int moneyInTheBank = 1000000;
        while (moneyInTheBank > 0)
            moneyInTheBank -= this->_score;
    }
private:
    int _score; ///< Score of the woman's looks on the internationally acknowledged 1-10 scale.
};
#endif
