#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:

    HelloWorld();
	virtual ~HelloWorld();

	virtual bool init();
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuPlayCallback(CCObject* pSender);
	void menuSettCallback(CCObject* pSender);
	void menuRankCallback(CCObject* pSender);
	void menuExitCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
