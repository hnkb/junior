
// Include "junior-simple.h" if you haven't learned about objects and classes yet.
//   Otherwise, include "junior.h"
#include <junior-simple.h>


int main()
{
	// Create a new window and display it on screen.
	// The name of our window (displayed in title-bar) will be "Junior example".
	create_window("Junior example");

	// Write something to the window created in previous line.
	// Use '\n' in your string literals to create a new line of text (a new paragraph).
	write("Hello world!\nI'm using junior to talk to you :)");

	// Draw a circle in our window.
	// The first two parameters are X and Y coordinates of the center, the last parameter is the radius.
	// All these values are in pixels.
	draw_circle(150, 200, 80);

	// Draw more circles
	// Instead of drawing hollow circles with draw_circle(), we use fill_circle() function to draw solid circles
	fill_circle(120, 180, 20);
	fill_circle(180, 180, 20);

	// Draw a line
	// The line will connect two points. The first two parameters define X and Y for first point, the next two
	//   parameters are X and Y of the second point. So, the line will be between (100, 230) and (200, 230).
	draw_line(100, 230, 200, 230);

	// Instead of finishing the program, let the user see our window and interact with it for a while.
	// If you remove the next line, the window will still be displayed, but our program wouldn't wait for user
	//   and will end immediately, giving the user no chance to see it.
	// The wait_for_event() function will report any interaction user has with our window (like mouse click,
	//   mouse move, pressing a keyboard button, etc. -- these are called events). We put this function in a
	//   loop, so the user can do multiple things. If we didn't put it in a loop, our program would end
	//   immediately after the first user interaction (for example, if they moved their mouse over our window).
	//   This function will evaluate to false when user closes the window, and the while loop will end.
	while (wait_for_event());

	return 0;
}
