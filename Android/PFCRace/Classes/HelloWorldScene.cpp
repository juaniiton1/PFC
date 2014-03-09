#include "HelloWorldScene.h"
#include "OptionsScene.h"
#include "LevelGameScene.h"
#include "LevelRankingScene.h"
#include "GameManager.h"
#include "NDKHelper.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
}

HelloWorld::~HelloWorld()
{
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if ( !CCLayer::init() )
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

	// www.colourlovers.com/palette/298635/wooden_color_pencil
	CCMenuItem* menuPlay = CCMenuItemImage::create("btn_play.png", "btn_play_h.png", this, menu_selector(HelloWorld::menuPlayCallback));
	CCMenuItem* menuRank = CCMenuItemImage::create("btn_ranking.png", "btn_ranking_h.png", this, menu_selector(HelloWorld::menuRankCallback));
	CCMenuItem* menuSett = CCMenuItemImage::create("btn_settings.png", "btn_settings_h.png", this, menu_selector(HelloWorld::menuSettCallback));
	CCMenuItem* menuExit = CCMenuItemImage::create("btn_exit.png", "btn_exit_h.png", this, menu_selector(HelloWorld::menuExitCallback));

	menuPlay->setPosition(ccp(vs.width/2, vs.height/2 + 100 - (200 - 162.5) ));
	menuRank->setPosition(ccp(vs.width/2 - 125, vs.height/2 - (200 - 162.5) - 75 ));
	menuSett->setPosition(ccp(vs.width/2, vs.height/2 - (200 - 162.5) - 75 ));
	menuExit->setPosition(ccp(vs.width/2 + 125, vs.height/2 - (200 - 162.5) - 75 ));

	CCMenu* pMenu = CCMenu::create(menuPlay, menuSett, menuRank, menuExit, NULL);
			pMenu->setPosition(ccp(0,0));

	this->addChild(pMenu, 5);

    this->setTouchEnabled(true);

    // Inicializamos los valores de BBDD
    std::string user = CCUserDefault::sharedUserDefault()->getStringForKey("user");
    if (user == "") CCUserDefault::sharedUserDefault()->setStringForKey("user", "Player 1");

    return true;
}

void HelloWorld::menuPlayCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(LevelGameScene::scene());
}

void HelloWorld::menuRankCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(LevelRankingScene::scene());
}

void HelloWorld::menuSettCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(OptionsScene::scene());
}

void HelloWorld::menuExitCallback(CCObject* pSender)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	#else
		CCDirector::sharedDirector()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	#endif
}
