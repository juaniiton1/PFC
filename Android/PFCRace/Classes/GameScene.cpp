/*
 * MainMenuScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "GameScene.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "Curva.h"
#include "NDKHelper.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

#define FIX_POS(_pos, _min, _max) 	\
			if (_pos < _min)        \
				_pos = _min;        \
			else if (_pos > _max)   \
				_pos = _max;        \

#define VEL_MIN 0	// Velocidad minima del coche
#define VEL_MAX 200	// Velocidad maxima del coche
#define VEL_INC 0.5	// Incremento de velocidad normal
#define VEL_BOO 2.0 // Incremento de velocidad con BOOST / NITRO
#define VEL_FRE 5.0 // Decremento de velocidad con Frenazo
#define DIS_CAR 100	// Distancia entre coche y coche en direccion contraria
#define VEL_OPP 50	// Rango de velocidad de los coches en contra
#define DIS_ROA 50	// Distancia de la pista visible

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

GameScene::GameScene()
{
	_layerPause = NULL;
	_layerEnd = NULL;
	_car = NULL;
	_road = NULL;
	_road2 = NULL;
	_grass = NULL;
	_labelVel = NULL;
	_labelDist = NULL;
	_labelTime = NULL;
	_labelTimeEnd = NULL;
	_labelStart = NULL;
	_oppCars = NULL;
	_piano1 = NULL;
	_piano2 = NULL;
	_curvasI = NULL;
	_curvasD = NULL;
	_vel = 0.0;
	_dist = 0.0;
	_started = false;
	_paused = false;
	_nextCurva = 0;
	_fuerzaCurva = 0;
	_time = 0;
	_mCircuito = 0;
	_level = GameManager::sharedGameManager()->getLevel();
	_control = CCUserDefault::sharedUserDefault()->getIntegerForKey("control");
	_frenando = false;
	_acelerando = false;
	_tramo = 0;
	_longTramo = 0;
}

GameScene::~GameScene()
{
	if (_oppCars) {
		_oppCars->release();
		_oppCars = NULL;
	}
	if (_layerPause) {
		_layerPause->release();
		_layerPause = NULL;
	}
	if (_layerEnd) {
		_layerEnd->release();
		_layerEnd = NULL;
	}
	if (_curvasI) {
		_curvasI->release();
		_curvasI = NULL;
	}
	if (_curvasD) {
		_curvasD->release();
		_curvasD = NULL;
	}
	if (_labelStart) {
		_labelStart->release();
		_labelStart = NULL;
	}
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	// Variables de tamanyo
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Layer con los botones al hacer pause
	_layerPause = CCLayerColor::create(ccc4(255,255,255,0));
	_layerPause->setContentSize(CCSizeMake(350, 100));
	_layerPause->ignoreAnchorPointForPosition(false);
	_layerPause->setAnchorPoint(ccp(0.5, 0.5));
	_layerPause->setPosition(ccp(vs.width/2, vs.height/2));
	_layerPause->retain();

	CCMenuItem* button_resume = CCMenuItemImage::create("btn_resume.png", "btn_resume_h.png", this, menu_selector(GameScene::menuResumeCallback));
				button_resume->setPosition(ccp(50, 50));
	CCMenuItem* button_restart = CCMenuItemImage::create("btn_restart.png", "btn_restart_h.png", this, menu_selector(GameScene::menuRestartCallback));
				button_restart->setPosition(ccp(175, 50));
	CCMenuItem* button_exit = CCMenuItemImage::create("btn_exit.png", "btn_exit_h.png", this, menu_selector(GameScene::menuExitCallback));
				button_exit->setPosition(ccp(300, 50));

	CCMenu* menu_pause = CCMenu::create(button_resume, button_restart, button_exit, NULL);
			menu_pause->setPosition(ccp(0,0));

	_layerPause->addChild(menu_pause);

	// Layer que se muestra al terminar la carrera
	_layerEnd = CCLayerColor::create(ccc4(0,0,0,0));
	_layerEnd->setContentSize(CCSizeMake(225, 225));
	_layerEnd->ignoreAnchorPointForPosition(false);
	_layerEnd->setAnchorPoint(ccp(0.5, 0.5));
	_layerEnd->setPosition(ccp(vs.width/2, vs.height/2));
	_layerEnd->retain();

	CCMenuItem* button_end_restart = CCMenuItemImage::create("btn_restart.png", "btn_restart_h.png", this, menu_selector(GameScene::menuRestartCallback));
				button_end_restart->setPosition(ccp(50, 50));
	CCMenuItem* button_end_exit = CCMenuItemImage::create("btn_exit.png", "btn_exit_h.png", this, menu_selector(GameScene::menuExitCallback));
				button_end_exit->setPosition(ccp(175, 50));

	CCMenu* menu_end = CCMenu::create(button_end_restart, button_end_exit, NULL);
			menu_end->setPosition(ccp(0,0));

	CCLayerColor* layerTime = CCLayerColor::create(ccc4(255,255,255,255));
				  layerTime->setContentSize(CCSizeMake(225, 100));
				  layerTime->ignoreAnchorPointForPosition(false);
				  layerTime->setAnchorPoint(ccp(0.5, 0.5));
				  layerTime->setPosition(ccp(112.5, 175));
				  layerTime->retain();

	_labelTimeEnd = CCLabelTTF::create("Time:\n0:00:000", "fonts/FrancoisOne.ttf", 30, CCSizeMake(225, 100), kCCTextAlignmentCenter);
	_labelTimeEnd->setColor(ccc3(0,0,0));
	_labelTimeEnd->setPosition(ccp(112.5, 50));
	layerTime->addChild(_labelTimeEnd);

	_layerEnd->addChild(menu_end);
	_layerEnd->addChild(layerTime);


	// Boton de pause (menu)
	CCMenuItem* button_pause = CCMenuItemImage::create("btn_pause.png", "btn_pause_h.png", this, menu_selector(GameScene::menuPauseCallback));
				button_pause->setPosition(ccp(75, vs.height - 75));

	CCMenu* menu = CCMenu::create(button_pause, NULL);
			menu->setPosition(ccp(0,0));

	// Velocidad
	CCString* stringVel = CCString::createWithFormat("%d km/h", (int)_vel);
	_labelVel = CCLabelTTF::create(stringVel->getCString(), "fonts/FrancoisOne.ttf", 30, CCSizeMake(140, 50), kCCTextAlignmentCenter);
	_labelVel->setColor(ccc3(255,255,255));
	_labelVel->setPosition(ccp(vs.width - 72, 50));

	// Distancia
	CCString* stringDist = CCString::createWithFormat("%d m", (int)_dist);
	_labelDist = CCLabelTTF::create(stringDist->getCString(), "fonts/FrancoisOne.ttf", 30, CCSizeMake(140, 50), kCCTextAlignmentCenter);
	_labelDist->setColor(ccc3(255,255,255));
	_labelDist->setPosition(ccp(72, 50));

	// Tiempo
	int hs = _time/3600000;
	int mins = (_time%3600000)/60000;
	int ss = ((_time%3600000)%60000)/1000;
	int ms = ((_time%3600000)%60000)%1000;
	CCString* stringTime = CCString::createWithFormat("%d:%02d:%03d", mins, ss, ms);
	_labelTime = CCLabelTTF::create(stringTime->getCString(), "fonts/FrancoisOne.ttf", 30, CCSizeMake(140, 50), kCCTextAlignmentCenter);
	_labelTime->setColor(ccc3(255,255,255));
	_labelTime->setPosition(ccp(vs.width - 72, vs.height - 50));

	// Click to start
	_labelStart = CCLabelTTF::create("Click to Start", "fonts/FrancoisOne.ttf", 50, CCSizeMake(200, 200), kCCTextAlignmentCenter);
	_labelStart->setColor(ccc3(255,255,255));
	_labelStart->setPosition(ccp(vs.width/2, vs.height/2));
	_labelStart->retain();

	// Hierba
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("texture_grass2.jpg");
				 texture->setTexParameters(&tp);
	_grass = CCSprite::createWithTexture(texture, CCRectMake(0, 0, vs.width, vs.height + 256));
	_grass->setPosition( ccp( vs.width/2, vs.height/2 + 128 ) );

	// Carretera
	texture = CCTextureCache::sharedTextureCache()->addImage("texture_road2.jpg");
	texture->setTexParameters(&tp);
	_road = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 512, vs.height + 512));
	_road->setPosition( ccp( vs.width/2, vs.height/2 + 256 ) );

	// Pianos
	texture = CCTextureCache::sharedTextureCache()->addImage("piano.png");
	texture->setTexParameters(&tp);
	_piano1 = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 64, vs.height + 64));
	_piano1->setPosition( ccp(vs.width * 0.15, vs.height / 2 + 32) );
	_piano2 = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 64, vs.height + 64));
	_piano2->setPosition( ccp(vs.width * 0.85, vs.height / 2 + 32) );

	// Coche
	_car = CCSprite::create("car_above_blue.png");
	_car->setPosition(ccp(vs.width/2, 50));

	// Creamos los indicadores de curva
	_curvasI = new CCArray;
	_curvasD = new CCArray;

	for (int i = 1; i <= 5; i++) {
		CCString* str_curvaI = CCString::createWithFormat("curva_izq_%d.png", i);
		CCString* str_curvaD = CCString::createWithFormat("curva_der_%d.png", i);
		CCSprite* curvaI = CCSprite::create(str_curvaI->getCString());
				  curvaI->setOpacity(0);
				  curvaI->setPosition(ccp(vs.width/2, vs.height/4*3));
		CCSprite* curvaD = CCSprite::create(str_curvaD->getCString());
				  curvaD->setOpacity(0);
				  curvaD->setPosition(ccp(vs.width/2, vs.height/4*3));
		_curvasI->addObject(curvaI);
		_curvasD->addObject(curvaD);
		this->addChild(curvaI, 5);
		this->addChild(curvaD, 5);
	}

	// Anyadimos los elementos a la vista
	this->addChild(_grass, 1);
	this->addChild(_road, 2);
	this->addChild(_car, 3);
	this->addChild(menu, 5);
	this->addChild(_labelVel, 5);
	this->addChild(_labelDist, 5);
	this->addChild(_labelTime, 5);
	this->addChild(_labelStart, 5);


	// Inicializamos el vector de curvas segun el nivel
	if (_level == 1) {
		_curvas.push_back(Curva(250, 200, 2, false));
		_curvas.push_back(Curva(550, 200, 2, false));
		_curvas.push_back(Curva(1200, 100, 4, false));
		_curvas.push_back(Curva(1650, 200, 2, false));
		_curvas.push_back(Curva(1950, 200, 2, false));
		_curvas.push_back(Curva(2500, 200, 2, false));
		_curvas.push_back(Curva(2800, 200, 2, false));
		_mCircuito = 3000;

	} else if (_level == 2) {
		_curvas.push_back(Curva(100, 150, 2, false));
		_curvas.push_back(Curva(350, 150, 2, false));
		_curvas.push_back(Curva(700, 250, 1, false));
		_curvas.push_back(Curva(1150, 150, 3, false));
		_curvas.push_back(Curva(1500, 100, 4, false));
		_curvas.push_back(Curva(1800, 100, 4, false));
		_curvas.push_back(Curva(2000, 200, 2, true));
		_curvas.push_back(Curva(2300, 50, 5, true));
		_curvas.push_back(Curva(2750, 150, 2, true));
		_curvas.push_back(Curva(3000, 50, 4, true));
		_curvas.push_back(Curva(3100, 100, 2, false));
		_curvas.push_back(Curva(3300, 100, 2, false));
		_curvas.push_back(Curva(3500, 100, 4, false));
		_curvas.push_back(Curva(3800, 250, 2, false));
		_curvas.push_back(Curva(4250, 250, 1, false));
		_curvas.push_back(Curva(4700, 150, 3, false));
		_mCircuito = 5350;

	} else { // _level == 3
		_curvas.push_back(Curva(100, 50, 2, true));
		_curvas.push_back(Curva(250, 50, 2, false));
		_curvas.push_back(Curva(400, 100, 4, false));
		_curvas.push_back(Curva(600, 100, 2, false));
		_curvas.push_back(Curva(800, 50, 4, true));
		_curvas.push_back(Curva(950, 50, 4, true));
		_curvas.push_back(Curva(1100, 100, 2, false));
		_curvas.push_back(Curva(1300, 50, 4, true));
		_curvas.push_back(Curva(1450, 50, 4, true));
		_curvas.push_back(Curva(1600, 100, 4, false));
		_curvas.push_back(Curva(1900, 100, 2, false));
		_curvas.push_back(Curva(2100, 50, 5, true));
		_curvas.push_back(Curva(2450, 100, 2, true));
		_curvas.push_back(Curva(2650, 50, 5, false));
		_curvas.push_back(Curva(2900, 100, 4, false));
		_curvas.push_back(Curva(3200, 150, 2, true));
		_curvas.push_back(Curva(3450, 100, 4, true));
		_curvas.push_back(Curva(3650, 50, 4, true));
		_curvas.push_back(Curva(3800, 100, 4, false));
		_curvas.push_back(Curva(4000, 50, 4, true));
		_curvas.push_back(Curva(4150, 150, 2, true));
		_curvas.push_back(Curva(4400, 100, 2, false));
		_curvas.push_back(Curva(4600, 100, 2, false));
		_curvas.push_back(Curva(4800, 100, 5, true));
		_curvas.push_back(Curva(5000, 50, 2, true));
		_curvas.push_back(Curva(5150, 100, 5, false));
		_curvas.push_back(Curva(5450, 200, 5, true));
		_mCircuito = 5850;
	}

	// Inicializamos el vector de coches en contra
	_oppCars = new CCArray;

	// Activamos el acelerometro y el touch
    this->setTouchEnabled(true);

    // Calculamos el tamanyo de cada tramo para informar al Arduino
    int nleds = CCUserDefault::sharedUserDefault()->getIntegerForKey("nleds");
    _longTramo = (int) (_mCircuito/nleds);

    // Enviamos al Arduino que hemos entrado
    int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
	CCDictionary* prms = CCDictionary::create();
				  prms->setObject(CCString::createWithFormat("%d", iColor), "color");
	SendMessageWithParams(string("JugadorEntra"), prms);

    return true;
}

void GameScene::updateFrame(float dt)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	//long cmDist 	= _dist*100;
	//int offsetDist 	= cmDist % 667;
	//int offsetY		= offsetDist / 10;

	//_piano1->setPosition( ccp(vs.width * 0.15, vs.height / 2 + 32 - offsetY) );
	//_piano2->setPosition( ccp(vs.width * 0.85, vs.height / 2 + 32 - offsetY) );

	// Movemos el cesped y la carretera
	float mRec	= (_vel/3.6) * dt;
	float pxRec	= mRec / 0.039;

	CCPoint posRoad  = _road->getPosition();
	CCPoint posGrass = _grass->getPosition();

	if (posRoad.y - pxRec < vs.height/2 - 256) {
		_road->setPosition( ccp(posRoad.x, posRoad.y + 512 - pxRec) );
	} else {
		_road->setPosition( ccp(posRoad.x, posRoad.y - pxRec) );
	}

	if (posGrass.y - pxRec < vs.height/2 - 128) {
		_grass->setPosition( ccp(posGrass.x, posGrass.y + 256 - pxRec) );
	} else {
		_grass->setPosition( ccp(posGrass.x, posGrass.y - pxRec) );
	}

	// Tamanyos del coche y la carretera
	CCRect roadRec 	= _road->boundingBox();
	CCSize carSize 	= _car->getContentSize();


	// Detectamos colision entre coche y coche contrario
	//   - eliminamos al contrario
	//	 - centramos a velocidad 0 al coche principal
	CCRect carRect = CCRectMake(
			_car->getPosition().x - (_car->getContentSize().width/2),
			_car->getPosition().y - (_car->getContentSize().height/2),
			_car->getContentSize().width,
			_car->getContentSize().height);

	CCObject *it = NULL;
	CCARRAY_FOREACH(_oppCars, it)
	{
		CCSprite *oppCar = dynamic_cast<CCSprite*>(it);
		CCRect oppCarRect = CCRectMake(
				oppCar->getPosition().x - (oppCar->getContentSize().width/2),
				oppCar->getPosition().y - (oppCar->getContentSize().height/2),
				oppCar->getContentSize().width,
				oppCar->getContentSize().height);

		if (carRect.intersectsRect(oppCarRect))
		{
			// SONIDO DE CHOQUE
			// ****************

			// 1. Paramos los schedules y el acelerometro, y ponemos velocidad a 0
			this->setAccelerometerEnabled(false);
			//this->unscheduleAll();
			_vel = 0;

			// 2. Eliminamos el oponente con su efecto/accion
			CCFiniteTimeAction* oppJump = CCSpawn::create(	CCMoveBy::create(8, ccp(0,500)),
															//CCRotateBy::create(8, 4320),
															NULL);
			CCFiniteTimeAction* oppDel = CCCallFuncN::create( this, callfuncN_selector(GameScene::oppCarDelete));

			// 3. Movemos el _car al centro con su efecto/accion
			CCRepeat* _carVibration = CCRepeat::create(CCSequence::create(
					CCMoveBy::create(0.05f, ccp(5,0)),
					CCMoveBy::create(0.05f, ccp(-5,0)),
					NULL), 40);
			CCActionInterval* _carFadeIn = CCFadeIn::create(2.0f);
			CCActionInterval* _carFadeOut = CCFadeOut::create(2.0f);
			CCActionInstant* _carPlace = CCPlace::create(ccp(vs.width/2, 50));

			// 4. Reanudamos carrera
			CCFiniteTimeAction* resumeRace = CCCallFuncN::create( this, callfuncN_selector(GameScene::resumeRace));

			// 5. Ejecutamos todas las acciones
			oppCar->runAction( CCSequence::create(oppJump, _carFadeOut, oppDel, NULL));
			_car->runAction( _carVibration );
			_car->runAction( CCSequence::create(_carFadeOut, _carPlace, _carFadeIn, resumeRace, NULL) );

		}
	}

	// Sumamos el tiempo
	_time += (long)(dt*1000);
	int hs = _time/3600000;
	int mins = (_time%3600000)/60000;
	int ss = ((_time%3600000)%60000)/1000;
	int ms = ((_time%3600000)%60000)%1000;
	CCString* stringTime = CCString::createWithFormat("%d:%02d:%03d", mins, ss, ms);
	_labelTime->setString(stringTime->getCString());
}

void GameScene::oppCarDelete(CCNode* sender)
{
	CCSprite* oppCar = (CCSprite*) sender;

	// Eliminamos el coche que se choca
	_oppCars->removeObject(oppCar);
	this->removeChild(oppCar);
}

void GameScene::resumeRace(CCNode* sender)
{
	// Llamamos cada 0,1 segundos a la logica del juego
	this->schedule( schedule_selector(GameScene::gameLogic), 0.1 );
	this->schedule( schedule_selector(GameScene::updateFrame) );

	// Activamos el acelerometro y el touch
	this->setAccelerometerEnabled(true);
}

void GameScene::showCar(CCNode* sender)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
	CCSize carSize 	= _car->getContentSize();

	CCSprite* warning = (CCSprite*) sender;

	// Velocidad del coche en contra
	int velRandom = rand() % VEL_OPP;

	int posX = warning->getPosition().x;

	// Colocamos y mostramos el coche
	CCSprite* oppCar = CCSprite::create("car_above_orange.png");
					  oppCar->setTag(velRandom);
					  oppCar->setPosition( ccp( posX, vs.height + carSize.height/2 ));
	_oppCars->addObject(oppCar);
	this->addChild(oppCar, 3);

	// Eliminamos el warning temporal
	this->removeChild(warning);
}

void GameScene::gameLogic(float dt)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	// Actualizamos velocidad
	CCString* stringVel = CCString::createWithFormat("%d km/h", (int)_vel);
	_labelVel->setString(stringVel->getCString());

	// Calculamos la distancia recorrida en 0,1s a velocidad _vel
	// 1km/h = 1000m/3600s = 1000m/36000ds (1ds = 0,1s)
	float distAnt = _dist;
	_dist += _vel/36;

	// Actualizamos distancia
	CCString* stringDist = CCString::createWithFormat("%d m", (int)_dist);
	_labelDist->setString(stringDist->getCString());

	// Cada distancia de tramo enviamos al Arduino que avanzamos una posicion
	if ((int) (_dist/_longTramo) > _tramo) {

	    // Enviamos al Arduino que hemos avanzado
	    int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
		CCDictionary* prms = CCDictionary::create();
					  prms->setObject(CCString::createWithFormat("%d", iColor), "color");
		SendMessageWithParams(string("JugadorAvanza"), prms);
		_tramo = (int) (_dist/_longTramo);
	}

	// Tamanyos del coche y la carretera
	CCRect roadRec 	= _road->boundingBox();
	CCSize carSize 	= _car->getContentSize();

	// Cada 100 metros sacamos un coche en contra
	if ( (int) (_dist / DIS_CAR) > (int) (distAnt / DIS_CAR) ) {

		// Calculamos la posicion de salida (random)
		int rangeX 	= roadRec.size.width - carSize.width;
		int minY 	= roadRec.origin.x + carSize.width/2;
		int posX 	= ( rand() % rangeX ) + minY;

		// Creamos el indicador de coche en contra
		CCSprite* warning = CCSprite::create("warning.png");
		this->addChild(warning, 4);

		// Mostramos el warning donde saldra el coche y lo sacamos
		CCActionInstant* _wPlace = CCPlace::create(ccp(posX, vs.height - 50));
		CCActionInterval* _wParpadeo = CCBlink::create(2, 4);
		CCActionInstant* _wReplace = CCPlace::create(ccp(posX, vs.height + 100));
		CCFiniteTimeAction* _showCar = CCCallFuncN::create( this, callfuncN_selector(GameScene::showCar));
		warning->runAction( CCSequence::create(_wPlace, _wParpadeo, _wReplace, _showCar, NULL) );
	}

	// Movemos a los coches en direccion contraria
	CCObject *it = NULL;
	CCARRAY_FOREACH(_oppCars, it)
	{
		CCSprite *auxCar = dynamic_cast<CCSprite*>(it);

		// Cogemos la velocidad del coche en contra y calculamos la distancia visible recorrida
		int auxVel 		= auxCar->getTag();
		float diffVel 	= _vel - auxVel;
		float auxDist 	= diffVel/36;

		CCPoint auxPos = auxCar->getPosition();
		CCPoint newPos = ccp(auxPos.x, auxPos.y - auxDist * roadRec.size.height / DIS_ROA);
		CCFiniteTimeAction* actionMove = CCMoveTo::create( (float) 0.1, newPos );
		auxCar->runAction( CCSequence::create(actionMove, NULL) );

		// Eliminamos al coche contrario si:
		// 	 - se le pasa y queda por debajo de la pantalla
		//	 - se aleja tanto por delante que dobla la carretera actual
		if ( auxPos.y > vs.height + carSize.height || auxPos.y < 0 - carSize.height/2 ) {
			this->removeChild(auxCar);
			_oppCars->removeObject(auxCar);
		}
	}

	if (_nextCurva < _curvas.size())
	{
		int startCurva 	= _curvas[_nextCurva].getDist();
		int endCurva 	= startCurva + _curvas[_nextCurva].getLong();
		int fuerza 		= _curvas[_nextCurva].getFuerza();

		// Comprobamos si viene curva 50 m antes de esta
		if (startCurva - 50 <= _dist && _dist < startCurva) {

			if (_curvas[_nextCurva].getIzq()) {
				((CCSprite*) _curvasI->objectAtIndex(fuerza-1))->setOpacity(200 - (startCurva - _dist) * (200 / 50));

			} else {
				((CCSprite*) _curvasD->objectAtIndex(fuerza-1))->setOpacity(200 - (startCurva - _dist) * (200 / 50));
			}
		}

		// Si estamos en curva lo hacemos parpadear
		else if (distAnt <= startCurva && startCurva <= _dist) {
			CCRepeatForever *parpadeo = CCRepeatForever::create( CCBlink::create(1, 6) );

			if (_curvas[_nextCurva].getIzq()) {
				((CCSprite*) _curvasI->objectAtIndex(fuerza-1))->runAction(parpadeo);
				_fuerzaCurva = 1 * (_vel/VEL_MAX) * fuerza;

			} else {
				((CCSprite*) _curvasD->objectAtIndex(fuerza-1))->runAction(parpadeo);
				_fuerzaCurva = -1 * (_vel/VEL_MAX) * fuerza;
			}
		}

		// Si salimos de la curva quitamos el indicador
		else if (distAnt <= endCurva && endCurva <= _dist) {

			if (_curvas[_nextCurva].getIzq()) {
				((CCSprite*) _curvasI->objectAtIndex(fuerza-1))->stopAllActions();
				((CCSprite*) _curvasI->objectAtIndex(fuerza-1))->setOpacity(0);

			} else {
				((CCSprite*) _curvasD->objectAtIndex(fuerza-1))->stopAllActions();
				((CCSprite*) _curvasD->objectAtIndex(fuerza-1))->setOpacity(0);
			}
			_nextCurva++;
			_fuerzaCurva = 0;
		}
	}

	// Hemos llegado al final del circuito
	if (_dist >= _mCircuito) {

		this->setAccelerometerEnabled(false);
		this->unscheduleAllSelectors();

		CCFiniteTimeAction* actionMove = CCMoveBy::create( (float) 1.0, ccp(0, 600) );
		_car->runAction( CCSequence::create(actionMove, NULL) );

		if (!_paused) {
			_paused = true;

			// Enviamos al Arduino que hemos terminado
			int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
			CCDictionary* prms = CCDictionary::create();
						  prms->setObject(CCString::createWithFormat("%d", iColor), "color");
			SendMessageWithParams(string("JugadorTermina"), prms);

			// Mostramos el tiempo final y el layer de final
			int hs = _time/3600000;
			int mins = (_time%3600000)/60000;
			int ss = ((_time%3600000)%60000)/1000;
			int ms = ((_time%3600000)%60000)%1000;
			CCString* stringTime = CCString::createWithFormat("Time:\n%d:%02d:%03d", mins, ss, ms);
			_labelTimeEnd->setString(stringTime->getCString());

			this->addChild(_layerEnd, 10);

			// Metemos el record en la BBDD
			CCString* keyMap = CCString::createWithFormat("map%d", _level);
		    int nRecords = CCUserDefault::sharedUserDefault()->getIntegerForKey(keyMap->getCString());

			bool trobat = false;
			for (int i = nRecords; i >= 0 && !trobat; i--) {
				CCString* iKeyTime = CCString::createWithFormat("map%dtime%d", _level, i);
				CCString* iKeyUser = CCString::createWithFormat("map%duser%d", _level, i);
				CCString* iKeyTimeM1 = CCString::createWithFormat("map%dtime%d", _level, i+1);
				CCString* iKeyUserM1 = CCString::createWithFormat("map%duser%d", _level, i+1);
				int iTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(iKeyTime->getCString());
				std::string iUser = CCUserDefault::sharedUserDefault()->getStringForKey(iKeyUser->getCString());
				if (_time < iTime) {
					// Actualizamos el siguiente
					CCUserDefault::sharedUserDefault()->setIntegerForKey(iKeyTimeM1->getCString(), iTime);
					CCUserDefault::sharedUserDefault()->setStringForKey(iKeyUserM1->getCString(), iUser);
				} else {
					trobat = true;
					// Actualizamos el actual
					std::string username  = CCUserDefault::sharedUserDefault()->getStringForKey("user");
					CCUserDefault::sharedUserDefault()->setIntegerForKey(iKeyTimeM1->getCString(), _time);
					CCUserDefault::sharedUserDefault()->setStringForKey(iKeyUserM1->getCString(), username);
				}
			}
			if (nRecords < 5) CCUserDefault::sharedUserDefault()->setIntegerForKey(keyMap->getCString(), nRecords+1);
		}
	}

}

void GameScene::didAccelerate(CCAcceleration* pAccelerationValue)
{
    CCDirector* sDir = CCDirector::sharedDirector();

    // Valores de acelerometro
    float accX = pAccelerationValue->x;
    float accY = pAccelerationValue->y;
    float accZ = pAccelerationValue->z;

    // Limites por izquierda y derecha de la carretera
    CCRect roadRec 	= _road->boundingBox();
    CCSize carSize 	= _car->getContentSize();
    float ptMin 	= roadRec.origin.x + carSize.width / 2.0;
    float ptMax 	= roadRec.origin.x + roadRec.size.width - carSize.width / 2.0;

    // Punto actual del coche y siguiente temporal
    CCPoint ptNow  = _car->getPosition();
    CCPoint ptTemp = sDir->convertToUI(ptNow);

    ptTemp.x += accX * 10.0 + _fuerzaCurva;

    // Si el control lo tenemos en Acelerometro
	if (_control == 0) {
		// Calculamos velocidad nueva
		if (accZ > 0) { // (movil boca abajo)

			// Freno de mano -> frenazo gradual segun hasta 0
			_vel = (_vel - VEL_FRE <= VEL_MIN) ? VEL_MIN : _vel - VEL_FRE;

		} else { // accZ <= 0 (movil boca arriba)

			if (accY > 0) { // (incilinado hacia delante | vertical girado)

				// Nitro -> aumentamos velocidad con mas aceleracion hasta maximo (100)
				_vel = (_vel + VEL_BOO > VEL_MAX) ? VEL_MAX : _vel + VEL_BOO;

			} else { // accY <= 0 (vertical hacia mi)

				// Acelerar/desacelerar -> modificamos velocidad gradualmente
				float velAcc = accZ * - VEL_MAX;
				if (velAcc > _vel)
					_vel = (_vel + VEL_INC > velAcc) ? velAcc : _vel + VEL_INC;
				else
					_vel = (_vel - VEL_INC < velAcc) ? velAcc : _vel - VEL_INC;
			}
		}
	}

    // Actualizamos posicion del coche
    // Tambien si toca al limite de la carretera, lo hacemos frenar
    CCPoint ptNext = sDir->convertToGL(ptTemp);

    if (ptNext.x < ptMin) {
    	ptNext.x = ptMin;
    	_vel -= (_vel - VEL_BOO >= 0) ? VEL_BOO : 0;

    } else if (ptNext.x > ptMax) {
    	ptNext.x = ptMax;
    	_vel -= (_vel - VEL_BOO >= 0) ? VEL_BOO : 0;
    }

    _car->setPosition(ptNext);
}

void GameScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (!_started) {
		_started = true;

		// Llamamos cada 0,1 segundos a la logica del juego
		this->schedule( schedule_selector(GameScene::gameLogic), 0.1 );
		this->schedule( schedule_selector(GameScene::updateFrame) );
		this->schedule( schedule_selector(GameScene::soltar), 0.1 );

		// Activamos el acelerometro y el touch
		this->setAccelerometerEnabled(true);

		// Quitamos el "Click to start"
		this->removeChild(_labelStart);

		// Enviamos al Arduino que hemos empezado
		int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
		CCDictionary* prms = CCDictionary::create();
					  prms->setObject(CCString::createWithFormat("%d", iColor), "color");
		SendMessageWithParams(string("EmpiezaCarrera"), prms);
	}

	if (_control == 1 && _started && !_paused) {
		// Comprobamos si hemos clickado en la derecha o izquierda

		if (location.x > vs.width/2) { // acelerar
			this->unschedule( schedule_selector(GameScene::soltar) );
			this->schedule( schedule_selector(GameScene::acelerar), 0.1 );
			_acelerando = true;

		} else { // frenar
			this->unschedule( schedule_selector(GameScene::soltar) );
			this->schedule( schedule_selector(GameScene::frenar), 0.1 );
			_frenando = true;
		}
	}
}

void GameScene::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (_control == 1 && _started && !_paused) {
		// Comprobamos si nos cambiamos de lado con el click

		if (location.x > vs.width/2 && _frenando) { // acelerar
			this->unschedule( schedule_selector(GameScene::frenar) );
			this->schedule( schedule_selector(GameScene::acelerar), 0.1 );
			_frenando = false;
			_acelerando = true;

		} else if (location.x < vs.width/2 && _acelerando) { // frenar
			this->unschedule( schedule_selector(GameScene::acelerar) );
			this->schedule( schedule_selector(GameScene::frenar), 0.1 );
			_acelerando = false;
			_frenando = true;
		}
	}
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (_control == 1 && _started && !_paused) {
		// Comprobamos donde dejamos de hacer click

		if (location.x > vs.width/2) { // acelerar
			this->unschedule( schedule_selector(GameScene::acelerar) );
			this->schedule( schedule_selector(GameScene::soltar), 0.1 );
			_acelerando = false;

		} else if (location.x < vs.width/2) { // frenar
			this->unschedule( schedule_selector(GameScene::frenar) );
			this->schedule( schedule_selector(GameScene::soltar), 0.1 );
			_frenando = false;
		}
	}
}

void GameScene::acelerar(float dt)
{
	_vel = (_vel + VEL_BOO < VEL_MAX) ? _vel + VEL_BOO : VEL_MAX;
}

void GameScene::frenar(float dt)
{
	_vel = (_vel - VEL_FRE > VEL_MIN) ? _vel - VEL_FRE : VEL_MIN;
}

void GameScene::soltar(float dt)
{
	_vel = (_vel - VEL_INC > VEL_MIN) ? _vel - VEL_INC : VEL_MIN;
}

void GameScene::menuPauseCallback(CCObject* pSender)
{
	if (!_paused) {
		this->setAccelerometerEnabled(false);
		this->unscheduleAllSelectors();

		this->addChild(_layerPause, 10);
		_paused = true;
	}
}

void GameScene::menuResumeCallback(CCObject* pSender)
{
	if (_started) {
		// Llamamos cada 0,1 segundos a la logica del juego
		this->schedule( schedule_selector(GameScene::gameLogic), 0.1 );
		this->schedule( schedule_selector(GameScene::updateFrame) );

		// Activamos el acelerometro y el touch
		this->setAccelerometerEnabled(true);
	}
	this->removeChild(_layerPause);
	_paused = false;
}

void GameScene::menuRestartCallback(CCObject* pSender)
{
	CCSize vs = CCDirector::sharedDirector()->getVisibleSize();

	_paused = false;
	_started = false;
	_vel = 0;
	_dist = 0;
	_time = 0;
	_nextCurva = 0;
	_fuerzaCurva = 0;
	_frenando = false;
	_acelerando = false;

	this->removeChild(_layerPause);
	this->removeChild(_layerEnd);
	this->addChild(_labelStart, 5);

	CCObject *it = NULL;
	CCARRAY_FOREACH(_curvasI, it)
	{
		CCSprite* auxCurva = dynamic_cast<CCSprite*>(it);
				  auxCurva->stopAllActions();
				  auxCurva->setOpacity(0);
	}
	CCARRAY_FOREACH(_curvasD, it)
	{
		CCSprite* auxCurva = dynamic_cast<CCSprite*>(it);
				  auxCurva->stopAllActions();
				  auxCurva->setOpacity(0);
	}

	_car->setPosition(ccp(vs.width/2, 50));
	_labelVel->setString("0 km/h");
	_labelDist->setString("0 m");
	_labelTime->setString("0:00:000");

	CCARRAY_FOREACH(_oppCars, it)
	{
		CCSprite *oppCar = dynamic_cast<CCSprite*>(it);
		this->removeChild(oppCar);
		_oppCars->removeObject(oppCar);
	}
}

void GameScene::menuExitCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());

    // Enviamos al Arduino que hemos entrado
    int iColor = CCUserDefault::sharedUserDefault()->getIntegerForKey("color");
	CCDictionary* prms = CCDictionary::create();
				  prms->setObject(CCString::createWithFormat("%d", iColor), "color");
	SendMessageWithParams(string("JugadorSale"), prms);
}
