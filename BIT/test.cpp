#include "../template.cpp"
#include "bit.cpp"

int main() {
	BIT b(10);
	fu(i, 10) b.update(i, i+1);
	WATCH(b.get(0));
	WATCH(b.get(9));
	WATCH(b.get(2, 1));
	WATCH(b.get(1, 2));
	WATCH(b.get(5, 5));
	WATCH(b.get(0, 9));
	WATCH(b.get(1, 9));
	return 0;
}
