//
// Created by Alexandr on 31.05.13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __StoreAScene_H_
#define __StoreAScene_H_

#include "LevelIconWidget.h"

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCListView/CCListView.h"
#include "CCListView/CCListViewCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define NUMBER_OF_ROWS 14

class StoreAScene : public CCLayer,
                        public CCBSelectorResolver,
                        public CCBMemberVariableAssigner,
                        public CCNodeLoaderListener,
                        public CCListViewDelegate {
public:
    static CCScene* getGoodsStoreScene();

    CREATE_FUNC(StoreAScene);
    virtual ~StoreAScene();

protected:
    StoreAScene();

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *pTarget, char const *pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject *pTarget, char const *pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject *pTarget, char const *pMemberVariableName, CCNode *pNode);
    virtual void onNodeLoaded(CCNode *pNode, CCNodeLoader *pNodeLoader);

    virtual void onEnter();
    virtual void onExit();

private:
    virtual void CCListView_numberOfCells(CCListView *listView, CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(CCListView *listView, CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(CCListView *listView, CCListViewProtrolData *data) {};
    virtual void CCListView_didScrollToRow(CCListView *listView, CCListViewProtrolData *data) {};

    virtual void updateCurrencyBalance(CCInteger *pBalance);
    virtual void updateGoodBalance(CCArray *pParams);

    //ccb
    CCNode *mBackgroundNode;
    CCLayerColor *mBackground;
    CCNode *mMainNode;
    CCNode *mTopNode;
    CCLabelTTF *mMuffinAmount;
    CCListView *mGoodsListView;
    CCListViewCell *mListRows[NUMBER_OF_ROWS];
    LevelIconWidget *mListItem[NUMBER_OF_ROWS];
    CCNode *mBottomNode;
    CCMenuItemImage *mButtonBack;
    CCMenuItemImage *mButtonMoreMuffins;

    //selectors
    void onBack(CCObject *pSender);
    void onMoreMuffins(CCObject *pSender);

    //class vars
    LevelIconWidget *mCacheListItems[NUMBER_OF_ROWS];

    std::basic_string<char> itemIdFromTag(int tag);

    int tagFromItemId(char const *itemId);
};

class StoreASceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StoreASceneLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StoreAScene);
};

class CCListViewLoader : public CCLayerColorLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCListViewLoader, loader);
protected:
    virtual CCListView * createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader) {
        CCListView *pRet = CCListView::create(CCListViewModeVertical);
        return pRet;
    }
};

class CCListViewCellLoader : public CCLayerColorLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCListViewCellLoader, loader);
protected:
    virtual CCListViewCell * createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader) {
        CCListViewCell *pRet = CCListViewCell::node();
        return pRet;
    }
};

//Utils methods
float applyScaleForNode(cocos2d::CCNode* node);

void fill(cocos2d::CCNode* targetNode);

void shiftToLeftBottom(cocos2d::CCNode* targetNode);

void shiftToTop(cocos2d::CCNode* targetNode);

void shiftToBottom(cocos2d::CCNode* targetNode);

void fillWidth(cocos2d::CCNode* targetNode);

void putToCenterMiddleOf(cocos2d::CCNode* targetNode, cocos2d::CCNode* anchorNode);

#endif //__StoreAScene_H_
