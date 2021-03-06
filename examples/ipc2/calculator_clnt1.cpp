//
// example of integrated broker interface handling
//

#include "calculator.h"

#include <pthread.h>


static const char* rolename = "calculator";
   
struct CalculatorClient;
static CalculatorClient* calc = 0;


struct CalculatorClient : Stub<Calculator>
{
   CalculatorClient()
    : Stub<Calculator>(rolename, "auto:")
   {
      // NOOP
   }
   
   void connected()
   {
      value.attach() >> std::tr1::bind(&CalculatorClient::valueChanged, this, _1);
      calc = this;
   }
   
   void valueChanged(double d)
   {
      std::cout << "Now having " << d << std::endl;
   }
};


void* threadRunner(void* arg)
{
   Dispatcher disp;
   disp.enableBrokerage();
   
   CalculatorClient clnt;
   disp.addClient(clnt);
   
   disp.run();
   return 0;
}


int main(int argc, char** argv)
{
   if (argc > 1)
      rolename = argv[1];
   
   pthread_t thread;
   pthread_create(&thread, 0, threadRunner, 0);
   
   // wait for calculator to be ready
   while(!calc)
      usleep(100);
   
   char arg;
   double d;
   
   bool finished = false;
   while(!finished)
   {
      std::cout << "Input: q(uit),c(lear),+-<value> > ";
      std::cin >> arg;
      switch(arg)
      {
         case '+':
            // FIXME make this inherent thread-safe if configured so
            std::cin >> d;
            calc->add(d);
            break;
            
         case '-':
            std::cin >> d;
            calc->sub(d);
            break;
         
         case 'c':
            std::cout << "Clear" << std::endl;
            calc->clear();
            break;
         
         case 'q':
            std::cout << "Quit" << std::endl;
            finished = true;
            break;
            
         default:
            std::cout << "Unknown command: " << arg << std::endl;
            break;
      }
   }   
}
