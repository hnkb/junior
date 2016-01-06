
#include <junior-simple.h>


int main()
{
	create_window(L"Junior example");

	write(L"Hello Junior!", 10, 10);
	draw_circle(500, 300, 40);

	while (wait_for_event());

	return 0;
}
