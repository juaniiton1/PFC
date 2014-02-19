/*
 * MainMenuScene.cpp
 *
 *  Created on: 31/01/2014
 *      Author: Joan
 */

#include "GameScene.h"
#include "GameManager.h"
#include "HelloWorldScene.h"
#include "NDKHelper.h"

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

GameScene::GameScene(): _car(NULL), _road(NULL), _labelVel(NULL), _labelDist(NULL), _oppCars(NULL)
{
	_vel = 0.0;
	_dist = 0.0;
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
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Layer de arriba con el indicadores
	CCLayerColor* layer_menu = CCLayerColor::create(ccc4(255,255,255,255));
				  layer_menu->setContentSize(CCSizeMake(visibleSize.width, 100));
				  layer_menu->setPosition(ccp(0, visibleSize.height-100));

	// Boton de volver (menu)
	CCMenuItem* button_back = CCMenuItemImage::create("btn_volver.png", "btn_volver_h.png", this, menu_selector(GameScene::menuBackCallback));
				button_back->setPosition(ccp(105, 50));

	CCMenu* menu = CCMenu::create(button_back, NULL);
			menu->setPosition(ccp(0,0));

	// Velocidad
	CCString* stringVel = CCString::createWithFormat("%d km/h", (int)_vel);
	_labelVel = CCLabelTTF::create(stringVel->getCString(), "Helvetica", 20, CCSizeMake(100, 30),
																					kCCTextAlignmentCenter);
	_labelVel->setColor(ccc3(0,0,0));
	_labelVel->setPosition(ccp(visibleSize.width-70, 50));

	// Distancia
	CCString* stringDist = CCString::createWithFormat("%d m", (int)_dist);
	_labelDist = CCLabelTTF::create(stringDist->getCString(), "Helvetica", 20, CCSizeMake(100, 30),
																					kCCTextAlignmentCenter);
	_labelDist->setColor(ccc3(0,0,0));
	_labelDist->setPosition(ccp(visibleSize.width/2, 50));

	// Hierba
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("texture_grass.jpg");
				 texture->setTexParameters(&tp);
	_grass = CCSprite::createWithTexture(texture, CCRectMake(0, 0, visibleSize.width, visibleSize.height + 256));
	_grass->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 + 128 ) );

	// Carretera
	texture = CCTextureCache::sharedTextureCache()->addImage("texture_road.jpg");
	texture->setTexParameters(&tp);
	_road = CCSprite::createWithTexture(texture, CCRectMake(0, 0, visibleSize.width * 0.7, visibleSize.height));
	_road->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 ) );

	// Pianos
	texture = CCTextureCache::sharedTextureCache()->addImage("piano.png");
	texture->setTexParameters(&tp);
	_piano1 = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 64, visibleSize.height + 64));
	_piano1->setPosition( ccp(visibleSize.width * 0.15, visibleSize.height / 2 + 32) );
	_piano2 = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 64, visibleSize.height + 64));
	_piano2->setPosition( ccp(visibleSize.width * 0.85, visibleSize.height / 2 + 32) );

	// Coche
	_car = CCSprite::create("car_behind.png");
	_car->setPosition(ccp(visibleSize.width/2, 50));

	// Anyadimos los elementos a la vista
	this->addChild(_grass, 1);
	this->addChild(_road, 2);
	this->addChild(_piano1, 2);
	this->addChild(_piano2, 2);
	this->addChild(_car, 3);
	this->addChild(layer_menu, 10);

	// Anyadimos los elementos al layer de arriba
	layer_menu->addChild(menu);
	layer_menu->addChild(_labelVel);
	layer_menu->addChild(_labelDist);

	// Inicializamos el vector de coches en contra
	_oppCars = new CCArray;

	// Activamos el acelerometro y el touch
	this->setAccelerometerEnabled(true);
    this->setTouchEnabled(true);

    // Llamamos cada 0,1 segundos a la logica del juego
    this->schedule( schedule_selector(GameScene::gameLogic), 0.1 );
    this->schedule( schedule_selector(GameScene::updateFrame) );

    return true;
}

void GameScene::updateFrame(float dt)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	long cmDist 	= _dist*100;
	int offsetDist 	= cmDist % 667;
	int offsetY		= offsetDist / 10;

	_piano1->setPosition( ccp(visibleSize.width * 0.15, visibleSize.height / 2 + 32 - offsetY) );
	_piano2->setPosition( ccp(visibleSize.width * 0.85, visibleSize.height / 2 + 32 - offsetY) );

	offsetDist 	= cmDist % 2667;
	offsetY		= offsetDist / 10;

	_grass->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 + 128 - offsetY) );
}

void GameScene::gameLogic(float dt)
{
	// Actualizamos velocidad
	CCString* stringVel = CCString::createWithFormat("%d km/h", (int)_vel);
	_labelVel->setString(stringVel->getCString());

	// Calculamos al distancia recorrida en 0,1s a velocidad _vel
	// 1km/h = 1000m/3600s = 1000m/36000ds (1ds = 0,1s)
	float distAnt = _dist;
	_dist += _vel/36;

	// Actualizamos distancia
	CCString* stringDist = CCString::createWithFormat("%d m", (int)_dist);
	_labelDist->setString(stringDist->getCString());

	// Tamanyos del coche y la carretera
	CCRect roadRec 	= _road->boundingBox();
	CCSize carSize 	= _car->getContentSize();

	// Cada 500 metros sacamos un coche en contra
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
		oppCar->setPosition( ccp( posX, roadRec.size.height ));
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
		if ( newPos.y > roadRec.size.height * 2 || newPos.y < 0 - carSize.height/2 ) {
			this->removeChild(auxCar);
			_oppCars->removeObject(auxCar);
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

    ptTemp.x += accX * 10.0;

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
}

void GameScene::menuBackCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
