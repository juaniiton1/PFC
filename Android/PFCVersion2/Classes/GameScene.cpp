/*
 * MainMenuScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "NDKHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;

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

GameScene::GameScene():_palette(NULL), _ledsSelected(NULL), scrollView(NULL), _labels(NULL), menu_leds(NULL) //, tableView(NULL)
{

}

GameScene::~GameScene()
{
	if (_ledsSelected) {
		_ledsSelected->release();
	}
	if (_labels) {
		_labels->release();
	}
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Fondo con la textura de madera
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
				 texture->setTexParameters(&tp);
	CCSprite* background = CCSprite::createWithTexture(texture, CCRectMake(0, 0, visibleSize.width, visibleSize.height));
			  background->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 ) );

	this->addChild(background, 1);

	CCSprite* con_smile = CCSprite::create("con_wait.png", CCRectMake(0,0,64,64) );
			  con_smile->setPosition( ccp(visibleSize.width - 64, visibleSize.height - 64) );
	this->addChild(con_smile, 2);

	int aux_nleds = GameManager::sharedGameManager()->getNLeds();
	bool connected = GameManager::sharedGameManager()->getConnected();

	CCMenuItem *btn_clear = CCMenuItemImage::create("btn_clear.png", "btn_clear_h.png", this, menu_selector(GameScene::menuClearCallback));
				btn_clear->setPosition(ccp(visibleSize.width/2, visibleSize.height - 60));

	CCMenuItem *button_back = CCMenuItemImage::create("btn_volver.png", "btn_volver_h.png", this, menu_selector(GameScene::menuBackCallback));
				button_back->setPosition(ccp(origin.x + 105, visibleSize.height - 60));

	CCMenu* menu;

	if (connected) {
		con_smile->setTexture(CCTextureCache::sharedTextureCache()->addImage("con_ok.png"));

		CCMenuItem *button_send = CCMenuItemImage::create("btn_enviar.png", "btn_enviar_h.png", this, menu_selector(GameScene::menuSendCallback));
					button_send->setPosition(ccp(visibleSize.width - 105, origin.y + 60));
		menu = CCMenu::create(button_send, button_back, btn_clear, NULL);

	} else {
		menu = CCMenu::create(button_back, btn_clear, NULL);
	}

	menu->setPosition(ccp(0,0));
	this->addChild(menu, 5);

	scrollView = CCScrollView::create(CCSizeMake(visibleSize.width, visibleSize.height/3));
	scrollView->setContentSize(CCSizeMake(140*aux_nleds, visibleSize.height/3));
	scrollView->setDirection(kCCScrollViewDirectionHorizontal);
	scrollView->setPosition(ccp(10, visibleSize.height/3));
	scrollView->setTouchEnabled(true);
	this->addChild(scrollView, 5);

	menu_leds = CCMenu::create();
	menu_leds->setPosition(ccp(0,0));
	scrollView->addChild(menu_leds, 5);

	_labels = CCArray::create();
	_labels->retain();

	for (int i=0; i<aux_nleds; i++) {

		CCMenuItem *item_led = CCMenuItemImage::create("LED3.png", "LED3.png", this, menu_selector(GameScene::itemLedCallback));
		//CCSprite *sprite = CCSprite::create( "LED3.png", CCRectMake(0,0,120,120) );
		item_led->setAnchorPoint(CCPointZero);
		item_led->setPosition(ccp(140*i + 10, 0));
		item_led->setTag(i);
		menu_leds->addChild(item_led);

		CCString* i_str = CCString::createWithFormat("%d", i+1);
		CCLabelTTF *label = CCLabelTTF::create(i_str->getCString(), "Helvetica", 25, CCSizeMake(50, 30), kCCTextAlignmentCenter);
		label->setPosition(ccp(140*i + 45, 130));
		label->setAnchorPoint(CCPointZero);
		label->setTag(123);
		label->setColor(ccc3(0,0,255));
		label->setHorizontalAlignment(kCCTextAlignmentCenter);
		scrollView->addChild(label);
		_labels->insertObject(label, i);
	}

	_palette = CCSprite::create( "256colour.png", CCRectMake(0,0,512,128) );
	_palette->setPosition( ccp( origin.x + 256, origin.y + 64 ) );
	this->addChild(_palette, 5);

	_ledsSelected = CCArray::create();
	_ledsSelected->retain();

    this->setTouchEnabled(true);

    return true;
}

void GameScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if ( _ledsSelected->count() > 0 and _palette->boundingBox().containsPoint(location) )
	{
		int locX = location.x;
		int locY = location.y;
		CCLOG("X:%d Y:%d", locX, locY);
		int red = (locX / 72) * 36;
		int green = 255 - ( (locY / 18) * 36 );
		int blue = ( (locX % 64) / 16) * 85;

		CCObject *it = NULL;
		CCARRAY_FOREACH(_ledsSelected, it)
		{
			CCMenuItem *led = dynamic_cast<CCMenuItem*>(it);
			led->setColor( ccc3(red, green, blue) );
		}
	}
}

void GameScene::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if ( _ledsSelected->count() > 0 and _palette->boundingBox().containsPoint(location) )
	{
		int locX = location.x;
		int locY = location.y;
		CCLOG("X:%d Y:%d", locX, locY);
		int red = (locX / 72) * 36;
		int green = 255 - ( (locY / 18) * 36 );
		int blue = ( (locX % 64) / 16) * 85;

		CCObject *it = NULL;
		CCARRAY_FOREACH(_ledsSelected, it)
		{
			CCMenuItem *led = dynamic_cast<CCMenuItem*>(it);
			led->setColor( ccc3(red, green, blue) );
		}
	}
}

void GameScene::itemLedCallback(CCObject* pSender)
{
	CCMenuItem *led = (CCMenuItem*) pSender;
	CCLOG("CLICK EN LED CON TAG/INDEX %d", led->getTag());

	if (_ledsSelected->containsObject(led)) {
		// si ya esta seleccionado
		_ledsSelected->removeObject(led);
		CCLabelTTF* label = (CCLabelTTF*) _labels->objectAtIndex(led->getTag());
					label->setColor(ccc3(0,0,255));

	} else {
		// si no esta seleccionado
		_ledsSelected->addObject(led);
		CCLabelTTF* label = (CCLabelTTF*) _labels->objectAtIndex(led->getTag());
					label->setColor(ccc3(0,255,0));
	}
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void GameScene::menuSendCallback(CCObject* pSender)
{
	CCArray* colores = new CCArray;

	int aux_nleds = GameManager::sharedGameManager()->getNLeds();

	for (int i = 0; i < aux_nleds; i++)
	{
		CCMenuItem* led = (CCMenuItem*) menu_leds->getChildByTag(i);
		if (led != NULL) {
			ccColor3B rgb = led->getColor();
			int intRgb = rgb.b + rgb.g*256 + rgb.r*65536;
			colores->addObject(CCString::createWithFormat("%d", intRgb));
		}
	}

	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::createWithFormat("%d", aux_nleds), "nLeds");
	prms->setObject(colores, "colores");

	SendMessageWithParams(string("SendColors"), prms);

	colores->release();
}

void GameScene::menuClearCallback(CCObject* pSender)
{
	CCObject *it = NULL;
	CCARRAY_FOREACH(_ledsSelected, it)
	{
		CCMenuItem* led = dynamic_cast<CCMenuItem*>(it);
		CCLabelTTF* label = (CCLabelTTF*) _labels->objectAtIndex(led->getTag());
		label->setColor(ccc3(0,0,255));
	}
	_ledsSelected->removeAllObjects();
}
