/*
 * RankingScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "LevelRankingScene.h"
#include "RankingScene.h"
#include "GameManager.h"

USING_NS_CC;

CCScene* LevelRankingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelRankingScene *layer = LevelRankingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

LevelRankingScene::LevelRankingScene()
{
}

LevelRankingScene::~LevelRankingScene()
{
}

// on "init" you need to initialize your instance
bool LevelRankingScene::init()
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

	CCMenuItem* level1 = CCMenuItemImage::create("btn_level_1.png", "btn_level_1_h.png", this, menu_selector(LevelRankingScene::level1Callback));
				level1->setPosition(ccp(vs.width/2 - 275, vs.height/2));

	CCMenuItem* level2 = CCMenuItemImage::create("btn_level_2.png", "btn_level_2_h.png", this, menu_selector(LevelRankingScene::level2Callback));
				level2->setPosition(ccp(vs.width/2, vs.height/2));

	CCMenuItem* level3 = CCMenuItemImage::create("btn_level_3.png", "btn_level_3_h.png", this, menu_selector(LevelRankingScene::level3Callback));
				level3->setPosition(ccp(vs.width/2 + 275, vs.height/2));

	CCMenu* menu = CCMenu::create(level1, level2, level3, NULL);
			menu->setPosition(ccp(0,0));

	this->addChild(menu, 5);

    this->setTouchEnabled(true);

    return true;
}

void LevelRankingScene::level1Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(1);
	CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}

void LevelRankingScene::level2Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(2);
	CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}

void LevelRankingScene::level3Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(3);
	CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}
