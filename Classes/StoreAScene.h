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

class StoreAScene : public Layer,
                        public CCBSelectorResolver,
                        public CCBMemberVariableAssigner,
                        public NodeLoaderListener,
                        public TableViewDataSource,
                        public TableViewDelegate
{
public:
    static Scene* getGoodsStoreScene();

    CREATE_FUNC(StoreAScene);
    virtual ~StoreAScene();

protected:
    StoreAScene();

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref *pTarget, char const *pSelectorName) {return NULL;};
    virtual bool onAssignCCBMemberVariable(Ref *pTarget, char const *pMemberVariableName, Node *pNode);
    virtual void onNodeLoaded(Node *pNode, NodeLoader *pNodeLoader);

    virtual void onEnter();
    virtual void onExit();

private:
    virtual void onCurrencyBalanceChanged(cocos2d::EventCustom *event);
    virtual void updateCurrencyBalance(cocos2d::__Integer *balance);
    virtual void updateGoodBalance(cocos2d::EventCustom *event);
    virtual void onGoodEquipped(cocos2d::EventCustom *event);
    virtual void onGoodUnEquipped(cocos2d::EventCustom *event);
    virtual void onGoodUpgrade(cocos2d::EventCustom *event);

    //ccb
    Node *mBackgroundNode;
    LayerColor *mBackground;
    Node *mMainNode;
    Node *mTopNode;
    Label *mMuffinAmount;
    TableView *mGoodsTableView;
    TableViewCell *mListRows[NUMBER_OF_ROWS];
    LevelIconWidget *mListItem[NUMBER_OF_ROWS];
    Node *mBottomNode;
    MenuItemImage *mButtonBack;
    MenuItemImage *mButtonMoreMuffins;

    //selectors
    void onBack(Ref *pSender);
    void onMoreMuffins(Ref *pSender);

    //class vars
    LevelIconWidget *mCacheListItems[NUMBER_OF_ROWS];

    std::basic_string<char> itemIdFromTag(int tag);

    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    
    cocos2d::EventListener *currencyBalanceChangedHandler;
    cocos2d::EventListener *goodBalanceChangedHandler;
    cocos2d::EventListener *goodEquippedHandler;
    cocos2d::EventListener *goodUnequippedHandler;
    cocos2d::EventListener *goodUpgradeHandler;
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
    virtual TableView * createNode(cocos2d::Node * pParent, CCBReader * pCCBReader) {
        TableView *pRet = new TableView();
        pRet->initWithViewSize(Size::ZERO, NULL);
        pRet->autorelease();
        pRet->setDataSource(NULL);
        return pRet;
    }
};

class CCTableViewCellLoader : public NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCTableViewCellLoader, loader);
protected:
    virtual TableViewCell * createNode(cocos2d::Node * pParent, CCBReader * pCCBReader) {
        TableViewCell *pRet = new TableViewCell();
        pRet->autorelease();
        return pRet;
    }
};

//Utils methods
float applyScaleForNode(cocos2d::Node* node);

void fill(cocos2d::Node* targetNode);

void shiftToLeftBottom(cocos2d::Node* targetNode);

void shiftToTop(cocos2d::Node* targetNode);

void shiftToBottom(cocos2d::Node* targetNode);

void fillWidth(cocos2d::Node* targetNode);

void putToCenterMiddleOf(cocos2d::Node* targetNode, cocos2d::Node* anchorNode);

#endif //__StoreAScene_H_
