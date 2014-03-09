/*
 * OptionsScene.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __LEVEL_GAME_SCENE_H__
#define __LEVEL_GAME_SCENE_H__

#include "cocos2d.h"

class LevelGameScene : public cocos2d::CCLayerColor
{
protected:

public:

	LevelGameScene();
	virtual ~LevelGameScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(LevelGameScene);

    void level1Callback(CCObject* pSender);
    void level2Callback(CCObject* pSender);
    void level3Callback(CCObject* pSender);
};

#endif // __LEVEL_GAME_SCENE_H__
