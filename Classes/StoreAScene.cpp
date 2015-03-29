//
// Created by Alexandr on 31.05.13.
//


#include "StoreAScene.h"
#include "MainScene.h"
#include "StoreBScene.h"
#include "ExampleEventHandler.h"

using namespace soomla;


StoreAScene::StoreAScene() {
    mBackgroundNode = NULL;
    mBackground = NULL;
    mMainNode = NULL;
    mTopNode = NULL;
    mMuffinAmount = NULL;
    mGoodsTableView = NULL;
    for(int i = 0; i < NUMBER_OF_ROWS; i++ ) {
        mListRows[i] = NULL;
        mListItem[i] = NULL;
        mCacheListItems[i] = NULL;
    }
    mBottomNode = NULL;
    mButtonBack = NULL;
    mButtonMoreMuffins = NULL;
}

StoreAScene::~StoreAScene() {
    for(int i = 0; i < NUMBER_OF_ROWS; i++) {
        CC_SAFE_RELEASE(mListRows[i]);
    }
}

bool StoreAScene::onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackgroundNode", Node*, mBackgroundNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackground", LayerColor*, mBackground)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainNode", Node*, mMainNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTopNode", Node*, mTopNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMuffinAmount", Label*, mMuffinAmount)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoodsTableView", TableView*, mGoodsTableView)
    if(strcmp(pMemberVariableName, "mListRows") == 0) {
        mListRows[pNode->getTag()] = static_cast<TableViewCell*>(pNode);
        return true;
    } else if(strcmp(pMemberVariableName, "mListItem") == 0) {
        mListItem[pNode->getTag()] = dynamic_cast<LevelIconWidget*>(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBottomNode", Node*, mBottomNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonBack", MenuItemImage*, mButtonBack)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonMoreMuffins", MenuItemImage*, mButtonMoreMuffins)

    return false;
}

void StoreAScene::onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader) {
    CC_UNUSED_PARAM(pNode);
    CC_UNUSED_PARAM(pNodeLoader);

    CC_ASSERT(mBackgroundNode);
    CC_ASSERT(mBackground);
    CC_ASSERT(mMainNode);
    CC_ASSERT(mTopNode);
    CC_ASSERT(mMuffinAmount);
        CC_ASSERT(mGoodsTableView);
    for(int i = 0; i < NUMBER_OF_ROWS; i++ ) {
        CC_ASSERT(mListRows[i]);
        mListRows[i]->retain();
        mListRows[i]->removeFromParentAndCleanup(false);
        mListRows[i]->setPosition(Vec2::ZERO);

        CC_ASSERT(mListItem[i]);
    }
    CC_ASSERT(mBottomNode);
    CC_ASSERT(mButtonBack);
    CC_ASSERT(mButtonMoreMuffins);

    applyScaleForNode(this);
    fill(mBackgroundNode);
    putToCenterMiddleOf(mMainNode, mBackgroundNode);
    shiftToTop(mTopNode);
    shiftToBottom(mBottomNode);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCSoomlaStore::getInstance()->startIabServiceInBg();
#endif

    ////*****
    for (int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (itemId != "ERROR") {
            std::string name = " ";
            std::string info = " ";

            CCError *soomlaError = NULL;

            CCVirtualItem *virtualItem;
            virtualItem = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId.c_str(), &soomlaError);
            if (!soomlaError) {
                name = virtualItem->getName()->getCString();
                info = virtualItem->getDescription()->getCString();
            } else {
                CCSoomlaUtils::logException("StoreAScene::createListViewItem", soomlaError);
            }

            soomlaError = NULL;
            int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance(itemId.c_str(), &soomlaError);
            if (soomlaError) {
                CCSoomlaUtils::logException("StoreAScene::getItemBalance", soomlaError);
                balance = 0;
                CC_ASSERT(false);
            }
            if (itemId.compare("show_room") == 0 || itemId.compare("delivery_vehicle") == 0) {
                setProgressForItem(itemId, mListItem[i]);
            }
            else if (itemId.compare("fat_cat") == 0
                    || itemId.compare("happi_hippo") == 0
                    || itemId.compare("funkey_monkey") == 0) {
                setEquippedForItem(itemId, mListItem[i]);
            }

            CCPurchasableVirtualItem *purchasableVirtualItem = dynamic_cast<CCPurchasableVirtualItem *>(virtualItem);
            double price;
            if (purchasableVirtualItem != NULL) {
                CCPurchaseType *purchaseType = purchasableVirtualItem->getPurchaseType();
                if (dynamic_cast<CCPurchaseWithMarket *>(purchaseType)) {
                    price = static_cast<CCPurchaseWithMarket *>(purchaseType)->getMarketItem()->getPrice()->getValue();
                } else if (dynamic_cast<CCPurchaseWithVirtualItem *>(purchaseType)) {
                    price = static_cast<CCPurchaseWithVirtualItem *>(purchaseType)->getAmount()->getValue();
                } else {
                    price = -1;
                }
            } else {
                price = -1;
            }

            mListItem[i]->setData(itemId.c_str(), name.c_str(), info.c_str(), price, balance);
        }
    }

    //////******
    mGoodsTableView->setBounceable(true);
    mGoodsTableView->setDataSource(this);
    mGoodsTableView->setDelegate(this);
    mGoodsTableView->reloadData();

    CCError *soomlaError = NULL;
    int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance("currency_muffin", &soomlaError);
    if (soomlaError) {
        CCSoomlaUtils::logException("StoreAScene::setCurrencyBalanceLabel", soomlaError);
        balance = 0;
        CCLOG("Soomla balance error");
    }

    updateCurrencyBalance(CCInteger::create(balance));
}

SEL_MenuHandler StoreAScene::onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", StoreAScene::onBack)
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMoreMuffins", StoreAScene::onMoreMuffins)

    return NULL;
}

void StoreAScene::onMoreMuffins(Ref *pSender) {
    CC_UNUSED_PARAM(pSender);

    Scene *s = StoreBScene::getStoreBScene();
    CCDirector::getInstance()->setDepthTest(true);
    TransitionScene *transition = TransitionMoveInR::create(0.8f, s);

    CCDirector::getInstance()->replaceScene(transition);
}


void StoreAScene::onEnter() {
    CCLayer::onEnter();
    
    currencyBalanceChangedHandler = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED,
                                                                          CC_CALLBACK_1(StoreAScene::onCurrencyBalanceChanged, this));
    goodBalanceChangedHandler = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED,
                                                                          CC_CALLBACK_1(StoreAScene::updateGoodBalance, this));
    goodEquippedHandler = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_GOOD_EQUIPPED,
                                                                          CC_CALLBACK_1(StoreAScene::onGoodEquipped, this));
    goodUnequippedHandler = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_GOOD_UNEQUIPPED,
                                                                          CC_CALLBACK_1(StoreAScene::onGoodUnEquipped, this));
    goodUpgradeHandler = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_GOOD_UPGRADE,
                                                                          CC_CALLBACK_1(StoreAScene::onGoodUpgrade, this));
}

void StoreAScene::onExit() {
    Director::getInstance()->getEventDispatcher()->removeEventListener(currencyBalanceChangedHandler);
    Director::getInstance()->getEventDispatcher()->removeEventListener(goodBalanceChangedHandler);
    Director::getInstance()->getEventDispatcher()->removeEventListener(goodEquippedHandler);
    Director::getInstance()->getEventDispatcher()->removeEventListener(goodUnequippedHandler);
    Director::getInstance()->getEventDispatcher()->removeEventListener(goodUpgradeHandler);
    Layer::onExit();
}

void StoreAScene::onBack(Ref *pSender) {
    CC_UNUSED_PARAM(pSender);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCSoomlaStore::getInstance()->stopIabServiceInBg();
#endif

    Scene *s = MainScene::getMainScene();
    CCDirector::getInstance()->setDepthTest(true);
    TransitionScene *transition = TransitionMoveInL::create(0.8f, s);

    CCDirector::getInstance()->replaceScene(transition);
}

string StoreAScene::itemIdFromTag(int tag) {
    string ret;
    switch (tag) {
        case 1:
            ret = "fruit_cake";
            return ret;
        case 2:
            ret = "pavlova";
            return ret;
        case 3:
            ret = "pavlova_10";
            return ret;
        case 4:
            ret = "chocolate_cake";
            return ret;
        case 5:
            ret = "cream_cup";
            return ret;
        case 6:
            ret = "cream_cup_10";
            return ret;
        case 8:
            ret = "show_room";
            return ret;
        case 9:
            ret = "delivery_vehicle";
            return ret;
        case 11:
            ret = "fat_cat";
            return ret;
        case 12:
            ret = "happi_hippo";
            return ret;
        case 13:
            ret = "funkey_monkey";
            return ret;
        default:
            ret = "ERROR";
            return ret;
    }
}

Scene *StoreAScene::getGoodsStoreScene() {
    NodeLoaderLibrary * ccNodeLoaderLibrary = NodeLoaderLibrary::getInstance();

    ccNodeLoaderLibrary->registerNodeLoader("StoreAScene", StoreASceneLoader::loader());
    ccNodeLoaderLibrary->registerNodeLoader("LevelIconWidget", LevelIconWidgetLoader::loader());
    ccNodeLoaderLibrary->registerNodeLoader("CCTableView", CCTableViewLoader::loader());
    ccNodeLoaderLibrary->registerNodeLoader("CCTableViewCell", CCTableViewCellLoader::loader());

    CCBReader *ccbReader = new CCBReader(ccNodeLoaderLibrary);

    return ccbReader->createSceneWithNodeGraphFromFile("ccb/StoreAScene.ccbi");
}

void StoreAScene::onCurrencyBalanceChanged(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    __Integer *pBalance = dynamic_cast<__Integer *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_BALANCE));
    
    updateCurrencyBalance(pBalance);
}

void StoreAScene::updateCurrencyBalance(__Integer *pBalance) {
    char buf[20] = "/0";
    sprintf(buf, "%i", pBalance->getValue());
    mMuffinAmount->setString(buf);
    
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        mListItem[i]->updateAfford();
    }
}

void StoreAScene::updateGoodBalance(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    soomla::CCVirtualGood *virtualGood = dynamic_cast<CCVirtualGood *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_GOOD));
    __Integer *balance = dynamic_cast<__Integer *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_BALANCE));

    ////*****
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (virtualGood->getItemId()->compare(itemId.c_str()) == 0) {
            mListItem[i]->setBalance(balance->getValue());
            break;
        }
    }
}


void StoreAScene::onGoodEquipped(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    soomla::CCEquippableVG *virtualGood = dynamic_cast<CCEquippableVG *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG));
    
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (virtualGood->getItemId()->compare(itemId.c_str()) == 0) {
            mListItem[i]->setEquiped(true);
            break;
        }
    }
}

void StoreAScene::onGoodUnEquipped(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    soomla::CCEquippableVG *virtualGood = dynamic_cast<CCEquippableVG *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG));
    
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (virtualGood->getItemId()->compare(itemId.c_str()) == 0) {
            mListItem[i]->setEquiped(false);
            break;
        }
    }
}

void StoreAScene::onGoodUpgrade(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    soomla::CCVirtualGood *virtualGood = dynamic_cast<CCVirtualGood *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_GOOD));
    
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (virtualGood->getItemId()->compare(itemId.c_str()) == 0) {
            setProgressForItem(itemId, mListItem[i]);
            break;
        }
    }
}

TableViewCell* StoreAScene::tableCellAtIndex(TableView *table, ssize_t idx) {
    CC_UNUSED_PARAM(table);

    return mListRows[NUMBER_OF_ROWS - idx - 1];
}

ssize_t StoreAScene::numberOfCellsInTableView(TableView *table) {
    CC_UNUSED_PARAM(table);

    return NUMBER_OF_ROWS;
}

Size StoreAScene::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    CC_UNUSED_PARAM(table);

    return mListRows[NUMBER_OF_ROWS - idx - 1]->getContentSize();
}

void StoreAScene::setProgressForItem(string &itemId, LevelIconWidget *pWidget) {
    CCError *soomlaError = NULL;
    int progress = CCStoreInventory::sharedStoreInventory()->getGoodUpgradeLevel(itemId.c_str(), &soomlaError);
    if (soomlaError) {
        CCSoomlaUtils::logException("StoreAScene::setProgressForItem", soomlaError);
        progress = 0;
        CC_ASSERT(false);
    }
    pWidget->setProgress(progress);
}

void StoreAScene::setEquippedForItem(string &itemId, LevelIconWidget *pWidget) {
    CCError *soomlaError = NULL;
    bool equipped = CCStoreInventory::sharedStoreInventory()->isVirtualGoodEquipped(itemId.c_str(), &soomlaError);
    if (soomlaError) {
        CCSoomlaUtils::logException("StoreAScene::setEquippedForItem", soomlaError);
        equipped = false;
        CC_ASSERT(false);
    }
    pWidget->setEquiped(equipped);
}

//Utils

float applyScaleForNode(cocos2d::Node* node) {
    cocos2d::Size winSize = CCDirector::getInstance()->getWinSize();
    float scale = std::min(winSize.width / node->getContentSize().width,
            winSize.height / node->getContentSize().height);
    node->setScale(scale);
    node->setPosition(Vec2((winSize.width - node->getContentSize().width * scale) / 2,
    (winSize.height - node->getContentSize().height * scale) / 2));

    return scale;
}

void fill(cocos2d::Node* targetNode) {
    Vec2 leftBottom =
            targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
    Vec2 rightTop =
            targetNode->getParent()->convertToWorldSpace(
                    targetNode->getPosition() + Vec2(targetNode->getContentSize()));

    Size winSize = CCDirector::getInstance()->getWinSize();

    float scaleX = winSize.width / (rightTop.x - leftBottom.x);
    float scaleY = winSize.height / (rightTop.y - leftBottom.y);

    targetNode->setScale(MAX(scaleX, scaleY));
}

void shiftToLeftBottom(cocos2d::Node* targetNode) {
    Vec2 position = targetNode->getParent()->convertToNodeSpace(Vec2(0, 0));
    targetNode->setPosition(position);
}

void shiftToTop(cocos2d::Node* targetNode) {
    Size winSize = CCDirector::getInstance()->getWinSize();
    Vec2 position =
            targetNode->getParent()->convertToNodeSpace(Vec2(winSize.width, winSize.height));
    targetNode->setPositionY(position.y);
}

void shiftToBottom(cocos2d::Node* targetNode) {
    Vec2 position = targetNode->getParent()->convertToNodeSpace(Vec2(0, 0));
    targetNode->setPositionY(position.y);
}

void fillWidth(cocos2d::Node* targetNode) {
    Vec2 leftBottom =
            targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
    Vec2 rightTop =
            targetNode->getParent()->convertToWorldSpace(
                    targetNode->getPosition() + Vec2(targetNode->getContentSize()));

    Size winSize = CCDirector::getInstance()->getWinSize();

    targetNode->setScale(winSize.width / (rightTop.x - leftBottom.x));
}

void putToCenterMiddleOf(cocos2d::Node* targetNode, cocos2d::Node* anchorNode) {

    Vec2 leftBottom =
            anchorNode->convertToWorldSpaceAR(
                                              Vec2::ZERO - ccpCompMult(
                                                                       Vec2(anchorNode->getAnchorPoint().x, anchorNode->getAnchorPoint().y),
                                                                       Vec2(anchorNode->getContentSize())));
    Vec2 rightTop =
            anchorNode->convertToWorldSpaceAR(
                                              Vec2::ZERO + ccpCompMult(
                                                                       Vec2(1 - anchorNode->getAnchorPoint().x, 1 - anchorNode->getAnchorPoint().y),
                                                                       Vec2(anchorNode->getContentSize())));

    Vec2 globalPosition = Vec2(leftBottom.x + (rightTop.x - leftBottom.x) / 2, leftBottom.y + (rightTop.y - leftBottom.y) / 2);

    targetNode->setPosition(targetNode->getParent()->convertToNodeSpace(globalPosition));
}
