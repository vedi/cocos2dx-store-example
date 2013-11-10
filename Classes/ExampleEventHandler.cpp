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
#include "CCStoreUtils.h"
#include "cocos2d.h"

#define TAG "ExampleEventHandler"

USING_NS_CC;

void EventHandler::onBillingNotSupported() {
    soomla::CCStoreUtils::logDebug(TAG, "BillingNotSupported");
}

void EventHandler::onBillingSupported() {
    soomla::CCStoreUtils::logDebug(TAG, "BillingSupported");
}

void EventHandler::onOpeningStore() {
    soomla::CCStoreUtils::logDebug(TAG, "OpeningStore");
}

void EventHandler::onClosingStore() {
    soomla::CCStoreUtils::logDebug(TAG, "ClosingStore");
}

void EventHandler::onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
    soomla::CCStoreUtils::logDebug(TAG, "CurrencyBalanceChanged");
    CCNotificationCenter::sharedNotificationCenter()->postNotification
        (EVENT_ON_CURRENCY_BALANCE_CHANGED, CCInteger::create(balance));
}

void EventHandler::onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodBalanceChanged");
    CCNotificationCenter::sharedNotificationCenter()->postNotification
        (EVENT_ON_GOOD_BALANCE_CHANGED, CCArray::create(virtualGood, CCInteger::create(balance), NULL));
}

void EventHandler::onGoodEquipped(soomla::CCEquippableVG *equippableVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodEquipped");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_EQUIPPED, equippableVG);
}

void EventHandler::onGoodUnEquipped(soomla::CCEquippableVG *equippableVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodUnEquipped");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UNEQUIPPED, equippableVG);
}

void EventHandler::onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG) {
    soomla::CCStoreUtils::logDebug(TAG, "GoodUpgrade");
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ON_GOOD_UPGRADE, virtualGood);
}

void EventHandler::onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "ItemPurchased");
}

void EventHandler::onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "ItemPurchaseStarted");
}

void EventHandler::onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseCancelled");
}

void EventHandler::onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchase");
}


void EventHandler::onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketPurchaseStarted");
}

void EventHandler::onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem) {
    soomla::CCStoreUtils::logDebug(TAG, "MarketRefund");
}

void EventHandler::onRestoreTransactions(bool success) {
    soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactions");
}

void EventHandler::onRestoreTransactionsStarted() {
    soomla::CCStoreUtils::logDebug(TAG, "RestoreTransactionsStarted");
}

void EventHandler::onUnexpectedErrorInStore() {
    soomla::CCStoreUtils::logDebug(TAG, "UnexpectedErrorInStore");
}

void EventHandler::onStoreControllerInitialized() {
    soomla::CCStoreUtils::logDebug(TAG, "StoreContorllerInitialized");
}
