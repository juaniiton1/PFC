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

class GameScene : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
protected:
	cocos2d::CCArray* _leds;
	cocos2d::CCArray* _cellsSelected;
	cocos2d::CCSprite* _palette;
	cocos2d::CCSprite* _ledSelected;
	cocos2d::CCLabelTTF* _labelConnected;
	cocos2d::extension::CCTableView* tableView;
	int _indexLed;

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

    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
};

#endif // __HELLOWORLD_SCENE_H__
