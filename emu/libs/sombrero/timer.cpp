#include <algorithm>
#include <climits>
#include <cstdio>
#include <signal.h>
#include <sys/time.h>

#include "timer.h"

vector<Timer*> Timer::timers;
int Timer::nextTick;

void Timer::setupSignalHandler(int period) {
  struct sigaction sigAction;
  sigAction.sa_handler = Timer::signalHandler;
  sigAction.sa_flags = 0;  
  sigemptyset(&sigAction.sa_mask);
  sigaction(SIGALRM, &sigAction, NULL);

  itimerval timerVal;
  timerVal.it_interval.tv_sec = (period / 1000);
  timerVal.it_interval.tv_usec = (period % 1000) * 1000;
  timerVal.it_value = timerVal.it_interval;

  setitimer(ITIMER_REAL, &timerVal, NULL);
}

void Timer::add(Timer *t) {
  if (timers.empty()) {
    setupSignalHandler(t->period);
  }

  timers.push_back(t);
}

void Timer::remove(Timer *t) {
  timers.erase(find(timers.begin(), timers.end(), t));
}

void Timer::signalHandler(int signum) {
  int elapsed = Timer::nextTick;
  int nextTick = INT_MAX;

  if (signum == SIGALRM) {
    for (vector<Timer*>::iterator it = timers.begin(); it != timers.end(); it++) {
      Timer *t = *it;
      t->value -= elapsed;
      if (t->value <= 0) {
        t->signal();
        t->value += t->period;
      }
      nextTick = min(nextTick, t->value);
    }

    if (nextTick != INT_MAX) {
      Timer::nextTick = nextTick;
      setupSignalHandler(nextTick);
    } else {
      setupSignalHandler(0);
    }
  }
}

Timer::Timer(int period) : period(period), value(period) {}

Timer::~Timer() {
  remove(this);
}

void Timer::start() {
  add(this);
}

void Timer::stop() {
  remove(this);
}
