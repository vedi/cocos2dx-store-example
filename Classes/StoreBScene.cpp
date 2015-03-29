/*
 * Copyright (C) 2012 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StoreBScene.h"

#include "Cocos2dxStore.h"
#include "StoreAScene.h"
#include "ExampleEventHandler.h"

/**
 * This is the second scene (the window where the VirtualCurrencyPacks are shown).
 * In this file you can find example calls to the store's interfaces.
 **/

USING_NS_CC;
using namespace soomla;

Scene* StoreBScene::getStoreBScene() {
    NodeLoaderLibrary * ccNodeLoaderLibrary = NodeLoaderLibrary::getInstance();

    ccNodeLoaderLibrary->registerNodeLoader("StoreBScene", StoreBSceneLoader::loader());

    CCBReader *ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->retain();
    Scene *pScene = ccbReader->createSceneWithNodeGraphFromFile("ccb/StoreBScene.ccbi");
    ccbReader->release();
    return pScene;
}

bool StoreBScene::init() {

    for (unsigned int i = 0; i < GOODS_NUMBER; i++) {
        mGoodTitles[i]       = NULL;
        mGoodDescriptions[i] = NULL;
        mPrices[i]           = NULL;
    }

    return true;
}

void StoreBScene::onBack(Ref* pSender) {

    CC_UNUSED_PARAM(pSender);

    Scene *s = StoreAScene::getGoodsStoreScene();
    CCDirector::getInstance()->setDepthTest(true);
    TransitionScene *transition = TransitionMoveInL::create(0.8f, s);

    CCDirector::getInstance()->replaceScene(transition);
}


void StoreBScene::onBuy(Ref* pSender) {
    if (pSender) {
        int tag = ((Node*)pSender)->getTag();
        string itemId = itemIdFromTag(tag);
        CCError *soomlaError = NULL;
        CCStoreInventory::sharedStoreInventory()->buyItem(itemId.c_str(), &soomlaError);
        if (soomlaError) {
            CCSoomlaUtils::logException("StoreBScene::onBuy", soomlaError);
            return;
        }
    }
}

string StoreBScene::itemIdFromTag(int tag) {
    switch (tag) {
        case 0: return "no_ads";
        case 1: return "muffins_10";
        case 2: return "muffins_50";
        case 3: return "muffins_400";
        case 4: return "muffins_1000";
        default: return "ERROR";
    }
}

void StoreBScene::onEnter() {
    Layer::onEnter();
    
    currencyBalanceChangedListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener(CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED,
                                                                                                           CC_CALLBACK_1(StoreBScene::onCurrencyBalanceChanged, this));
}

void StoreBScene::onExit() {
    Director::getInstance()->getEventDispatcher()->removeEventListener(currencyBalanceChangedListener);
    CCLayer::onExit();
}

void StoreBScene::onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader) {

    CC_UNUSED_PARAM(pNode);
    CC_UNUSED_PARAM(pNodeLoader);

    CC_ASSERT(mBackgroundNode);
    CC_ASSERT(mMainNode);
    CC_ASSERT(mTopNode);
    CC_ASSERT(mBottomNode);
    CC_ASSERT(mMuffinAmount);

    for (unsigned int i = 0; i < GOODS_NUMBER; i++) {
        CC_ASSERT(mGoodTitles[i]       );
        CC_ASSERT(mGoodDescriptions[i] );
        CC_ASSERT(mPrices[i]           );

        CCError *soomlaError = NULL;
        CCVirtualItem *virtualItem = CCStoreInfo::sharedStoreInfo()->getItemByItemId(
                mGoodTitles[i]->getString().c_str(), &soomlaError);
        if (soomlaError) {
            CCSoomlaUtils::logException("StoreBScene", soomlaError);
            continue;
        }
        CC_ASSERT(virtualItem);
        CCPurchasableVirtualItem *purchasableVirtualItem = dynamic_cast<CCPurchasableVirtualItem *>(virtualItem);
        CC_ASSERT(purchasableVirtualItem);
        CCPurchaseWithMarket *purchaseWithMarket = dynamic_cast<CCPurchaseWithMarket *>(purchasableVirtualItem->getPurchaseType());
        CC_ASSERT(purchaseWithMarket);
        double price = purchaseWithMarket->getMarketItem()->getPrice()->getValue();

        mGoodTitles[i]->setString(virtualItem->getName()->getCString());
        mGoodDescriptions[i]->setString(virtualItem->getDescription()->getCString());
        mPrices[i]->setString(CCString::createWithFormat("%.2f", price)->getCString());

    }

    applyScaleForNode(this);
    fill(mBackgroundNode);
    shiftToTop(mTopNode);
    shiftToBottom(mBottomNode);

    this->setKeypadEnabled(true);

    CCError *soomlaError = NULL;
    int balance = CCStoreInventory::sharedStoreInventory()->getItemBalance("currency_muffin", &soomlaError);
    if (soomlaError) {
        CCSoomlaUtils::logException("StoreBScene", soomlaError);
        balance = 0;
        CCLOG("Soomla balance error");
    }
    updateCurrencyBalance(CCInteger::create(balance));
}

SEL_MenuHandler StoreBScene::onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBuy", StoreBScene::onBuy)
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", StoreBScene::onBack)
    return NULL;
}

bool StoreBScene::onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackgroundNode", Node *, mBackgroundNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainNode", Node *, mMainNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTopNode", Node *, mTopNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBottomNode", Node *, mBottomNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMuffinAmount", Label *, mMuffinAmount)

    if (strcmp(pMemberVariableName, ("mGoodTitles")) == 0) {
        mGoodTitles[pNode->getTag()] = (Label *) pNode;
        return true;
    }
    if (strcmp(pMemberVariableName, ("mGoodDescriptions")) == 0) {
        mGoodDescriptions[pNode->getTag()] = (Label *) pNode;
        return true;
    }
    if (strcmp(pMemberVariableName, ("mPrices")) == 0) {
        mPrices[pNode->getTag()] = (Label *) pNode;
        return true;
    }

    return false;
}

void StoreBScene::onCurrencyBalanceChanged(cocos2d::EventCustom *event) {
    __Dictionary *eventData = (__Dictionary *)event->getUserData();
    updateCurrencyBalance(dynamic_cast<__Integer *>(eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_BALANCE)));
}

void StoreBScene::updateCurrencyBalance(__Integer *pBalance) {
    char buf[20] = "/0";
    sprintf(buf, "%i", pBalance->getValue());
    mMuffinAmount->setString(buf);
}


void StoreBScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
    Layer::onKeyReleased(keyCode, event);
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
        onBack(NULL);
    }
}
