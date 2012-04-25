#include <cstdio>

#include "sigslot.h"
#include "timer.h"

class Toto : public has_slots<> {
	public:
		void blah() {
			printf("blah\n");
		}

		void blih() {
			printf("blih\n");
		}
};

int main() {
	Toto toto;

	Timer t(1100);
	Timer t2(200);

	t.signal.connect(&toto, &Toto::blah);
	t2.signal.connect(&toto, &Toto::blih);

	t.start();
	t2.start();

	while (1);

	return 0;
}
