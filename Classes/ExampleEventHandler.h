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

#ifndef __ExampleEventHandler__
#define __ExampleEventHandler__

#include <string>
#include "Cocos2dxStore.h"

using namespace std;

class ExampleEventHandler {
public:
    
    ExampleEventHandler();

    virtual void onBillingNotSupported(cocos2d::EventCustom *event);

    virtual void onBillingSupported(cocos2d::EventCustom *event);

    virtual void onCurrencyBalanceChanged(cocos2d::EventCustom *event);

    virtual void onGoodBalanceChanged(cocos2d::EventCustom *event);

    virtual void onGoodEquipped(cocos2d::EventCustom *event);

    virtual void onGoodUnEquipped(cocos2d::EventCustom *event);

    virtual void onGoodUpgrade(cocos2d::EventCustom *event);

    virtual void onItemPurchased(cocos2d::EventCustom *event);

    virtual void onItemPurchaseStarted(cocos2d::EventCustom *event);

    virtual void onMarketPurchaseCancelled(cocos2d::EventCustom *event);

    virtual void onMarketPurchase(cocos2d::EventCustom *event);

    virtual void onMarketPurchaseStarted(cocos2d::EventCustom *event);

    virtual void onMarketPurchaseVerification(cocos2d::EventCustom *event);

    virtual void onRestoreTransactionsStarted(cocos2d::EventCustom *event);

    virtual void onRestoreTransactionsFinished(cocos2d::EventCustom *event);

    virtual void onUnexpectedErrorInStore(cocos2d::EventCustom *event);

    virtual void onSoomlaStoreInitialized(cocos2d::EventCustom *event);

    virtual void onMarketItemsRefreshed(cocos2d::EventCustom *event);

    virtual void onMarketItemsRefreshStarted(cocos2d::EventCustom *event);
    
    virtual void onMarketItemsRefreshFailed(cocos2d::EventCustom *event);

    // Android Only
    virtual void onMarketRefund(cocos2d::EventCustom *event);
	virtual void onIabServiceStarted(cocos2d::EventCustom *event);
	virtual void onIabServiceStopped(cocos2d::EventCustom *event);
};

#endif /* !__ExampleEventHandler__ */
