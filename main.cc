#include "reader.h"

int main(int argc, char **argv) {
	FILE *input_file;
	if (argc != 2) return -1;
	if ((input_file = std::fopen(argv[1], "r")) == NULL)
		return -1;
	reader::Reader r(input_file);
	while (r.ConsumeLine() != NULL) {}
	std::fclose(input_file);
	return 0;
}
