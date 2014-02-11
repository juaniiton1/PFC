#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
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

	// Fondo con la textura de madera
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("purty_wood.png");
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	texture->setTexParameters(&tp);
	CCSprite *background = CCSprite::createWithTexture(texture,
			CCRectMake(0, 0, visibleSize.width, visibleSize.height));
	background->setPosition( ccp( visibleSize.width/2, visibleSize.height/2 ) );

	this->addChild(background, 1);

	CCLOG("origin.x=%f | origin.y=%f | visibleSize.width=%f | visibleSize.height=%f",
				origin.x, origin.y, visibleSize.width, visibleSize.height);
	CCLOG("vS.width: %f ; vS.height: %f", visibleSize.width, visibleSize.height);

	// www.colourlovers.com/palette/298635/wooden_color_pencil
	CCMenuItem *pEnterItem  = CCMenuItemImage::create("icon_bulb.png", "icon_bulb_hover.png", this, menu_selector(HelloWorld::menuEnterCallback));
	CCMenuItem *pConfigItem = CCMenuItemImage::create("icon_settings.png", "icon_settings_hover.png", this, menu_selector(HelloWorld::menuConfigCallback));
	CCMenuItem *pCloseItem  = CCMenuItemImage::create("icon_exit.png", "icon_exit_hover.png", this, menu_selector(HelloWorld::menuCloseCallback));

	CCMenu* pMenu = CCMenu::create(pEnterItem, pConfigItem, pCloseItem, NULL);
	pMenu->alignItemsHorizontallyWithPadding(60);

	this->addChild(pMenu, 5);

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
