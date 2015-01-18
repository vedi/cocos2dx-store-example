

#ifndef __MuffinRushAssets_H_
#define __MuffinRushAssets_H_

#include "CCStoreAssets.h"
#include "cocos2d.h"

class MuffinRushAssets: public soomla::CCStoreAssets {
    cocos2d::__Array *mCurrencies;
    cocos2d::__Array *mGoods;
    cocos2d::__Array *mCurrencyPacks;
    cocos2d::__Array *mCategories;
public:
    MuffinRushAssets():
    mCurrencies(NULL),
    mGoods(NULL),
    mCurrencyPacks(NULL),
    mCategories(NULL)
    {}

    virtual ~MuffinRushAssets();

    static MuffinRushAssets *create();
    bool init();

    virtual int getVersion();

    virtual cocos2d::__Array *getCurrencies();

    virtual cocos2d::__Array *getGoods();

    virtual cocos2d::__Array *getCurrencyPacks();

    virtual cocos2d::__Array *getCategories();
};


#endif //__MuffinRushAssets_H_
