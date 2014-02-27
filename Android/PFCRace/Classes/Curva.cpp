#include "Curva.h"

Curva::Curva()
{
	dis = 0;
	lon = 0;
	fuerza = 1;
	izq = false;
}

Curva::Curva(int dist, int longi, int fuer, bool izquierda)
{
	dis = dist;
	lon = longi;
	fuerza = fuer;
	izq = izquierda;
}

Curva::~Curva()
{
}

int Curva::getDist()
{
	return dis;
}

int Curva::getLong()
{
	return lon;
}

int Curva::getFuerza()
{
	return fuerza;
}

int Curva::getIzq()
{
	return izq;
}
