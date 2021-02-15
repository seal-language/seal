#include <stdio.h>

int main() {
	int arrays[4] = { 0, 1, 2, 3 };

	for (int item : arrays) {
		printf("%d ", item);
	}

	return 0;
}