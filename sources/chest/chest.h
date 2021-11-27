//
// Created by Théo Omnès on 21/10/2021.
//

#ifndef MALLOCWORLD_CHESS_H
#define MALLOCWORLD_CHESS_H

#include "../bag/bag.h"

#include <stdbool.h>

typedef struct ChestSlot {
    ItemId id;
    int16_t quantity;
}ChestSlot;

typedef struct Chest {
    ChestSlot slot;
    struct Chest* next;
} Chest;

int addItemsInChest(ItemId itemId, int quantityToAdd);
ChestSlot findItemInChest(ItemId id);
bool updateItemQuantityInChest(ItemId itemId, int8_t newQuantity);
bool insertChestSlotInSaveFile(ChestSlot slotToAdd);
int removeItemsFromChest(ItemId itemId, int quantityToRemove);
ChestSlot newChestSlot(int quantity, ItemId itemId);
void pushSlotInChest(ChestSlot slot, Chest** chest);
void printChest(Chest* chest);
void printChestSlot(ChestSlot slot);

#endif //MALLOCWORLD_CHESS_H