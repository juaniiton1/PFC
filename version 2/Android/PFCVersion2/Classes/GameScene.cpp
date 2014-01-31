/*
 * MainMenuScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "NDKHelper.h"

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameScene::GameScene():_leds(NULL), _palette(NULL), _ledSelected(NULL), _labelConnected(NULL)
{
}

GameScene::~GameScene()
{
	if (_leds) {
		_leds->release();
		_leds = NULL;
	}
	if (_palette) {
		_palette = NULL;
	}
	if (_ledSelected) {
		_ledSelected = NULL;
	}
	if (_labelConnected) {
		_labelConnected = NULL;
	}
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if ( !CCLayerColor::initWithColor( ccc4(100, 100, 100, 255) ) )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	_leds = new CCArray;

	CCSprite *led1 = CCSprite::create( "LED.png", CCRectMake(0,0,120,120) );
	led1->setPosition( ccp( origin.x + 60, origin.y + (visibleSize.height/3) * 2) );
	_leds->addObject(led1);
	this->addChild(led1);

	CCSprite *led2 = CCSprite::create( "LED.png", CCRectMake(0,0,120,120) );
	led2->setPosition( ccp( origin.x + 180, origin.y + (visibleSize.height/3) * 2) );
	_leds->addObject(led2);
	this->addChild(led2);

	CCSprite *led3 = CCSprite::create( "LED.png", CCRectMake(0,0,120,120) );
	led3->setPosition( ccp( origin.x + 300, origin.y + (visibleSize.height/3) * 2) );
	_leds->addObject(led3);
	this->addChild(led3);

	CCSprite *led4 = CCSprite::create( "LED.png", CCRectMake(0,0,120,120) );
	led4->setPosition( ccp( origin.x + 420, origin.y + (visibleSize.height/3) * 2) );
	_leds->addObject(led4);
	this->addChild(led4);

	CCSprite *led5 = CCSprite::create( "LED.png", CCRectMake(0,0,120,120) );
	led5->setPosition( ccp( origin.x + 540, origin.y + (visibleSize.height/3) * 2) );
	_leds->addObject(led5);
	this->addChild(led5);

	_palette = CCSprite::create( "256colour.png", CCRectMake(0,0,512,128) );
	// palette->setPosition( ccp( origin.x + visibileSize.height/2, origin.y + visibleSize.height/3 ) );
	_palette->setPosition( ccp( origin.x + 256, origin.y + 64 ) );
	this->addChild(_palette);

	bool connected = GameManager::sharedGameManager()->getConnected();

	if (connected) {
		_labelConnected = CCLabelTTF::create("Conectado!", "Helvetica", 30,
											 CCSizeMake(250, 40), kCCTextAlignmentCenter);
		_labelConnected->setColor(ccc3(0,255,0));

	} else {
		_labelConnected = CCLabelTTF::create("No conectado", "Helvetica", 30,
											 CCSizeMake(250, 40), kCCTextAlignmentCenter);
		_labelConnected->setColor(ccc3(255,0,0));
	}

	_labelConnected->setPosition(ccp(origin.x + visibleSize.width / 2,
									 origin.y + visibleSize.height - 40 ));
	this->addChild(_labelConnected);

	CCMenuItemFont *button_back = CCMenuItemFont::create("Volver al menu", this,
										menu_selector(GameScene::menuBackCallback));
	button_back->setPosition(ccp(origin.x + 100, origin.y + visibleSize.height - 30));

	CCMenu* menu;

	if (connected) {
		CCMenuItemFont *button_send = CCMenuItemFont::create("Enviar colores", this,
										menu_selector(GameScene::menuSendCallback));
		button_send->setPosition(ccp(origin.x + visibleSize.width - 100, origin.y + 30));
		menu = CCMenu::create(button_send, button_back, NULL);

	} else {
		menu = CCMenu::create(button_back, NULL);
	}

	menu->setPosition(ccp(0,0));
	// menu->alignItemsVertically();
	// menu->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + (visibleSize.height/5) * 1));
	this->addChild(menu);

    this->setTouchEnabled(true);

    return true;
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if ( _ledSelected != NULL and _palette->boundingBox().containsPoint(location) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
		int locX = location.x;
		int locY = location.y;
		CCLOG("X:%d Y:%d", locX, locY);
		int red = (locX / 72) * 36;
		int green = 255 - ( (locY / 18) * 36 );
		int blue = ( (locX % 64) / 16) * 85;
		_ledSelected->setColor( ccc3(red, green, blue) );

	} else
	{
		CCObject *it = NULL;
		int i = 1;
		CCARRAY_FOREACH(_leds, it)
		{
			CCSprite *led = dynamic_cast<CCSprite*>(it);
			if (led->boundingBox().containsPoint(location))
			{
				_ledSelected = led;
				//led->setColor( ccc3(255,0,0) );
			}
			i++;
		}
	}
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void GameScene::menuSendCallback(CCObject* pSender)
{
	CCArray* colores;
	CCObject *it = NULL;
	CCARRAY_FOREACH(_leds, it)
	{
		CCSprite *led = dynamic_cast<CCSprite*>(it);
		ccColor3B rgb = _ledSelected->getColor();
		CCArray* uno_colores;
		uno_colores->addObject(rgb.r);
		uno_colores->addObject(rgb.g);
		uno_colores->addObject(rgb.b);
		colores->addObject(uno_colores);
	}

	CCDictionary* prms = CCDictionary::create();
	prms->setObject(5, "nLeds");
	prms->setObject(colores, "colores");

	SendMessageWithParams(string("SendColors"), prms);
}
