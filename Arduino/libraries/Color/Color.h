#ifndef Color_h
#define Color_h
#include "Arduino.h"

class Color
{
  private:
	unsigned char red; 
	unsigned char green;
	unsigned char blue;
 public:
 	Color();
    Color(unsigned char r, unsigned char g, unsigned char b);

	void setRed(unsigned char r);
	void setGreen(unsigned char g);
	void setBlue(unsigned char b);

	unsigned char getRed();
	unsigned char getGreen();
	unsigned char getBlue();

	void setOneColor(unsigned char color, unsigned char rgb);
};

#endif
