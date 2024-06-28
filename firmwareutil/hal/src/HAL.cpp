#include <cstdlib>
#include <cassert>

void abort() {
	while (1) {

	}
}

void __assert (const char *, int, const char *) {
	abort();
}

void __assert_func (const char *, int, const char *, const char *) {
	abort();
}
