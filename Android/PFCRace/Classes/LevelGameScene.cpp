/*
 * RankingScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "LevelGameScene.h"
#include "GameScene.h"
#include "GameManager.h"

USING_NS_CC;

CCScene* LevelGameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelGameScene *layer = LevelGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

LevelGameScene::LevelGameScene()
{
}

LevelGameScene::~LevelGameScene()
{
}

// on "init" you need to initialize your instance
bool LevelGameScene::init()
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

	CCMenuItem* level1 = CCMenuItemImage::create("btn_level_1.png", "btn_level_1_h.png", this, menu_selector(LevelGameScene::level1Callback));
				level1->setPosition(ccp(vs.width/2 - 275, vs.height/2));

	CCMenuItem* level2 = CCMenuItemImage::create("btn_level_2.png", "btn_level_2_h.png", this, menu_selector(LevelGameScene::level2Callback));
				level2->setPosition(ccp(vs.width/2, vs.height/2));

	CCMenuItem* level3 = CCMenuItemImage::create("btn_level_3.png", "btn_level_3_h.png", this, menu_selector(LevelGameScene::level3Callback));
				level3->setPosition(ccp(vs.width/2 + 275, vs.height/2));

	CCMenu* menu = CCMenu::create(level1, level2, level3, NULL);
			menu->setPosition(ccp(0,0));

	this->addChild(menu, 5);

    this->setTouchEnabled(true);

    return true;
}

void LevelGameScene::level1Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(1);
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void LevelGameScene::level2Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(2);
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void LevelGameScene::level3Callback(CCObject* pSender)
{
	GameManager::sharedGameManager()->setLevel(3);
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
