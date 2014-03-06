/*
 * OptionsScene.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __RANKING_SCENE_H__
#define __RANKING_SCENE_H__

#include "cocos2d.h"

class RankingScene : public cocos2d::CCLayerColor
{
protected:

public:

	RankingScene();
	virtual ~RankingScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(RankingScene);

    void menuBackCallback(CCObject* pSender);
};

#endif // __OPTIONS_SCENE_H__
