#include <Canvas.h>

Color::Color()
{

}

Color::Color(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
}

Color::~Color()
{

}

Canvas::Canvas(int width, int height) :
	_WIDTH(width),
	_HEIGHT(height),
	_line_color(0,0,0),
	_fill_color(1,1,1)
{
	// Allocate data
	_pixel_data.resize(_WIDTH * _HEIGHT);

	// Reset data
	for (int j = 0; j < _HEIGHT; ++j)
	{
		for (int i = 0; i < _WIDTH; ++i)
		{
			Color c;
			c.r = c.g = c.b = 1;
			_pixel_data[i + j * _WIDTH] = c;
		}
	}
}

Canvas::~Canvas()
{
}

void Canvas::drawLine(int from_x, int from_y, int to_x, int to_y)
{
	double diff_x = (to_x - from_x);
	double diff_y = (to_y - from_y);
	
	double distance = sqrt(diff_x * diff_x + diff_y * diff_y);
	// Step one pixel at the time
	for (int i = 0; i < distance; ++i)
	{
		int x_idx = from_x + (diff_x / distance) * i;
		int y_idx = from_y + (diff_y / distance) * i;

		if (x_idx < 0 || x_idx >= _WIDTH ||
			y_idx < 0 || y_idx >= _HEIGHT)
		{
			continue;
		}
		else
		{
			_pixel_data[x_idx + y_idx * _WIDTH] = _line_color;
		}
	}
}

void Canvas::drawPoint(int pos_x, int pos_y, int size)
{
	int min_x = pos_x - size / 2;
	int min_y = pos_y - size / 2;
	int max_x = pos_x + size / 2;
	int max_y = pos_y + size / 2;
	
	fillRectangle(min_x, min_y, max_x, max_y);
}

void Canvas::fillRectangle(int min_x, int min_y, int max_x, int max_y)
{
	for (int j = min_y; j <= max_y; ++j)
	{
		for (int i = min_x; i <= max_x; ++i)
		{
			if (i < 0 || i >= _WIDTH ||
				j < 0 || j >= _HEIGHT)
			{
				continue;
			}
			else
			{
				if (i == min_x || i == max_x ||
					j == min_y || j == max_y)
				{ // On border, use line color
					_pixel_data[i + j * _WIDTH] = _line_color;
				}
				else 
				{ // Inside, use fill color
					_pixel_data[i + j * _WIDTH] = _fill_color;
				}
			}
		}
	}
}

void Canvas::setPixel(int i, int j, Color c)
{
	if (i >= 0 && i < _WIDTH &&
		j >= 0 && j < _HEIGHT)
	{
		_pixel_data[i + j * _WIDTH] = c;
	}
}

void Canvas::fill(Color c)
{
	for (int j = 0; j < _HEIGHT; ++j)
	{
		for (int i = 0; i < _WIDTH; ++i)
		{
			_pixel_data[i + j * _WIDTH] = c;
		}
	}
}

// Getters
Color Canvas::lineColor()
{
	return _line_color;
}

Color Canvas::fillColor()
{
	return _fill_color;
}

int Canvas::width()
{
	return _WIDTH;
}

int Canvas::height()
{
	return _HEIGHT;
}

Color Canvas::pixel(int i, int j)
{
	return _pixel_data[i + j * _WIDTH];
}

// Setters
void Canvas::setLineColor(Color c)
{
	_line_color = c;
}

void Canvas::setFillColor(Color c)
{
	_fill_color = c;
}
