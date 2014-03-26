#include "AppDelegate.h"
#include "Soomla.h"
#include "MuffinRushAssets.h"
#include "StoreAScene.h"
#include "MainScene.h"
#include "ExampleEventHandler.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    handler = new ExampleEventHandler();
}

AppDelegate::~AppDelegate() 
{
    soomla::CCSoomla::sharedSoomla()->removeEventHandler(handler);
}

bool AppDelegate::applicationDidFinishLaunching() {

    // We initialize CCStoreController and the event handler before
    // we open the store.
    soomla::CCSoomla::sharedSoomla()->addEventHandler(handler);

    MuffinRushAssets *assets = MuffinRushAssets::create();
    CCDictionary *storeParams = CCDictionary::create();
    storeParams->
            setObject(CCString::create("ExampleSoomSecret"), "soomSec");
    storeParams->
            setObject(CCString::create("ExamplePublicKey"), "androidPublicKey");
    storeParams->
            setObject(CCString::create("ExampleCustomSecret"), "customSecret");

    // This is the call to initialize CCStoreController
    soomla::CCStoreController::createShared(assets, storeParams);

    /*
     * ** Set the amount of each currency to 10,000 if the **
     * ** balance drops under 1,000                        **
     *
     * ** Of course, this is just for testing...           **
     */

    CCArray *currencies =
            soomla::CCStoreInfo::sharedStoreInfo()->getVirtualCurrencies();
    CCObject *currencyObject;
    CCARRAY_FOREACH(currencies, currencyObject) {
            soomla::CCVirtualCurrency *vc =
                    dynamic_cast<soomla::CCVirtualCurrency *>(currencyObject);
            int balance = soomla::CCStoreInventory::sharedStoreInventory()->
                    getItemBalance(vc->getItemId()->getCString(), NULL);
            if (balance < 1000) {
                soomla::CCStoreInventory::sharedStoreInventory()->
                        giveItem(vc->getItemId()->getCString(), 10000 - balance, NULL);
            }
        }

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = MainScene::getMainScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
