#ifndef __CURVA_H__
#define __CURVA_H__

class Curva
{
private:
	int dis;
	int lon;
	int fuerza;
	bool izq;

public:

    Curva();
    Curva(int dist, int longi, int fuer, bool izquierda);
	~Curva();

	int getDist();
	int getLong();
	int getFuerza();
	int getIzq();

};

#endif // __CURVA_H__
