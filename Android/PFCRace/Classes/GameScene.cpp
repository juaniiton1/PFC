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
	_car = NULL;
	_road = NULL;
	_road2 = NULL;
	_grass = NULL;
	_labelVel = NULL;
	_labelDist = NULL;
	_labelStart = NULL;
	_oppCars = NULL;
	_piano1 = NULL;
	_piano2 = NULL;
	_curvaI = NULL;
	_curvaD = NULL;
	_vel = 0.0;
	_dist = 0.0;
	_started = false;
	_nextCurva = 0;
	_fuerzaCurva = 0;
}

GameScene::~GameScene()
{
	if (_oppCars) {
		_oppCars->release();
		_oppCars = NULL;
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

	// Layer de arriba con el indicadores
	/*CCLayerColor* layer_menu = CCLayerColor::create(ccc4(255,255,255,255));
				  layer_menu->setContentSize(CCSizeMake(vs.width, 100));
				  layer_menu->setPosition(ccp(0, vs.height-100));*/

	// Boton de volver (menu)
	CCMenuItem* button_back = CCMenuItemImage::create("btn_volver.png", "btn_volver_h.png", this, menu_selector(GameScene::menuBackCallback));
				button_back->setPosition(ccp(95, vs.height - 50));

	CCMenu* menu = CCMenu::create(button_back, NULL);
			menu->setPosition(ccp(0,0));

	// Velocidad
	CCString* stringVel = CCString::createWithFormat("%d km/h", (int)_vel);
	_labelVel = CCLabelTTF::create(stringVel->getCString(), "Helvetica", 30, CCSizeMake(140, 50), kCCTextAlignmentCenter);
	_labelVel->setColor(ccc3(255,255,255));
	_labelVel->enableStroke(ccc3(0,0,0), 3.0);
	_labelVel->setPosition(ccp(vs.width - 72, 50));

	// Distancia
	CCString* stringDist = CCString::createWithFormat("%d m", (int)_dist);
	_labelDist = CCLabelTTF::create(stringDist->getCString(), "Helvetica", 30, CCSizeMake(140, 50), kCCTextAlignmentCenter);
	_labelDist->setColor(ccc3(255,255,255));
	_labelDist->enableStroke(ccc3(0,0,0), 3.0);
	_labelDist->setPosition(ccp(72, 50));

	// Click to start
	_labelStart = CCLabelTTF::create("Click to Start", "Helvetica", 50, CCSizeMake(200, 200), kCCTextAlignmentCenter);
	_labelStart->setColor(ccc3(255,255,255));
	_labelStart->enableStroke(ccc3(0,0,0), 3.0);
	_labelStart->setPosition(ccp(vs.width/2, vs.height/2));

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
	_car = CCSprite::create("car_behind.png");
	_car->setPosition(ccp(vs.width/2, 50));

	// Creamos los indicadores de curva
	_curvaI = CCSprite::create("curva_izq.png", CCRectMake(0,0,150,150));
	_curvaI->setPosition(ccp(vs.width/2, vs.height/4*3));
	_curvaI->retain();
	_curvaD = CCSprite::create("curva_der.png", CCRectMake(0,0,150,150));
	_curvaD->setPosition(ccp(vs.width/2, vs.height/4*3));
	_curvaD->retain();

	// Anyadimos los elementos a la vista
	this->addChild(_grass, 1);
	this->addChild(_road, 2);
	//this->addChild(_piano1, 2);
	//this->addChild(_piano2, 2);
	this->addChild(_car, 3);
	this->addChild(menu, 5);
	this->addChild(_labelVel, 5);
	this->addChild(_labelDist, 5);
	this->addChild(_labelStart, 5);
	//this->addChild(layer_menu, 10);

	// Anyadimos los elementos al layer de arriba
	/*layer_menu->addChild(menu);
	layer_menu->addChild(_labelVel);
	layer_menu->addChild(_labelDist);*/

	// Inicializamos el vector de curvas
	_curvas.push_back(Curva(250, 150, 2, false));
	_curvas.push_back(Curva(800, 300, 5, true));

	// Inicializamos el vector de coches en contra
	_oppCars = new CCArray;

	// Activamos el acelerometro y el touch
    this->setTouchEnabled(true);

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
			CCFiniteTimeAction* oppJump = CCSpawn::create(	CCJumpBy::create(4, ccp(500,0), 20, 12),
															CCRotateBy::create(4, 2160),
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

	// Tamanyos del coche y la carretera
	CCRect roadRec 	= _road->boundingBox();
	CCSize carSize 	= _car->getContentSize();

	// Cada 100 metros sacamos un coche en contra
	if ( (int) (_dist / DIS_CAR) > (int) (distAnt / DIS_CAR) ) {

		int velRandom = rand() % VEL_OPP;

		CCSprite* oppCar = CCSprite::create("car_behind.png", CCRectMake(0,0,100,84));
				  oppCar->setTag(velRandom);
		_oppCars->addObject(oppCar);

		// Calculamos la posicion de salida (random)
		int rangeX 	= roadRec.size.width - carSize.width;
		int minY 	= roadRec.origin.x + carSize.width/2;
		int posX 	= ( rand() % rangeX ) + minY;

		// Colocamos y mostramos el coche
		oppCar->setPosition( ccp( posX, vs.height + carSize.height ));
		this->addChild(oppCar, 3);
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
		if ( auxPos.y > vs.height * 2 || auxPos.y < 0 - carSize.height/2 ) {
			this->removeChild(auxCar);
			_oppCars->removeObject(auxCar);
		}
	}

	if (_nextCurva < _curvas.size())
	{
		int startCurva 	= _curvas[_nextCurva].getDist();
		int endCurva 	= startCurva + _curvas[_nextCurva].getLong();

		/*CCLOG("/////CURVA?/////");
		CCLOG("startCurva = %d | endCurva = %d", startCurva, endCurva);
		CCLOG("distAnt = %f | _dist = %f", distAnt, _dist);*/

		// Comprobamos si viene curva 50 m antes de esta
		if (_dist >= startCurva - 50 && startCurva - 50 >= distAnt) {
			if (_curvas[_nextCurva].getIzq()) {
				this->addChild(_curvaI, 5);
			} else {
				this->addChild(_curvaD, 5);
			}
		}

		// Si estamos en curva lo hacemos parpadear
		else if (_dist >= startCurva && startCurva >= distAnt) {
			CCRepeatForever *parpadeo = CCRepeatForever::create( CCBlink::create(1, 2) );
			if (_curvas[_nextCurva].getIzq()) {
				_curvaI->runAction(parpadeo);
				_fuerzaCurva = 1 * _curvas[_nextCurva].getFuerza();
			} else {
				_curvaD->runAction(parpadeo);
				_fuerzaCurva = -1 * _curvas[_nextCurva].getFuerza();
			}
		}

		// Si salimos de la curva quitamos el indicador
		else if (_dist >= endCurva && endCurva >= distAnt) {

			if (_curvas[_nextCurva].getIzq()) {
				_curvaI->stopAllActions();
				this->removeChild(_curvaI);
			} else {
				_curvaD->stopAllActions();
				this->removeChild(_curvaD);
			}
			_nextCurva++;
			_fuerzaCurva = 0;
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

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	if (!_started) {
		_started = true;

	    // Llamamos cada 0,1 segundos a la logica del juego
	    this->schedule( schedule_selector(GameScene::gameLogic), 0.1 );
	    this->schedule( schedule_selector(GameScene::updateFrame) );

	    // Activamos el acelerometro y el touch
	    this->setAccelerometerEnabled(true);

	    // Quitamos el "Click to start"
	    this->removeChild(_labelStart);
	}
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
