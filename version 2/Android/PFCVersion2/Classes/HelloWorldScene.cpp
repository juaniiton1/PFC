#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
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
	if ( !CCLayerColor::initWithColor( ccc4(100, 100, 100, 255) ) )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCMenuItemFont *pEnterItem = CCMenuItemFont::create("Entrar", this,
													menu_selector(HelloWorld::menuEnterCallback));
	pEnterItem->setFontSize(35);
	pEnterItem->setFontName("Helvetica");

	CCMenuItemFont *pConfigItem = CCMenuItemFont::create("Configurar", this,
													menu_selector(HelloWorld::menuConfigCallback));
	pConfigItem->setFontSize(35);
	pConfigItem->setFontName("Helvetica");

	CCMenuItemFont *pCloseItem = CCMenuItemFont::create("Salir", this,
													menu_selector(HelloWorld::menuCloseCallback));
	pCloseItem->setFontSize(35);
	pCloseItem->setFontName("Helvetica");

	CCMenu* pMenu = CCMenu::create(pEnterItem, pConfigItem, pCloseItem, NULL);
	pMenu->alignItemsVertically();
	this->addChild(pMenu, 1);

    this->setTouchEnabled(true);

    return true;
}

void HelloWorld::menuEnterCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void HelloWorld::menuConfigCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(OptionsScene::scene());
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
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
