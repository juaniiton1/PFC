/*
 * RankingScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "RankingScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* RankingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    RankingScene *layer = RankingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

RankingScene::RankingScene()
{
}

RankingScene::~RankingScene()
{
}

// on "init" you need to initialize your instance
bool RankingScene::init()
{
	if ( !CCLayerColor::initWithColor( ccc4(100, 100, 100, 255) ) )
	{
		return false;
	}

	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Fondo con la textura de madera
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
				 texture->setTexParameters(&tp);
	CCSprite* background = CCSprite::createWithTexture(texture, CCRectMake(0, 0, vs.width, vs.height));
			  background->setPosition( ccp( vs.width/2, vs.height/2 ) );

	this->addChild(background, 1);

	CCMenuItem* button_back = CCMenuItemImage::create("btn_aceptar.png", "btn_aceptar_h.png", this, menu_selector(RankingScene::menuBackCallback));
				button_back->setPosition(ccp(vs.width - 105, 60));

	CCMenu* menu = CCMenu::create(button_back, NULL);
	menu->setPosition(ccp(0,0));

	CCString* strLabel = CCString::createWithFormat("%-7s %-18s %-20s", "Pos", "Time", "User");
	CCLabelTTF* label1 = CCLabelTTF::create(strLabel->getCString(), "Helvetica", 40, CCSizeMake(600, 50), kCCTextAlignmentLeft);
				label1->setColor(ccc3(0,0,255));
				label1->setPosition(ccp(vs.width/2, vs.height - 100));
	this->addChild(label1, 5);

	for (int i = 1; i <= 5; i++) {
		CCString* iKeyTime = CCString::createWithFormat("map1time%d", i);
		CCString* iKeyUser = CCString::createWithFormat("map1user%d", i);
		int iTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(iKeyTime->getCString());
		std::string iUser = CCUserDefault::sharedUserDefault()->getStringForKey(iKeyUser->getCString());

		int mins = (iTime%3600000)/60000;
		int ss = ((iTime%3600000)%60000)/1000;
		int ms = ((iTime%3600000)%60000)%1000;

		CCString* strLabel = CCString::createWithFormat("%-10d %d:%02d:%-10.03d %s", i, mins, ss, ms, iUser.c_str());
		CCLabelTTF* label = CCLabelTTF::create(strLabel->getCString(), "Helvetica", 40, CCSizeMake(600, 50), kCCTextAlignmentLeft);
					label->setColor(ccc3(255,0,0));
					label->setPosition(ccp(vs.width/2, vs.height - 100 - (50 * i)));
		this->addChild(label, 5);
	}

	this->addChild(menu, 5);

    this->setTouchEnabled(true);

    return true;
}

void RankingScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
