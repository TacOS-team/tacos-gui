#include <vector>

#include "sigslot.h"

using namespace std;
using namespace sigslot;

class Timer {
  public:
    Timer(int period);
    ~Timer();
    void start();
    void stop();

    signal0<> signal;

  private:
    int period;
    int value;  

    static void setupSignalHandler(int period);
    static void signalHandler(int signum);
    static void add(Timer *t);
    static void remove(Timer *t);

    static vector<Timer*> timers;
    static int nextTick;
};
