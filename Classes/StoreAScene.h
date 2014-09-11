//
// Created by Alexandr on 31.05.13.
//



#ifndef __StoreAScene_H_
#define __StoreAScene_H_

#include "LevelIconWidget.h"

#include "cocos2d.h"
#include "Cocos2dxStore.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;

#define NUMBER_OF_ROWS 14

class StoreAScene : public CCLayer,
                        public CCBSelectorResolver,
                        public CCBMemberVariableAssigner,
                        public NodeLoaderListener,
                        public TableViewDataSource,
                        public TableViewDelegate
{
public:
    static CCScene* getGoodsStoreScene();

    CREATE_FUNC(StoreAScene);
    virtual ~StoreAScene();

protected:
    StoreAScene();

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *pTarget, char const *pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(CCObject *pTarget, char const *pSelectorName) {return NULL;};
    virtual bool onAssignCCBMemberVariable(CCObject *pTarget, char const *pMemberVariableName, CCNode *pNode);
    virtual void onNodeLoaded(CCNode *pNode, NodeLoader *pNodeLoader);

    virtual void onEnter();
    virtual void onExit();

private:
    virtual void updateCurrencyBalance(Ref *pBalance);
    virtual void updateGoodBalance(Ref *pParams);
    virtual void onGoodEquipped(Ref *virtualGood);
    virtual void onGoodUnEquipped(Ref *virtualGood);
    virtual void onGoodUpgrade(Ref *virtualGood);

    //ccb
    CCNode *mBackgroundNode;
    CCLayerColor *mBackground;
    CCNode *mMainNode;
    CCNode *mTopNode;
    Label *mMuffinAmount;
    TableView *mGoodsTableView;
    TableViewCell *mListRows[NUMBER_OF_ROWS];
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

    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual CCSize tableCellSizeForIndex(TableView *table, ssize_t idx) override;
public:
    virtual void tableCellTouched(TableView *table, TableViewCell *cell) {}
    virtual void scrollViewDidScroll(ScrollView *view) {};
    virtual void scrollViewDidZoom(ScrollView *view) {};

    void setProgressForItem(std::string &itemId, LevelIconWidget *pWidget);
    void setEquippedForItem(std::string &itemId, LevelIconWidget *pWidget);
};

class StoreASceneLoader : public LayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StoreASceneLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StoreAScene);
};

class CCTableViewLoader : public ScrollViewLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCTableViewLoader, loader);
protected:
    virtual TableView * createNode(cocos2d::CCNode * pParent, CCBReader * pCCBReader) {
        TableView *pRet = new TableView();
        pRet->initWithViewSize(CCSizeZero, NULL);
        pRet->autorelease();
        pRet->setDataSource(NULL);
        return pRet;
    }
};

class CCTableViewCellLoader : public NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCTableViewCellLoader, loader);
protected:
    virtual TableViewCell * createNode(cocos2d::CCNode * pParent, CCBReader * pCCBReader) {
        TableViewCell *pRet = new TableViewCell();
        pRet->autorelease();
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
