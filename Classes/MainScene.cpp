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

#include "MainScene.h"
#include "StoreAScene.h"

USING_NS_CC;

/**
 * This is the welcome scene.
 * In this file you can find example calls to the store's interfaces.
 **/

#define SEL_GROUP "soomlaCallback"

Scene* MainScene::getMainScene() {
    NodeLoaderLibrary * ccNodeLoaderLibrary = NodeLoaderLibrary::getInstance();

    ccNodeLoaderLibrary->registerNodeLoader("MainScene", MainSceneLoader::loader());

    CCBReader *ccbReader = new CCBReader(ccNodeLoaderLibrary);

    return ccbReader->createSceneWithNodeGraphFromFile("ccb/MainScreen.ccbi");
}

SEL_MenuHandler MainScene::onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName) {
    return NULL;
}

cocos2d::extension::Control::Handler MainScene::onResolveCCBCCControlSelector(Ref *pTarget, char const *pSelectorName) {
    return NULL;
}

bool MainScene::onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBackgroundNode", Node*, mBackgroundNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainNode", Node*, mMainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mUnlockArea", Node*, mUnlockArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mUnlocker", Node*, mUnlocker);
    return false;
}

void MainScene::onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader) {
    CC_ASSERT(mBackgroundNode);
    CC_ASSERT(mMainNode);
    CC_ASSERT(mUnlockArea);
    CC_ASSERT(mUnlocker);

    applyScaleForNode(this);
    fill(mBackgroundNode);

    this->setKeypadEnabled(true);
}

void MainScene::onEnter() {
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        this->mOriginalPos = this->mUnlocker->getPosition();
        return true;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event) {
        Vec2 touchPoint = touch->getLocation();
        this->mUnlocker->setPosition(this->mUnlocker->getParent()->convertToNodeSpace(touchPoint));
    };
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        Rect rect1 = this->mUnlocker->getBoundingBox();
        Rect rect2 = this->mUnlockArea->getBoundingBox();

        if (rect1.intersectsRect(rect2)) {
            Scene *s = StoreAScene::getGoodsStoreScene();
            CCDirector::getInstance()->setDepthTest(true);
            TransitionScene *transition = TransitionMoveInR::create(0.8f, s);

            CCDirector::getInstance()->replaceScene(transition);
        }
        else {
            // Snap
            this->mUnlocker->runAction(CCMoveTo::create(0.2f, this->mOriginalPos));
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    CCLayer::onEnter();
}

void MainScene::onExit() {
    _eventDispatcher->removeEventListener(listener);

    CCLayer::onExit();
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
    Layer::onKeyReleased(keyCode, event);
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
        CCDirector::getInstance()->end();
    }
}
