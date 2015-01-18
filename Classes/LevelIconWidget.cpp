

#include "LevelIconWidget.h"
#include "CCStoreInventory.h"
#include "CCSoomlaUtils.h"


LevelIconWidget::LevelIconWidget() {
    mGoodsTitle = NULL;
    mDescriptionLabel = NULL;
    mProgressBar = NULL;
    mProgressLevel1 = NULL;
    mMenu = NULL;
    mButtonBuy = NULL;
    mButtonUpgrade = NULL;
    mEquipment = NULL;
    mPrice = NULL;
    mBalance = NULL;

}

void LevelIconWidget::onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader) {
    CC_UNUSED_PARAM(pNode);
    CC_UNUSED_PARAM(pNodeLoader);

    CC_ASSERT(mGoodsTitle);
    CC_ASSERT(mDescriptionLabel);
    CC_ASSERT(mProgressBar);
    CC_ASSERT(mProgressLevel1);
    CC_ASSERT(mMenu);
    CC_ASSERT(mButtonBuy);
    CC_ASSERT(mButtonUpgrade);
    CC_ASSERT(mEquipment);
    CC_ASSERT(mPrice);
    CC_ASSERT(mBalance);

    mEquipable = mEquipment->isVisible();
    mUpgradable = mButtonUpgrade->isVisible();
}

void LevelIconWidget::setData(char const *itemId, char const *name, char const *description, double price, int balance) {
    mItemId = itemId;
    mGoodsTitle->setString(name);
    mDescriptionLabel->setString(description);
    mPrice->setString(CCString::createWithFormat("%.2f", price)->getCString());
    setBalance(balance);
}

void LevelIconWidget::setBalance(int balance) {
    mCurrentBalance = balance;
    mBalance->setString(CCString::createWithFormat("%d", balance)->getCString());
    if (mEquipable) {
        mButtonBuy->setVisible(balance == 0);
        mEquipment->setVisible(balance != 0);
    }
}

void LevelIconWidget::setProgress(int progress) {
    CCAssert(progress >= 0 && progress <= 5, "Sorry UI does not support such progress");
    mProgressLevel1->setScaleX(progress);
}

void LevelIconWidget::setEquiped(bool equiped) {
    this->mEquiped = equiped;
    mEquipment->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
            equiped ? "equipped_O.png" : "equip_O.png"));
}

void LevelIconWidget::updateAfford() {
    if (mItemId.empty()) {
        return;
    }
    
    bool canAfford = soomla::CCStoreInventory::sharedStoreInventory()->canAfford(mItemId.c_str());
    
    if (mUpgradable) {
        if (mButtonUpgrade != NULL) {
            mButtonUpgrade->setVisible(canAfford);
        }
    }
    else if (mEquipable) {
        if (!mEquiped && (mCurrentBalance == 0)) {
            if (mButtonBuy != NULL) {
                mButtonBuy->setVisible(canAfford);
            }
        }
    }
    else {
        if (mButtonBuy != NULL) {
            mButtonBuy->setVisible(canAfford);
        }
    }
}

bool LevelIconWidget::onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoodsTitle", Label*, mGoodsTitle)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDescriptionLabel", Label*, mDescriptionLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mProgressBar", Sprite*, mProgressBar)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mProgressLevel1", Sprite*, mProgressLevel1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", Node*, mMenu)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonBuy", MenuItemImage*, mButtonBuy)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mButtonUpgrade", MenuItemImage*, mButtonUpgrade)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEquipment", MenuItemImage*, mEquipment)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPrice", Label*, mPrice)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBalance", Label*, mBalance)

    return false;
}

cocos2d::extension::Control::Handler LevelIconWidget::onResolveCCBCCControlSelector(Ref *pTarget, char const *pSelectorName) {
    CC_UNUSED_PARAM(pTarget);
    CC_UNUSED_PARAM(pSelectorName);

    return NULL;
}

SEL_MenuHandler LevelIconWidget::onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName) {
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBuy", LevelIconWidget::onBuy)
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onUpgrade", LevelIconWidget::onUpgrade)
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onEquipment", LevelIconWidget::onEquipment)

    return NULL;
}

void LevelIconWidget::onBuy(Ref *pSender) {

    CC_UNUSED_PARAM(pSender);

    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem(mItemId.c_str(), &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("LevelIconWidget::onBuy", soomlaError);
    }
}

void LevelIconWidget::onUpgrade(Ref *pSender) {

    CC_UNUSED_PARAM(pSender);

    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->upgradeGood(mItemId.c_str(), &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("LevelIconWidget::onUpgrade", soomlaError);
    }
}

void LevelIconWidget::onEquipment(Ref *pSender) {

    CC_UNUSED_PARAM(pSender);

    soomla::CCError *soomlaError = NULL;
    if (!this->mEquiped) {
        soomla::CCStoreInventory::sharedStoreInventory()->equipVirtualGood(mItemId.c_str(), &soomlaError);
    } else {
        soomla::CCStoreInventory::sharedStoreInventory()->unEquipVirtualGood(mItemId.c_str(), &soomlaError);
    }
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("LevelIconWidget::onEquipment", soomlaError);
    }
}
