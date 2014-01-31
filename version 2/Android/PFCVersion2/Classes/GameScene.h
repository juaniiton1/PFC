/*
 * MainMenuScene.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::CCLayerColor
{
protected:
	cocos2d::CCArray* _leds;
	cocos2d::CCSprite* _palette;
	cocos2d::CCSprite* _ledSelected;
	cocos2d::CCLabelTTF* _labelConnected;

public:

	GameScene();
	virtual ~GameScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    void menuBackCallback(CCObject* pSender);
    void menuSendCallback(CCObject* pSender);

    CREATE_FUNC(GameScene);

    // my functions
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

#endif // __HELLOWORLD_SCENE_H__
