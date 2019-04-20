/** --------------------------------------------------------
* @file    imgGen.cpp
* @author  Anthony Major
* @version 1.0
*
* CS162-01 - Assignment 1.1
*	* Generates image file in PGM format
*
*	Comments - My type handling is a mess, and I have a lot of conversion warnings. Looking back, I would probably just handle
*	everything as floats or doubles except for the image array, and convert as necessary when actually changing array values.
*
* ------------------------------------------------------------ */

#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

const string FILE_NAME = "image.ppm";

const short IMG_HEIGHT = 800;
const short IMG_WIDTH = 1000;

const double MANDELBROT_WIDTH_RATIO = 1.5;
const double MANDELBROT_HEIGHT_RATIO = 2.0;

struct Color { 
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
};

const Color RED = { 255, 0, 0 };
const Color ORANGE = { 255, 127, 0 };
const Color YELLOW = { 255, 255, 0 };
const Color GREEN = { 0, 255, 0 };
const Color BLUE = { 0, 0, 255 };
const Color INDIGO = { 75, 0, 130 };
const Color VIOLET = { 139, 0, 255 };
const Color BLACK = { 0, 0, 0 };
const Color WHITE = { 255, 255, 255 };

struct Point   { 
	short x;
	short y;
};

struct Complex { 
	double r;
	double i;
};

/*
   Constrains values between 0-255, used for gradients

   @param num- number to be constrained
   @return number between 0-255
*/
short constrain(short num);

/*
   Draws a single horizontal line

   @param  img - the image array	
   @param  left - start point of the line relative to the image array
   @param  right - end point of the line relative to the image array
   @param  height - the y value (height) of the image array to draw the line at
   @param  line_color - the color to draw that particular line
   @return none
*/
void draw_line(Color img[][IMG_WIDTH], short left, short right, short height, Color line_color);

/*
   Draws a rectangle with or without a horizontal gradient, using a loop that calls draw_line
   Uses custom struct Point to denote top left and bottom right of rectangles

   @param  img - the image array
   @param  top_left - (x,y) points that represent top left corner of rectangle
   @param  bottom_right - (x,y) points that represent bottom right corner of rectangle
   @param  start_color - color to start the gradient with
   @param  gradient_value - amount to change color brightness on each line. Negative values = darker shades, Positive = brighter shades
   @return none
*/
void draw_rect(Color img[][IMG_WIDTH], Point top_left, Point bottom_right, Color start_color, short gradient_value = 0, short start_brightness = 255);

/*
   Finds the distance between two points, squared. Used to reduce slow sqrt() calls

   @param  center - center point of circle
   @param  point - point to have distance measured to
   @return short type containing squared distance between points
*/
short distance_sq(Point center, Point point);

/*
   Draws a circle with or without a circular gradient

   @param  img - image array
   @param  center - (x,y) coordinate of circle center
   @param  radius - radius of desired circle
   @param  start_color - color to start the gradient with for the circle
   @param  gradient_value - amount to change color brightness per distance from center. Negative values = darker shades, Positive = brighter shades
   @return none
*/
void draw_circle(Color img[][IMG_WIDTH], Point center, short radius, Color start_color, short gradient_value = 0, short start_brightness = 255);

/*
   Function for squaring a complex number

   @param  z - complex number in the form a + bi
   @return complex number equal to (a + bi)^2
*/
Complex complex_sqare(Complex z);

/*
   Function for adding complex numbers

   @param  a, b - complex numbers to be added
   @return complex number equal to a + b
*/
Complex complex_add(Complex a, Complex b);

/*
   Checks whether a given complex number is in the mandelbrot set (falls within a radius of 2)

   @param  num - complex number
   @return true if number is within set, false if it is not
*/
bool in_set(Complex num);

/*
   Translates a given pixel relative to a new origin, and then scales the pixel 
   to a complex number within mandelbrot set (within a square with side lengths of 2)

   @param  origin - point that will be the new origin of the image
   @param  p - point representing the pixel that will be tested
   @param  width - width of the part of the image that will display the mandelbrot set
   @param  height - height of the part of the image that will display the mandelbrot set
   @return complex number that will either be in the set or not
*/
Complex point_to_complex(Point origin, Point p, short width, short height);

/*
   Iterates a given complex number C through the mandelbrot formula Z(n+1) = Z(n)^2 + C

   @param  num - complex number to be iterated
   @return unsigned char representing a grayscale value 0-255, comes from mandelbrot_coloring()
*/
Color mandelbrot_calc(Complex num);

/*
   Changes a given number of iterations into a color by multiplying by a 
   variable gradient

   @param  iterations - the number of iterations a complex number made before being considered in or out of the set
   @param  max_iterations - the maximum number of iterations performed before a pixel is considered in the set
   @return Color based on the amount of iterations
*/
Color mandelbrot_coloring(short iterations, short max_iterations);

/*
   Turns a section of an image array into a display of the mandelbrot set

   @param  img - image array to be modified
   @param  top_left - top left point within the image array representing a rectangle to display the set
   @param  bottom_right - bottom right point within the image array representing a rectangle to display the set
   @return none
*/
void mandelbrot(Color img[][IMG_WIDTH], Point top_left, Point bottom_right);

/*
   Returns a color of a given brightness

   @param  starting_color - color to scale the brightness of
   @param  brightness - brightness level to scale color to
   @return RGB color scaled to given brightness level
*/
Color color_gradient(Color starting_color, short brightness);

/*
   Draws all shapes and mandelbrot set on the given image array

   @param  img - image array
   @return none
*/
void create_image(Color img[][IMG_WIDTH]);

/*
   Outputs an image file in .pgm format from the image array

   @param  img - image array
   @return true if print to file operation worked, false if not
*/
bool write_image(Color img[][IMG_WIDTH]);

/* ------------------------------------------------------
	MAIN PROGRAM
   ------------------------------------------------------*/
int main()
{
	static Color img_array[IMG_HEIGHT][IMG_WIDTH]{0};

	create_image(img_array);

	bool success = false;
	success = write_image(img_array);
	if (success)
		cout << "Image created successfully.";
	else
		cout << "Image creation fail.";
	cin.get();

	return 0;
}

// ------------------------------------------------------
short constrain(short num) {
	if (num > 255)
		return 255;
	else if (num < 0)
		return 0;
	else
		return num;
}

// ------------------------------------------------------
void draw_line(Color img[][IMG_WIDTH], short left, short right, short height, Color line_color) {
	for (short x = left; x < right; x++){
		img[height][x] = line_color;
	}
}

// ------------------------------------------------------
void draw_rect(Color img[][IMG_WIDTH], Point top_left, Point bottom_right, Color start_color, short gradient_value, short start_brightness ) {
	short brightness = start_brightness;
	for (short y = top_left.y; y < bottom_right.y; y++) {
		draw_line(img, top_left.x, bottom_right.x, y, color_gradient(start_color, brightness));
		brightness += gradient_value;
	}
}

// ------------------------------------------------------
short distance_sq(Point center, Point point) {
	short a = abs(center.x - point.x);
	short b = abs(center.y - point.y);
	short c_squared = (a * a) + (b * b);
	return c_squared;
}

// ------------------------------------------------------
void draw_circle(Color img[][IMG_WIDTH], Point center, short radius, Color start_color, short gradient_value, short start_brightness ) {
	short brightness = start_brightness;
	short r_squared = radius * radius;
	
	for (short y = center.y - radius; y < center.y + radius; y++) {
		for (short x = center.x - radius; x < center.x + radius; x++) {
			Point d = { x,y };
			short d_squared = distance_sq(center, d);
			if ( d_squared <= r_squared ) {
				if (gradient_value != 0) {
					short distance = (short)(round(sqrt(d_squared)));
					brightness = constrain(start_brightness + (distance * gradient_value));
				}
				img[y][x] = color_gradient(start_color, brightness);
			}
		}
	}
}

// ------------------------------------------------------
Complex complex_sqare(Complex z) { // (a + bi)^2 = a^2 - b^2 + 2abi
	double a_sq = z.r * z.r;
	double b_sq = z.i * z.i;
	double abix2 = 2 * z.r * z.i;
	Complex result = { a_sq - b_sq, abix2 };
	return result;
}

// ------------------------------------------------------
Complex complex_add(Complex a, Complex b) {
	double real = a.r + b.r;
	double imaginary = a.i + b.i;
	Complex result = { real, imaginary };
	return result;
}

// ------------------------------------------------------
bool in_set(Complex num) {
	double distance_sq = (num.r * num.r) + (num.i * num.i);
	if (distance_sq > 4.0) // any number outside a circle of radius 2 is not in the mandelbrot set
		return false;
	else
		return true;
}

// ------------------------------------------------------
Complex point_to_complex(Point origin, Point p, short width, short height) {
	double trans_x = (p.x - origin.x);
	double trans_y = (p.y - origin.y);

	double width_per_pixel  = 3.0 / (double)(width);
	double height_per_pixel = 3.0 / (double)(height);

	Complex num = { trans_x * width_per_pixel, trans_y * height_per_pixel };
	return  num;
}

// ------------------------------------------------------
Color mandelbrot_calc(Complex num) {
	const short max_iterations = 50;
	short iterations = 0;
	Complex z;

	z = num;
	
	while (in_set(z) && iterations < max_iterations) {
		z = complex_sqare(z);
		z = complex_add(z, num);
		iterations++;
	} ;
	return mandelbrot_coloring(iterations, max_iterations);
}

// ------------------------------------------------------
Color mandelbrot_coloring(short iterations, short max_iterations) {
	Color c;
	short gradient_value = 10 * (iterations + 1);
	if (iterations == max_iterations)
		return BLACK;
	int rainbow = iterations % 7;

	switch (rainbow) {
		case 1: c = RED;
			break;
		case 2: c = ORANGE;
			break;
		case 3: c = YELLOW;
			break;
		case 4: c = GREEN;
			break;
		case 5: c = BLUE;
			break;
		case 6: c = INDIGO;
			break;
		case 7: c = VIOLET;
			break;
	}
	return color_gradient(c, gradient_value);
}

// ------------------------------------------------------
void mandelbrot(Color img[][IMG_WIDTH], Point top_left, Point bottom_right) {
	double width  = bottom_right.x - top_left.x;
	double height = bottom_right.y - top_left.y;
	Point origin = { (short)round((width / MANDELBROT_WIDTH_RATIO) + top_left.x) , (short)round((height / MANDELBROT_HEIGHT_RATIO) + top_left.y) };

	for (short y = top_left.y; y < bottom_right.y; y++) {
		for (short x = top_left.x; x < bottom_right.x; x++) {
			Point p = { x,y };
			Complex c = point_to_complex(origin, p, width, height); 
			img[y][x] = mandelbrot_calc(c);
		}
	}
}

// ------------------------------------------------------
Color color_gradient(Color starting_color, short brightness) {
	unsigned char red_value = constrain(round(((float)starting_color.red / 255.0f) * brightness));
	unsigned char green_value = constrain(round(((float)starting_color.green / 255.0f) * brightness));
	unsigned char blue_value = constrain(round(((float)starting_color.blue / 255.0f) * brightness));
	Color color = { red_value, green_value, blue_value };
	return color;
}

// ------------------------------------------------------
void create_image(Color img_array[][IMG_WIDTH]) {
	
	Point tl = { 100, 0 };
	Point br = { 900, 800 };
	mandelbrot(img_array, tl, br);

	tl = { 10, 10 };
	br = { 110, 250 };
	draw_rect(img_array, tl, br, BLUE, -1);

	tl = { 890, 10 };
	br = { 990, 250 };
	draw_rect(img_array, tl, br, VIOLET, -1);

	Point cent = { 75, 700 };
	draw_circle(img_array, cent, 70, ORANGE, -3);

	cent = { 925, 700 };
	draw_circle(img_array, cent, 70, YELLOW, -2);

}


// ------------------------------------------------------
bool write_image(Color img[][IMG_WIDTH]){

	ofstream img_file;
	img_file.open(FILE_NAME);
	if (!img_file) {
		return false;
	}

	img_file << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << endl << "255\n";
	for (int y = 0; y < IMG_HEIGHT; y++) {
		for (int x = 0; x < IMG_WIDTH; x++) {
			img_file << (int)(img[y][x].red) << " " << (int)(img[y][x].green) << " " << (int)(img[y][x].blue) << " ";
		}
		img_file << endl;
	}
	return true;
}
