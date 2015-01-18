

#ifndef __LevelIconWidget_H_
#define __LevelIconWidget_H_

#include "cocos2d.h"
#include "cocosbuilder/CocosBuilder.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;


class LevelIconWidget: public Node,
                       public CCBSelectorResolver,
                       public CCBMemberVariableAssigner,
                       public NodeLoaderListener {
public:

    CREATE_FUNC(LevelIconWidget);

    LevelIconWidget();

    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref *pTarget, char const *pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(Ref * pTarget, const char* pSelectorName);
    virtual void onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader);

    void setData(char const *itemId, char const *name, char const *description, double price, int balance);
    void setBalance(int balance);
    void setProgress(int progress);
    void setEquiped(bool equiped);
    void updateAfford();

private:
    std::string mItemId;
private:
    Label *mGoodsTitle;
    Label *mDescriptionLabel;
    Sprite *mProgressBar;
    Sprite *mProgressLevel1;
    Node *mMenu;
    MenuItemImage *mButtonBuy;
    MenuItemImage *mButtonUpgrade;
    MenuItemImage *mEquipment;
    Label *mPrice;
    Label *mBalance;

    bool mEquipable;
    bool mUpgradable;
    bool mEquiped;
    int mCurrentBalance;

    //selectors
    void onBuy(Ref *pSender);
    void onUpgrade(Ref *pSender);
    void onEquipment(Ref *pSender);
};


class LevelIconWidgetLoader : public NodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LevelIconWidgetLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LevelIconWidget);
};

#endif //__LevelIconWidget_H_
