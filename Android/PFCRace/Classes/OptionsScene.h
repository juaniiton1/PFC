/*
 * OptionsScene.h
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Color.h"
#include <vector>

class OptionsScene : public cocos2d::CCLayerColor
{
protected:
	cocos2d::CCLabelTTF* label_connected;
	cocos2d::CCLabelTTF* label_control;
	cocos2d::CCLabelTTF* label_nleds;
    cocos2d::extension::CCEditBox* input_ip;
    cocos2d::extension::CCEditBox* input_port;
    cocos2d::extension::CCEditBox* input_user;
    cocos2d::CCSprite* con_smile;
    cocos2d::CCSprite* color_preview;
	std::vector<Color> colors;

public:

	OptionsScene();
	virtual ~OptionsScene();

    virtual bool init();
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(OptionsScene);

    void connectionOk(CCNode *sender, void *data);
    void connectionError(CCNode *sender, void *data);

    void menuConnectCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);

	void colorNextCallback(CCObject* pSender);
	void colorPrevCallback(CCObject* pSender);

	void nledsMoreCallback(CCObject* pSender);
	void nledsLessCallback(CCObject* pSender);

	void controlNextCallback(CCObject* pSender);
	void controlPrevCallback(CCObject* pSender);

};

#endif // __OPTIONS_SCENE_H__
