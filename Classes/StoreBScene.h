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

#ifndef __STOREB_SCENE_H__
#define __STOREB_SCENE_H__

#include "cocos2d.h"
#include <string>
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;

using namespace std;

#define GOODS_NUMBER 5

class StoreBScene :
    public Layer,
    public CCBSelectorResolver,
    public CCBMemberVariableAssigner,
    public NodeLoaderListener {
private:
    string itemIdFromTag(int tag);
		
public:
    CREATE_FUNC(StoreBScene);
    StoreBScene():
        mBackgroundNode(NULL)
        , mMainNode(NULL)
        , mTopNode(NULL)
        , mBottomNode(NULL)
        , mMuffinAmount(NULL)
    {}

    static cocos2d::Scene*getStoreBScene();

    virtual void onEnter();
    virtual void onExit();

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref *pTarget, char const *pSelectorName) {return NULL;}
    virtual bool onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode);
    virtual void onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader);

    virtual void updateCurrencyBalance(cocos2d::__Integer *balance);
    virtual void onCurrencyBalanceChanged(cocos2d::EventCustom *event);

    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) override;
private:
    Node* mBackgroundNode;
    Node* mMainNode;
    Node *mTopNode;
    Node *mBottomNode;

    Label *mMuffinAmount;

    Label *mGoodTitles[GOODS_NUMBER];
    Label *mGoodDescriptions[GOODS_NUMBER];
    Label *mPrices[GOODS_NUMBER];
        
    cocos2d::EventListener *currencyBalanceChangedListener;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
	void onBack(Ref* pSender);
	void onBuy(Ref* pSender);
};

class StoreBSceneLoader: public LayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StoreBSceneLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StoreBScene);
};

#endif // __MainScene_SCENE_H__
