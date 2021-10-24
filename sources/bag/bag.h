//
// Filename: bag.h
//
// Made by Théo Omnès on 10 oct. 2021.
//
// Description:
//

#ifndef MALLOCWORLD_BAG_H
#define MALLOCWORLD_BAG_H

#endif //MALLOCWORLD_BAG_H

#ifndef MALLOCWORLD_ITEM_H
#include "../item/item.h"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef MALLOCWORLD_CONFIG_H
#include "../config/config.h"
#endif


/*
 * A Bag slot is a case which can contain
 * one type of item with different quantities ( >= 1 )
 * or be empty
 */
typedef struct BagSlot {
    Item* item;
    int8_t quantity;
    int8_t capacity;
} BagSlot;

/*
 * The Bag is the inventory of the character,
 * it's an array of BagSlot of the size bagCapacity
 */
typedef struct Bag {
    int8_t capacity;
    BagSlot** slots;
}Bag;

BagSlot* newBagSlot(Item* item, int8_t quantity, int8_t capacity);
void printSlot(BagSlot slot);
void freeBagSlot(BagSlot* bagSlot);
Bag* newBag(int8_t bagCapacity, int8_t slotsCapacity);
void printBag(Bag bag);
void freeBag(Bag* bag);
int8_t findBagCapacity();
BagSlot* getBagSlotAtIndex(Bag* bag, int index);
void setBagSlotAtIndex(Bag* bag, int index, BagSlot* slot);
bool addItemInBag(Bag* bag, Item* itemToAdd);
BagSlot* searchFirstEmptySlotInBag(Bag* bag);
int8_t findBagSlotCapacity();
BagSlot* searchFirstAvailableSlotByItemtypeInBag(Bag* bag, ItemType searched);
bool* searchSlotsByItemId(Bag* bag, ItemId itemId);
int16_t countNumberOfItemsInBagByItemId(Bag* bag, ItemId itemId);