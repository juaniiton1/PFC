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

OptionsScene::OptionsScene()
{
	label_connected = NULL;
	label_control = NULL;
	input_ip = NULL;
	input_port = NULL;
	input_user = NULL;
	con_smile = NULL;
	color_preview = NULL;
	// Inicializamos el vector de colores
	colors.push_back(Color(0, 0, 255));
	colors.push_back(Color(0, 255, 0));
	colors.push_back(Color(0, 255, 255));
	colors.push_back(Color(255, 0, 0));
	colors.push_back(Color(255, 0, 255));
	colors.push_back(Color(255, 255, 0));
	colors.push_back(Color(255, 255, 255));
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

	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Fondo con la textura de madera
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
				 texture->setTexParameters(&tp);
	CCSprite* background = CCSprite::createWithTexture(texture, CCRectMake(0, 0, vs.width, vs.height));
			  background->setPosition( ccp( vs.width/2, vs.height/2 ) );

	this->addChild(background, 1);

	con_smile = CCSprite::create("con_wait.png", CCRectMake(0,0,64,64) );
	con_smile->setPosition( ccp(vs.width - 64, vs.height - 64) );

	CCSize editBoxSize = CCSizeMake(200, 50);

	input_ip = CCEditBox::create(editBoxSize, CCScale9Sprite::create("edittext2.9.png"));
	input_ip->setPosition(ccp(vs.width/2 - 125, origin.y + vs.height/2 + 12.5 + 25));
	input_ip->setFontName("Helvetica");
	input_ip->setFontSize(25);
	input_ip->setFontColor(ccc3(50,50,50));
	input_ip->setPlaceHolder("IP:");
	input_ip->setText("192.168.1.40");
	input_ip->setPlaceholderFontColor(ccc3(150,150,150));
	input_ip->setInputMode(kEditBoxInputModeSingleLine);
	input_ip->setInputFlag(kEditBoxInputFlagSensitive);

	input_port = CCEditBox::create(editBoxSize, CCScale9Sprite::create("edittext2.9.png"));
	input_port->setPosition(ccp(vs.width/2 - 125, vs.height/2 - 12.5 - 25));
	input_port->setFontName("Helvetica");
	input_port->setFontSize(25);
	input_port->setFontColor(ccc3(50,50,50));
	input_port->setPlaceHolder("Puerto:");
	input_port->setText("2000");
	input_port->setPlaceholderFontColor(ccc3(150,150,150));
	input_port->setInputMode(kEditBoxInputModeNumeric);
	input_port->setInputFlag(kEditBoxInputFlagSensitive);

	std::string user = CCUserDefault::sharedUserDefault()->getStringForKey("user");

	input_user = CCEditBox::create(editBoxSize, CCScale9Sprite::create("edittext3.9.png"));
	input_user->setPosition(ccp(vs.width/2 + 125, vs.height/2 + 50 + 25));
	input_user->setFontName("Helvetica");
	input_user->setFontSize(25);
	input_user->setFontColor(ccc3(50,50,50));
	input_user->setPlaceHolder("Nombre:");
	input_user->setText(user.c_str());
	input_user->setPlaceholderFontColor(ccc3(150,150,150));
	input_user->setInputMode(kEditBoxInputModeSingleLine);
	input_user->setInputFlag(kEditBoxInputFlagSensitive);

	CCMenuItem* color_next = CCMenuItemImage::create("color_next.png", "color_next_h.png", this, menu_selector(OptionsScene::colorNextCallback));
				color_next->setPosition(ccp(vs.width/2 + 125 + 75, vs.height/2));

	CCMenuItem* color_prev = CCMenuItemImage::create("color_prev.png", "color_prev_h.png", this, menu_selector(OptionsScene::colorPrevCallback));
				color_prev->setPosition(ccp(vs.width/2 + 125 - 75, vs.height/2));

	int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
	Color color = colors[iColor];
	color_preview = CCSprite::create("color_preview.png");
	color_preview->setPosition( ccp(vs.width/2 + 125, vs.height/2) );
	color_preview->setColor(ccc3(color.r, color.g, color.b));

	CCMenuItem* control_next = CCMenuItemImage::create("color_next.png", "color_next_h.png", this, menu_selector(OptionsScene::controlNextCallback));
				control_next->setPosition(ccp(vs.width/2 + 125 + 75, vs.height/2 - 50 - 25));

	CCMenuItem* control_prev = CCMenuItemImage::create("color_prev.png", "color_prev_h.png", this, menu_selector(OptionsScene::controlPrevCallback));
				control_prev->setPosition(ccp(vs.width/2 + 125 - 75, vs.height/2 - 50 - 25));

	int iControl = CCUserDefault::sharedUserDefault()->getIntegerForKey("control");
	CCString* control = CCString::create( (iControl == 0) ? "Accel." : "Touch" );
	label_control = CCLabelTTF::create(control->getCString(), "fonts/FrancoisOne.ttf", 30, CCSizeMake(200, 50), kCCTextAlignmentCenter);
	label_control->setColor(ccc3(0,0,0));
	label_control->setPosition(ccp(vs.width/2 + 125, vs.height/2 - 50 - 25));

	CCLabelTTF* label_player = CCLabelTTF::create("Jugador", "fonts/FrancoisOne.ttf", 30, CCSizeMake(200, 50), kCCTextAlignmentCenter);
				label_player->setColor(ccc3(0,0,0));
				label_player->setPosition(ccp(vs.width/2 + 125, vs.height/2 + 50 + 25 + 50 + 25));

	label_connected = CCLabelTTF::create("No conectado", "fonts/FrancoisOne.ttf", 30, CCSizeMake(200, 50), kCCTextAlignmentCenter);
	label_connected->setColor(ccc3(255,0,0));
	label_connected->setPosition(ccp(vs.width/2 - 125, vs.height/2 + 12.5 + 50 + 25 + 25));

	bool connected = GameManager::sharedGameManager()->getConnected();

	// Comprobamos el estado de la conexion y segun ese informamos
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

	CCMenuItem* button_connect = CCMenuItemImage::create("btn_conectar.png", "btn_conectar_h.png", this, menu_selector(OptionsScene::menuConnectCallback));
				button_connect->setPosition(ccp(vs.width/2 - 125, vs.height/2 - 12.5 - 50 - 25 - 25));

	CCMenuItem* button_back = CCMenuItemImage::create("btn_aceptar.png", "btn_aceptar_h.png", this, menu_selector(OptionsScene::menuBackCallback));
				button_back->setPosition(ccp(vs.width/2 + 125, vs.height/2 - 50 - 25 - 50 - 25));

	CCMenu* menu = CCMenu::create(button_connect, button_back, color_next, color_prev, control_next, control_prev, NULL);
			menu->setPosition(ccp(0,0));

	this->addChild(con_smile, 5);
	this->addChild(color_preview, 5);
	this->addChild(label_connected, 5);
	this->addChild(label_player, 5);
	this->addChild(label_control, 5);
	this->addChild(input_ip, 5);
	this->addChild(input_port, 5);
	this->addChild(input_user, 5);
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
	// Guardamos el valor del nombre de jugador
	CCUserDefault::sharedUserDefault()->setStringForKey("user", input_user->getText());

	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void OptionsScene::colorNextCallback(CCObject* pSender)
{
	int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
		iColor = (iColor + 1) % 7;
	Color color = colors[iColor];
	color_preview->setColor(ccc3(color.r, color.g, color.b));

	CCUserDefault::sharedUserDefault()->setIntegerForKey("color", iColor);
}

void OptionsScene::colorPrevCallback(CCObject* pSender)
{
	int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
		iColor = (iColor - 1 + 7) % 7;
	Color color = colors[iColor];
	color_preview->setColor(ccc3(color.r, color.g, color.b));

	CCUserDefault::sharedUserDefault()->setIntegerForKey("color", iColor);
}

void OptionsScene::controlNextCallback(CCObject* pSender)
{
	int iControl = CCUserDefault::sharedUserDefault()->getIntegerForKey("control");
		iControl = (iControl + 1) % 2;
	CCString* control = CCString::create( (iControl == 0) ? "Accel." : "Touch" );
	label_control->setString(control->getCString());

	CCUserDefault::sharedUserDefault()->setIntegerForKey("control", iControl);
}

void OptionsScene::controlPrevCallback(CCObject* pSender)
{
	int iControl = CCUserDefault::sharedUserDefault()->getIntegerForKey("control");
		iControl = (iControl - 1 + 2) % 2;
	CCString* control = CCString::create( (iControl == 0) ? "Accel." : "Touch" );
	label_control->setString(control->getCString());

	CCUserDefault::sharedUserDefault()->setIntegerForKey("control", iControl);
}
