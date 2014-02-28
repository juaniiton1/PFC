/*
 * MainMenuScene.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Curva.h"
#include <vector>

class GameScene : public cocos2d::CCLayer
{
protected:
	cocos2d::CCSprite* _car;
	cocos2d::CCSprite* _road;
	cocos2d::CCSprite* _road2;
	cocos2d::CCSprite* _grass;
	cocos2d::CCLabelTTF* _labelVel;
	cocos2d::CCLabelTTF* _labelDist;
	cocos2d::CCLabelTTF* _labelStart;
	cocos2d::CCArray* _oppCars;
	cocos2d::CCSprite* _piano1;
	cocos2d::CCSprite* _piano2;
	std::vector<Curva> _curvas;
	cocos2d::CCSprite* _curvaI;
	cocos2d::CCSprite* _curvaD;
	float _vel;
	float _dist;
	bool _started;
	int _nextCurva;
	int _fuerzaCurva;

public:

	GameScene();
	virtual ~GameScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    CREATE_FUNC(GameScene);

    // my functions
    void menuBackCallback(CCObject* pSender);
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
    void gameLogic(float dt);
    void updateFrame(float dt);

    void oppCarDelete(cocos2d::CCNode* sender);
    void resumeRace(cocos2d::CCNode* sender);
};

#endif // __HELLOWORLD_SCENE_H__
