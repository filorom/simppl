//
// integrated broker interface handling
//

#include "include/ipc2.h"
#include "brokerclient.h"
#include "sbroker.h"
#include "calculator.h"


struct CalculatorImpl : Skeleton<Calculator>
{
   CalculatorImpl(const char* role)
    : Skeleton<Calculator>(role)
   {
      clear >> std::tr1::bind(&CalculatorImpl::handleClear, this);
      add >> std::tr1::bind(&CalculatorImpl::handleAdd, this, _1);
      sub >> std::tr1::bind(&CalculatorImpl::handleSub, this, _1);
   }
   
   void handleClear()
   {
      value = 0;
   }
   
   void handleAdd(double d)
   {
      double oldValue = value.value();
      value = oldValue + d;
   }
   
   void handleSub(double d)
   {
      double oldValue = value.value();
      value = oldValue - d;
   }
};


int main(int argc, char** argv)
{
   const char* role = "calculator";
   if (argc > 1)
      role = argv[1];
   
   Dispatcher disp("unix:calculator");
   disp.enableBrokerage();

   CalculatorImpl calc(role);
   disp.addServer(calc);   
   
   return disp.run();
}
