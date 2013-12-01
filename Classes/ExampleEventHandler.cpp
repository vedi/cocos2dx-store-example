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

#include "ExampleEventHandler.h"
#include "Soomla.h"
#include "cocos2d.h"

#define TAG "ExampleEventHandler >>>"

USING_NS_CC;

// TODO:
void ExampleEventHandler::onBillingNotSupported() {
    soomla::CCStoreUtils::logDebug(TAG, "BillingNotSupported");
}

void ExampleEventHandler::onBillingSupported() { // TODO:
    soomla::CCStoreUtils::logDebug(TAG, "BillingSupported");
}

void ExampleEventHandler::onOpeningStore() { // TODO:
    soomla::CCStoreUtils::logDebug(TAG, "OpeningStore");
}

void ExampleEventHandler::onClosingStore() { // TODO:
    soomla::CCStoreUtils::logDebug(TAG, "ClosingStore");
}

// TODO:
void ExampleEventHandler::onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
    soomla::CCStoreUtils::logDebug(TAG, "CurrencyBalanceChanged");
    CCNotificationCenter::sharedNotificationCenter()->postNotification
        (EVENT_ON_CURRENCY_BALANCE_CHANGED, CCInteger::create(balance));
}

// TODO:
void ExampleEventHandler::onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodBalanceChanged");
    CCNotificationCenter::sharedNotificationCenter()->postNotification
        (EVENT_ON_GOOD_BALANCE_CHANGED, CCArray::create(virtualGood, CCInteger::create(balance), NULL));
}

// TODO:
void ExampleEventHandler::onGoodEquipped(soomla::CCEquippableVG *equippableVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodEquipped");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_EQUIPPED, equippableVG);
}

// TODO:
void ExampleEventHandler::onGoodUnEquipped(soomla::CCEquippableVG *equippableVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodUnEquipped");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UNEQUIPPED, equippableVG);
}

// TODO:
void ExampleEventHandler::onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodUpgrade");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UPGRADE, virtualGood);
}

// TODO:
void ExampleEventHandler::onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "ItemPurchased");
}

// TODO:
void ExampleEventHandler::onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "ItemPurchaseStarted");
}

// TODO:
void ExampleEventHandler::onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseCancelled");
}

// TODO:
void ExampleEventHandler::onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchase");
}

// TODO:
void ExampleEventHandler::onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseStarted");
}

void ExampleEventHandler::onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketRefund");
}

// TODO:
void ExampleEventHandler::onRestoreTransactions(bool success) {
    soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactions");
}

// TODO:
void ExampleEventHandler::onRestoreTransactionsStarted() {
    soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsStarted");
}

// TODO:
void ExampleEventHandler::onUnexpectedErrorInStore() {
    soomla::CCStoreUtils::logDebug(TAG, "UnexpectedErrorInStore");
}

void ExampleEventHandler::onStoreControllerInitialized() { // TODO:
    soomla::CCStoreUtils::logDebug(TAG, "StoreContorllerInitialized");
}
