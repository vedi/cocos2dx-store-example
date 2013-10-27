//
// Created by Alexandr on 31.05.13.
//
// To change the template use AppCode | Preferences | File Templates.
//


#include "StoreAScene.h"
#include "MainScene.h"
#include "StoreBScene.h"
#include "SoomlaIncludes.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "EventHandler.h"

using namespace soomla;


StoreAScene::StoreAScene() {
    mBackgroundNode = NULL;
    mBackground = NULL;
    mMainNode = NULL;
    mTopNode = NULL;
    mMuffinAmount = NULL;
    mGoodsListView = NULL;
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

SEL_CCControlHandler StoreAScene::onResolveCCBCCControlSelector(CCObject *pTarget, char const *pSelectorName) {
    return NULL;
}

bool StoreAScene::onAssignCCBMemberVariable(CCObject *pTarget, char const *pMemberVariableName, CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackgroundNode", CCNode*, mBackgroundNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackground", CCLayerColor*, mBackground);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainNode", CCNode*, mMainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTopNode", CCNode*, mTopNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMuffinAmount", CCLabelTTF*, mMuffinAmount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoodsListView", CCListView*, mGoodsListView);
    if(strcmp(pMemberVariableName, "mListRows") == 0) {
        mListRows[pNode->getTag()] = static_cast<CCListViewCell*>(pNode);
        return true;
    } else if(strcmp(pMemberVariableName, "mListItem") == 0) {
        mListItem[pNode->getTag()] = dynamic_cast<LevelIconWidget*>(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBottomNode", CCNode*, mBottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonBack", CCMenuItemImage*, mButtonBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonMoreMuffins", CCMenuItemImage*, mButtonMoreMuffins);

    return false;
}

void StoreAScene::onNodeLoaded(CCNode *pNode, CCNodeLoader *pNodeLoader) {
    CC_ASSERT(mBackgroundNode);
    CC_ASSERT(mBackground);
    CC_ASSERT(mMainNode);
    CC_ASSERT(mTopNode);
    CC_ASSERT(mMuffinAmount);
    CC_ASSERT(mGoodsListView);
    for(int i = 0; i < NUMBER_OF_ROWS; i++ ) {
        CC_ASSERT(mListRows[i]);
        mListRows[i]->retain();
        mListRows[i]->removeFromParentAndCleanup(false);

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

    CCStoreController::sharedStoreController()->storeOpening();

    ////*****
    for (int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (itemId != "ERROR") {
            int tag = i;
            std::string name = " ";
            std::string info = " ";

            CCSoomlaError *soomlaError = NULL;
            CCVirtualItem *virtualItem = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId.c_str(), &soomlaError);
            if (!soomlaError) {
                name = virtualItem->getName()->getCString();
                info = virtualItem->getDescription()->getCString();
            } else {
                CCStoreUtils::logException("StoreAScene::createListViewItem", soomlaError);
            }

            soomlaError = NULL;
            int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance(itemId.c_str(), &soomlaError);
            if (soomlaError) {
                CCStoreUtils::logException("StoreAScene::setPriceBalanceLabel", soomlaError);
                balance = 0;
            }

            CCPurchasableVirtualItem *purchasableVirtualItem = dynamic_cast<CCPurchasableVirtualItem *>(virtualItem);
            float price;
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

            mListItem[i]->setData(virtualItem->getItemId()->getCString(), name.c_str(), info.c_str(), price, balance);
        }


        //mCacheListItems[data->nRow] = mListItem[data->nRow];
    }


    //////******

    mGoodsListView->setDelegate(this);
    mGoodsListView->setMode(CCListViewModeVertical);
    mGoodsListView->setSeparatorStyle(CCListViewCellSeparatorStyleNone);
    mGoodsListView->reload();

    CCSoomlaError *soomlaError = NULL;
    int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance("currency_muffin", &soomlaError);
    if (soomlaError) {
        CCStoreUtils::logException("StoreAScene::setCurrencyBalanceLabel", soomlaError);
        balance = 0;
        CCLog("Soomla balance error");
    }

    updateCurrencyBalance(CCInteger::create(balance));
}

SEL_MenuHandler StoreAScene::onResolveCCBCCMenuItemSelector(CCObject *pTarget, char const *pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", StoreAScene::onBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMoreMuffins", StoreAScene::onMoreMuffins);

    return NULL;
}

void StoreAScene::onMoreMuffins(CCObject *pSender) {
    CCScene *s = StoreBScene::getStoreBScene();
    CCDirector::sharedDirector()->setDepthTest(true);
    CCTransitionScene *transition = CCTransitionMoveInR::create(0.8f, s);

    CCDirector::sharedDirector()->replaceScene(transition);
}


void StoreAScene::onEnter() {
    CCLayer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
            callfuncO_selector(StoreAScene::updateCurrencyBalance),
            EVENT_ON_CURRENCY_BALANCE_CHANGED, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
            callfuncO_selector(StoreAScene::updateGoodBalance),
            EVENT_ON_GOOD_BALANCE_CHANGED, NULL);
}

void StoreAScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ON_CURRENCY_BALANCE_CHANGED);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ON_GOOD_BALANCE_CHANGED);
    CCLayer::onExit();
}

void StoreAScene::CCListView_cellForRow(CCListView *listView, CCListViewProtrolData *data) {
    CC_UNUSED_PARAM(listView);

    CCListViewCell *cell = mListRows[data->nRow];
    cell->setPosition(CCPointZero);
    cell->setOpacity(0);

    cell->setSelectionColor(ccc4(0, 0, 0, 0));
    data->cell = cell;
}

void StoreAScene::CCListView_numberOfCells(CCListView *listView, CCListViewProtrolData *data) {
    CC_UNUSED_PARAM(listView);

    data->nNumberOfRows = NUMBER_OF_ROWS;
}

void StoreAScene::onBack(CCObject *pSender) {
    CCStoreController::sharedStoreController()->storeClosing();

    CCScene *s = MainScene::getMainScene();
    CCDirector::sharedDirector()->setDepthTest(true);
    CCTransitionScene *transition = CCTransitionMoveInL::create(0.8f, s);

    CCDirector::sharedDirector()->replaceScene(transition);
}

int StoreAScene::tagFromItemId(const char* itemId) {
    string itemIdStr(itemId);
    if (itemIdStr == "fruit_cake") return 1;
    if (itemIdStr == "pavlova") return 2;
    if (itemIdStr == "pavlova_10") return 3;
    if (itemIdStr == "chocolate_cake") return 4;
    if (itemIdStr == "cream_cup") return 5;
    if (itemIdStr == "cream_cup_10") return 6;

    if (itemIdStr == "show_room") return 8;
    if (itemIdStr == "delivery_vehicle") return 9;

    if (itemIdStr == "fat_cat") return 11;
    if (itemIdStr == "happi_hippo") return 12;
    if (itemIdStr == "funkey_monkey") return 13;

    return 0;
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

CCScene *StoreAScene::getGoodsStoreScene() {
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();

    ccNodeLoaderLibrary->registerCCNodeLoader("StoreAScene", StoreASceneLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("LevelIconWidget", LevelIconWidgetLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("CCListView", CCListViewLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("CCListViewCell", CCListViewCellLoader::loader());

    CCBReader *ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);

    return ccbReader->createSceneWithNodeGraphFromFile("ccb/StoreAScene.ccbi");
}

void StoreAScene::updateCurrencyBalance(CCInteger *pBalance) {
    char buf[20] = "/0";
    sprintf(buf, "%i", pBalance->getValue());
    mMuffinAmount->setString(buf);
}

void StoreAScene::updateGoodBalance(CCArray *pParams) {
    soomla::CCVirtualGood *virtualGood = (CCVirtualGood *) pParams->objectAtIndex(0);
    CCInteger *balance = (CCInteger *) pParams->objectAtIndex(1);

    ////*****
    for (unsigned int i = 0; i < NUMBER_OF_ROWS; i++) {
        string itemId = itemIdFromTag(i);
        if (virtualGood->getItemId()->compare(itemId.c_str()) == 0) {
            mListItem[i]->setBalance(balance->getValue());
            break;
        }
    }
}



//Utils

float applyScaleForNode(cocos2d::CCNode* node) {
    cocos2d::CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float scale = std::min(winSize.width / node->getContentSize().width,
            winSize.height / node->getContentSize().height);
    node->setScale(scale);
    node->setPosition(ccp((winSize.width - node->getContentSize().width * scale) / 2,
    (winSize.height - node->getContentSize().height * scale) / 2));

    return scale;
}

void fill(cocos2d::CCNode* targetNode) {
    CCPoint leftBottom =
            targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
    CCPoint rightTop =
            targetNode->getParent()->convertToWorldSpace(
                    ccpAdd(targetNode->getPosition(), ccpFromSize(targetNode->getContentSize())));

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    float scaleX = winSize.width / (rightTop.x - leftBottom.x);
    float scaleY = winSize.height / (rightTop.y - leftBottom.y);

    targetNode->setScale(MAX(scaleX, scaleY));
}

void shiftToLeftBottom(cocos2d::CCNode* targetNode) {
    CCPoint position = targetNode->getParent()->convertToNodeSpace(ccp(0, 0));
    targetNode->setPosition(position);
}

void shiftToTop(cocos2d::CCNode* targetNode) {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint position =
            targetNode->getParent()->convertToNodeSpace(ccp(winSize.width, winSize.height));
    targetNode->setPositionY(position.y);
}

void shiftToBottom(cocos2d::CCNode* targetNode) {
    CCPoint position = targetNode->getParent()->convertToNodeSpace(ccp(0, 0));
    targetNode->setPositionY(position.y);
}

void fillWidth(cocos2d::CCNode* targetNode) {
    CCPoint leftBottom =
            targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
    CCPoint rightTop =
            targetNode->getParent()->convertToWorldSpace(
                    ccpAdd(targetNode->getPosition(), ccpFromSize(targetNode->getContentSize())));

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    targetNode->setScale(winSize.width / (rightTop.x - leftBottom.x));
}

void putToCenterMiddleOf(cocos2d::CCNode* targetNode, cocos2d::CCNode* anchorNode) {

    CCPoint leftBottom =
            anchorNode->convertToWorldSpaceAR(
                    ccpSub(CCPointZero, ccpCompMult(
                            ccp(anchorNode->getAnchorPoint().x, anchorNode->getAnchorPoint().y),
                            ccpFromSize(anchorNode->getContentSize()))));
    CCPoint rightTop =
            anchorNode->convertToWorldSpaceAR(
                    ccpAdd(CCPointZero, ccpCompMult(
                            ccp(1 - anchorNode->getAnchorPoint().x, 1 - anchorNode->getAnchorPoint().y),
                            ccpFromSize(anchorNode->getContentSize()))));

    CCPoint globalPosition = ccp(leftBottom.x + (rightTop.x - leftBottom.x) / 2, leftBottom.y + (rightTop.y - leftBottom.y) / 2);

    targetNode->setPosition(targetNode->getParent()->convertToNodeSpace(globalPosition));
}

