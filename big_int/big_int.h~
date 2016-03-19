#define MAX_DIGITS 10

enum Sign {
	MINUS,
	PLUS
};

struct BigInt {
	enum Sign sign;
	size_t mag;
	unsigned int digits[MAX_DIGITS];
};

struct BigInt *new_big_int(int64_t integer);

char *big_int_to_s(struct BigInt *big_int);

#undef MAX_DIGITS
