#include "HelloWorldScene.h"
#include "PhotoCuttingLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PhotoCuttingLayer* kobe = PhotoCuttingLayer::create("kobe.jpg", 3, 3);
	kobe->setAnchorPoint(Vec2(0, 0));
	kobe->setPosition(Vec2((visibleSize.width - kobe->getContentSize().width) / 2, (visibleSize.height - kobe->getContentSize().height) / 2));
	addChild(kobe);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
