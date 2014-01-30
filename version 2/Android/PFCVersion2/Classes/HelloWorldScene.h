#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayerColor
{
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	int _projectilesDestroyed;
	cocos2d::CCArray *_leds;
	cocos2d::CCSprite *_palette;
	cocos2d::CCSprite *_ledSelected;

public:

	HelloWorld();
	~HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    // my functions
    void addTarget();
    void spriteMoveFinished(CCNode* sender);
    void gameLogic(float dt);
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    void chartboostInterstitialCallback(CCObject* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
