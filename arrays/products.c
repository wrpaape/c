#include <stdio.h> /* printf */

void
print_array(const char *const restrict name,
	    const int *const restrict array,
	    const size_t count)
{
	if (count == 0) {
		printf("%s = {}\n",
		       name);
		return;
	}

	printf("%s = {\n\t%d",
	       name,
	       array[0]);

	for (size_t i = 1; i < count; ++i)
		printf(", %d",
		       array[i]);

	puts("\n}");
}

void
set_products(int *const restrict products,
	     const int *const restrict integers,
	     const size_t count)
{
	if (count < 2) {
		if (count == 1)
			products[0] = integers[0];

		return;
	}

	int acc_ascend[count];
	int acc_descend[count];
	int acc;
	size_t i;
	size_t j;
	size_t k;
	const size_t i_last = count - 1;

	acc = integers[0];
	i   = 0;

	while (1) {
		acc_ascend[i] = acc;

		if (i == i_last)
			break;

		++i;
		acc *= integers[i];
	}

	acc = integers[i];

	while (1) {
		acc_descend[i] = acc;

		if (i == 0)
			break;

		--i;
		acc *= integers[i];
	}

	j = 1;
	k = 2;

	products[i] = acc_descend[j];

	while (k < count) {
		products[j] = acc_ascend[i] * acc_descend[k];

		i = j;
		j = k;
		++k;
	}

	products[j] = acc_ascend[i];
}


int
main(void)
{
	const int integers[10] = {
		1, 5, 3, 4, 7, 6, 5, 3, 2, 8
	};

	int products[10];

	print_array("integers",
		    &integers[0],
		    10);

	set_products(&products[0],
		     &integers[0],
		     10);

	print_array("products",
		    &products[0],
		    10);

	return 0;
}
