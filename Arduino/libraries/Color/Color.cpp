#include "Arduino.h"
#include "Color.h"

Color::Color()
{

}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
{
  red = r;
  green = g;
  blue = b;
}

void Color::setRed(unsigned char r)
{
  red = r;
}

void Color::setGreen(unsigned char g)
{
  green = g;
}

void Color::setBlue(unsigned char b)
{
  blue = b;
}

unsigned char Color::getRed()
{
  return red;
}

unsigned char Color::getGreen()
{
  return green;
}

unsigned char Color::getBlue()
{
  return blue;
}

void Color::setOneColor(unsigned char color, unsigned char rgb)
{
  if (rgb == 0) {
    red = color;
  } else if (rgb == 1) {
    green = color;
  } else {
    blue = color;
  }
}