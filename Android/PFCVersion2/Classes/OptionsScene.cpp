/*
 * OptionsScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "OptionsScene.h"
#include "HelloWorldScene.h"
#include "GameManager.h"
#include "NDKHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* OptionsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    OptionsScene *layer = OptionsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

OptionsScene::OptionsScene():label_connected(NULL), label_nleds(NULL), input_ip(NULL), input_port(NULL), con_smile(NULL)
{
}

OptionsScene::~OptionsScene()
{
	NDKHelper::RemoveSelectorsInGroup("OptionsSelectors");
}

// on "init" you need to initialize your instance
bool OptionsScene::init()
{
	if ( !CCLayerColor::initWithColor( ccc4(100, 100, 100, 255) ) )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Fondo con la textura de madera
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	texture->setTexParameters(&tp);
	CCSprite *background = CCSprite::createWithTexture(texture,
			CCRectMake(0, 0, visibleSize.width, visibleSize.height));
	background->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 ) );

	CCLOG("vS.width: %d ; vS.height: %d", visibleSize.width, visibleSize.height);
	this->addChild(background, 1);

	con_smile = CCSprite::create("con_wait.png", CCRectMake(0,0,64,64) );
	con_smile->setPosition( ccp(visibleSize.width - 64, visibleSize.height - 64) );

	CCSize editBoxSize = CCSizeMake(250, 60);

	input_ip = CCEditBox::create(editBoxSize, CCScale9Sprite::create("edittext.9.png"));
	input_ip->setPosition(ccp(origin.x + visibleSize.width/3, origin.y + visibleSize.height/2 + 40));
	input_ip->setFontName("Helvetica");
	input_ip->setFontSize(25);
	input_ip->setFontColor(ccc3(50,50,50));
	input_ip->setPlaceHolder("IP:");
	input_ip->setText("192.168.1.40");
	input_ip->setPlaceholderFontColor(ccc3(150,150,150));
	input_ip->setInputMode(kEditBoxInputModeSingleLine);
	input_ip->setInputFlag(kEditBoxInputFlagSensitive);


	input_port = CCEditBox::create(editBoxSize, CCScale9Sprite::create("edittext.9.png"));
	input_port->setPosition(ccp(origin.x + visibleSize.width/3, origin.y + visibleSize.height/2 - 40));
	input_port->setFontName("Helvetica");
	input_port->setFontSize(25);
	input_port->setFontColor(ccc3(50,50,50));
	input_port->setPlaceHolder("Puerto:");
	input_port->setText("2000");
	input_port->setPlaceholderFontColor(ccc3(150,150,150));
	input_port->setInputMode(kEditBoxInputModeNumeric);
	input_port->setInputFlag(kEditBoxInputFlagSensitive);

	label_connected = CCLabelTTF::create("No conectado", "Helvetica", 30, CCSizeMake(245, 40),
																				kCCTextAlignmentCenter);
	label_connected->setColor(ccc3(255,0,0));
	label_connected->setPosition(ccp(visibleSize.width/3, (visibleSize.height/5) * 4));

	int aux_nleds = GameManager::sharedGameManager()->getNLeds();
	label_nleds = CCLabelTTF::create("4", "Helvetica", 50, CCSizeMake(100, 60), kCCTextAlignmentCenter);
	label_nleds->setColor(ccc3(0,0,0));
	label_nleds->setPosition(ccp((visibleSize.width/3)*2, visibleSize.height/2));
	label_nleds->setString(CCString::createWithFormat("%d", aux_nleds)->getCString());

	// Comprobamos el estado de la conexion y segun ese informamos
	bool connected = GameManager::sharedGameManager()->getConnected();
	if (connected) {
		label_connected->setString("Conectado!");
		label_connected->setColor(ccc3(0,255,0));

		con_smile->setTexture(CCTextureCache::sharedTextureCache()->addImage("con_ok.png"));

		const char* aux_ip   = GameManager::sharedGameManager()->getIp();
		const char* aux_port = GameManager::sharedGameManager()->getPort();
		CCLOG("VAMOS A VER QUE TIENEN IP:%s Y PUERTO:%s", aux_ip, aux_port);
		input_ip->setText(aux_ip);
		input_port->setText(aux_port);
	}

	CCMenuItem *button_connect = CCMenuItemImage::create("btn_conectar.png", "btn_conectar_h.png", this, menu_selector(OptionsScene::menuConnectCallback));
	button_connect->setPosition(ccp(visibleSize.width/3, visibleSize.height/5));

	CCMenuItem *button_back = CCMenuItemImage::create("btn_aceptar.png", "btn_aceptar_h.png", this, menu_selector(OptionsScene::menuBackCallback));
	button_back->setPosition(ccp((visibleSize.width/3)*2, visibleSize.height/5));

	CCMenuItem *button_more_leds = CCMenuItemImage::create("one_more.png", "one_more_h.png", this, menu_selector(OptionsScene::oneMoreLed));
	button_more_leds->setPosition(ccp((visibleSize.width/3) * 2 + 75, visibleSize.height/2 ));

	CCMenuItem *button_less_leds = CCMenuItemImage::create("one_less.png", "one_less_h.png", this, menu_selector(OptionsScene::oneLessLed));
	button_less_leds->setPosition(ccp((visibleSize.width/3) * 2 - 75, visibleSize.height/2 ));

	CCMenu* menu = CCMenu::create(button_connect, button_back, button_more_leds, button_less_leds, NULL);
	menu->setPosition(ccp(0,0));

	this->addChild(con_smile, 5);
	this->addChild(label_connected, 5);
	this->addChild(label_nleds, 5);
	this->addChild(input_ip, 5);
	this->addChild(input_port, 5);
	this->addChild(menu, 5);

	NDKHelper::AddSelector("OptionsSelectors", "connectionOk",
							callfuncND_selector(OptionsScene::connectionOk),
							this);

	NDKHelper::AddSelector("OptionsSelectors", "connectionError",
							callfuncND_selector(OptionsScene::connectionError),
							this);

    this->setTouchEnabled(true);

    return true;
}

void OptionsScene::connectionOk(CCNode *sender, void *data)
{
	label_connected->setColor(ccc3(0,255,0));
	label_connected->setString("Conectado!");

	con_smile->setTexture(CCTextureCache::sharedTextureCache()->addImage("con_ok.png"));

	GameManager::sharedGameManager()->setConnected(true);
	GameManager::sharedGameManager()->setIp(input_ip->getText());
	GameManager::sharedGameManager()->setPort(input_port->getText());

	CCLOG("TERMINO DE ACTUALIZAR GAMEMANAGER. IP:%s PUERTO:%s", input_ip->getText(), input_port->getText());
}

void OptionsScene::connectionError(CCNode *sender, void *data)
{
	label_connected->setColor(ccc3(255,0,0));
	label_connected->setString("Error de conexion");

	con_smile->setTexture(CCTextureCache::sharedTextureCache()->addImage("con_err.png"));

	GameManager::sharedGameManager()->setConnected(false);
}

void OptionsScene::menuConnectCallback(CCObject* pSender)
{
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(input_ip->getText()), "ip");
	prms->setObject(CCString::create(input_port->getText()), "port");
	SendMessageWithParams(string("SelectorConnect"), prms);
}

void OptionsScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void OptionsScene::oneMoreLed(CCObject* pSender)
{
	int aux_nleds = GameManager::sharedGameManager()->getNLeds();
	GameManager::sharedGameManager()->setNLeds(aux_nleds+1);
	label_nleds->setString(CCString::createWithFormat("%d", aux_nleds+1)->getCString());
}

void OptionsScene::oneLessLed(CCObject* pSender)
{
	int aux_nleds = GameManager::sharedGameManager()->getNLeds();
	if (aux_nleds > 2) {
		GameManager::sharedGameManager()->setNLeds(aux_nleds-1);
		label_nleds->setString(CCString::createWithFormat("%d", aux_nleds-1)->getCString());
	}
}
