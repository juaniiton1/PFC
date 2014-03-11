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

class GameScene : public cocos2d::CCLayer
{
protected:
	cocos2d::CCArray* _ledsSelected;
	cocos2d::CCArray* _labels;
	cocos2d::CCSprite* _palette;
	cocos2d::CCMenu* menu_leds;
	cocos2d::extension::CCScrollView* scrollView;

public:

	GameScene();
	virtual ~GameScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    void menuBackCallback(CCObject* pSender);
    void menuSendCallback(CCObject* pSender);
    void menuClearCallback(CCObject* pSender);
    void itemLedCallback(CCObject* pSender);

    CREATE_FUNC(GameScene);

    // my functions
    void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

};

#endif // __HELLOWORLD_SCENE_H__
