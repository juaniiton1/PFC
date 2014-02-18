/*
 * GameManager.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "GameManager.h"

USING_NS_CC;

GameManager* GameManager::mySingleton = NULL;

GameManager::GameManager()
{
	connected = false;
	ip = "0.0.0.0";
	port = "0";
	nLeds = 4;
}

GameManager* GameManager::sharedGameManager()
{
	//If the singleton has no instance yet, create one
	if(NULL == mySingleton)
	{
		//Create an instance to the singleton
		mySingleton = new GameManager();
	}

	//Return the singleton object
	return mySingleton;
}

bool GameManager::getConnected()
{
	return connected;
}

void GameManager::setConnected(bool b)
{
	connected = b;
}

char* GameManager::getIp()
{
	return ip;
}

void GameManager::setIp(const char* c)
{
	ip = (char*) c;
}

char* GameManager::getPort()
{
	return port;
}

void GameManager::setPort(const char* c)
{
	port = (char*) c;
}

int GameManager::getNLeds()
{
	return nLeds;
}

void GameManager::setNLeds(int n)
{
	nLeds = n;
}
