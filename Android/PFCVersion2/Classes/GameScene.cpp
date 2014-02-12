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

GameScene::GameScene():_leds(NULL), _palette(NULL), _labelConnected(NULL), _cellsSelected(NULL)
{
	_indexLed = -1;
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
	if (_cellsSelected) {
		_cellsSelected->release();
		_cellsSelected = NULL;
	}
	if (_labelConnected) {
		_labelConnected = NULL;
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
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	texture->setTexParameters(&tp);
	CCSprite *background = CCSprite::createWithTexture(texture, CCRectMake(0, 0, visibleSize.width, visibleSize.height));
	background->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 ) );

	this->addChild(background, 1);

	CCSprite* con_smile = CCSprite::create("con_wait.png", CCRectMake(0,0,64,64) );
	con_smile->setPosition( ccp(visibleSize.width - 64, visibleSize.height - 64) );
	this->addChild(con_smile, 2);

	tableView = CCTableView::create(this, CCSizeMake(visibleSize.width, visibleSize.height/3));
	tableView->setDirection(kCCScrollViewDirectionHorizontal);
	tableView->setPosition(ccp(10, visibleSize.height/3));
	tableView->setDelegate(this);
	this->addChild(tableView, 5);
	tableView->reloadData();

	_palette = CCSprite::create( "256colour.png", CCRectMake(0,0,512,128) );
	_palette->setPosition( ccp( origin.x + 256, origin.y + 64 ) );
	this->addChild(_palette, 5);

	bool connected = GameManager::sharedGameManager()->getConnected();

	if (connected) {
		_labelConnected = CCLabelTTF::create("Conectado!", "Helvetica", 30, CCSizeMake(250, 40), kCCTextAlignmentCenter);
		_labelConnected->setColor(ccc3(0,255,0));

		con_smile->setTexture(CCTextureCache::sharedTextureCache()->addImage("con_ok.png"));

	} else {
		_labelConnected = CCLabelTTF::create("No conectado", "Helvetica", 30, CCSizeMake(250, 40), kCCTextAlignmentCenter);
		_labelConnected->setColor(ccc3(255,0,0));
	}

	_labelConnected->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 40 ));
	this->addChild(_labelConnected, 5);

	CCMenuItem *button_back = CCMenuItemImage::create("btn_volver.png", "btn_volver_h.png", this, menu_selector(GameScene::menuBackCallback));
	button_back->setPosition(ccp(origin.x + 105, visibleSize.height - 60));

	CCMenu* menu;

	if (connected) {
		CCMenuItem *button_send = CCMenuItemImage::create("btn_enviar.png", "btn_enviar_h.png", this, menu_selector(GameScene::menuSendCallback));
		button_send->setPosition(ccp(visibleSize.width - 105, origin.y + 60));
		menu = CCMenu::create(button_send, button_back, NULL);

	} else {
		menu = CCMenu::create(button_back, NULL);
	}

	menu->setPosition(ccp(0,0));
	this->addChild(menu, 5);

	_cellsSelected = new CCArray;

    this->setTouchEnabled(true);

    return true;
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if ( _cellsSelected->count() > 0 and _palette->boundingBox().containsPoint(location) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
		int locX = location.x;
		int locY = location.y;
		CCLOG("X:%d Y:%d", locX, locY);
		int red = (locX / 72) * 36;
		int green = 255 - ( (locY / 18) * 36 );
		int blue = ( (locX % 64) / 16) * 85;

		CCObject *it = NULL;
		CCARRAY_FOREACH(_cellsSelected, it)
		{
			CCTableViewCell *cell = dynamic_cast<CCTableViewCell*>(it);
			CCSprite *led = (CCSprite*) cell->getChildByTag(5);
			led->setColor( ccc3(red, green, blue) );
		}
	}
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void GameScene::menuSendCallback(CCObject* pSender)
{
	CCArray* colores = new CCArray;;
	CCObject *it = NULL;

	int aux_nleds = GameManager::sharedGameManager()->getNLeds();

	for (int i = 0; i < aux_nleds; i++)
	{
		CCTableViewCell *cell = tableView->cellAtIndex(i);
		CCSprite *led = (CCSprite*) cell->getChildByTag(5);
		ccColor3B rgb = led->getColor();
		CCArray* uno_colores = new CCArray;
		int intRgb = rgb.r + rgb.g*256 + rgb.b*65536;
		// si solo mandamos un array con los colores en int nos ahorramos los arrays individuales
		// colores->addObject(CCString::createWithFormat("%d", intRgb));
		uno_colores->addObject(CCString::createWithFormat("%d", rgb.r));
		uno_colores->addObject(CCString::createWithFormat("%d", rgb.g));
		uno_colores->addObject(CCString::createWithFormat("%d", rgb.b));
		colores->addObject(uno_colores);
	}

	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::createWithFormat("%d", aux_nleds), "nLeds");
	prms->setObject(colores, "colores");

	SendMessageWithParams(string("SendColors"), prms);
}

CCTableViewCell* GameScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = CCString::createWithFormat("%d", idx+1);
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();

        CCLOG("1. CELDA CON INDICE %u TIENE PUNTERO EN %p", idx, cell);

        CCSprite *sprite = CCSprite::create( "LED3.png", CCRectMake(0,0,120,120) );
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(10, 0));
        sprite->setTag(5);
        cell->addChild(sprite);

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 25, CCSizeMake(50, 30), kCCTextAlignmentCenter);
        label->setPosition(ccp(45, 130));
		label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        label->setColor(ccc3(0,0,255));
        label->setHorizontalAlignment(kCCTextAlignmentCenter);
        cell->addChild(label);

        CCLOG("2. CELDA CON INDICE %u TIENE PUNTERO EN %p", idx, cell);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }


    return cell;
}

void GameScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
    CCSprite *sprite = (CCSprite*) cell->getChildByTag(5);
    CCLabelTTF *label = (CCLabelTTF*) cell->getChildByTag(123);

    if (_cellsSelected->containsObject(cell))
    {
    	// si ya esta seleccionado
    	_cellsSelected->removeObject(cell);
    	label->setColor(ccc3(0,0,255));

    } else
    {
    	// si no esta seleccionado
    	_cellsSelected->addObject(cell);
    	label->setColor(ccc3(0,255,0));
    }
}

CCSize GameScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(140, 160);
}

unsigned int GameScene::numberOfCellsInTableView(CCTableView *table)
{
    return GameManager::sharedGameManager()->getNLeds();
}
