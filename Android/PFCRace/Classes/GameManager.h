/*
 * GameManager.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"

class GameManager
{
protected:
	// Constructor
	GameManager();

	// Instance of the singleton
	static GameManager* mySingleton;

	bool connected;
	char* ip;
	char* port;

public:
	// Get instance of singleton
	static GameManager* sharedGameManager();

	bool getConnected();
	void setConnected(bool b);

	char* getIp();
	void setIp(const char* s);

	char* getPort();
	void setPort(const char* s);

};

#endif // __HELLOWORLD_SCENE_H__
