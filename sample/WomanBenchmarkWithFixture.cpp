#include <hayai.hpp>

#include "Woman.hpp"

class WomanFixture
    :   public Hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->HotWoman = new Woman(10);
    }
    
    virtual void TearDown()
    {
        delete this->HotWoman;
    }

    Woman* HotWoman;
};

BENCHMARK_F(WomanFixture, HotWoman_SpendSomeMoney, 10, 100)
{
    HotWoman->SpendSomeMoney();
}
