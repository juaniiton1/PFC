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

OptionsScene::OptionsScene():label_connected(NULL), input_ip(NULL), input_port(NULL)
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

	bool connected = GameManager::sharedGameManager()->getConnected();

	if (connected) {
		label_connected = CCLabelTTF::create("Conectado!", "Helvetica", 30,
											 CCSizeMake(245, 40), kCCTextAlignmentCenter);
		label_connected->setColor(ccc3(0,255,0));
		input_ip->setText(GameManager::sharedGameManager()->getIp());
		input_port->setText(GameManager::sharedGameManager()->getPort());

	} else {
		label_connected = CCLabelTTF::create("No conectado", "Helvetica", 30,
											 CCSizeMake(245, 40), kCCTextAlignmentCenter);
		label_connected->setColor(ccc3(255,0,0));
	}

	label_connected->setPosition(ccp(origin.x + visibleSize.width/2,
										 origin.y + (visibleSize.height/5) * 4));


	CCSize editBoxSize = CCSizeMake(300, 50);

	input_ip = CCEditBox::create(editBoxSize, CCScale9Sprite::create("green_edit.png"));
	input_ip->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + (visibleSize.height/5) * 3));
	input_ip->setFontName("Helvetica");
	input_ip->setFontSize(30);
	input_ip->setFontColor(ccc3(255,255,255));
	input_ip->setPlaceHolder("IP:");
	input_ip->setPlaceholderFontColor(ccc3(255,255,255));
	input_ip->setInputMode(kEditBoxInputModeSingleLine);
	input_ip->setInputFlag(kEditBoxInputFlagSensitive);


	input_port = CCEditBox::create(editBoxSize, CCScale9Sprite::create("green_edit.png"));
	input_port->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + (visibleSize.height/5) * 2));
	input_port->setFontName("Helvetica");
	input_port->setFontSize(30);
	input_port->setFontColor(ccc3(255,255,255));
	input_port->setPlaceHolder("PORT:");
	input_port->setPlaceholderFontColor(ccc3(255,255,255));
	input_port->setInputMode(kEditBoxInputModeNumeric);
	input_port->setInputFlag(kEditBoxInputFlagSensitive);

	CCMenuItemFont *button_connect = CCMenuItemFont::create("Conectar", this,
															menu_selector(OptionsScene::menuConnectCallback));
	button_connect->setFontSize(40);
	button_connect->setFontName("Helvetica");

	CCMenuItemFont *button_back = CCMenuItemFont::create("Volver al menu", this,
														 menu_selector(OptionsScene::menuBackCallback));

	CCMenu* menu = CCMenu::create(button_connect, button_back, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + (visibleSize.height/5) * 1));

	this->addChild(label_connected, 1);
	this->addChild(input_ip, 1);
	this->addChild(input_port, 1);
	this->addChild(menu, 1);

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

	GameManager::sharedGameManager()->setConnected(true);
	GameManager::sharedGameManager()->setIp(input_ip->getText());
	GameManager::sharedGameManager()->setPort(input_port->getText());
}

void OptionsScene::connectionError(CCNode *sender, void *data)
{
	label_connected->setColor(ccc3(255,0,0));
	label_connected->setString("Error de conexion");

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
