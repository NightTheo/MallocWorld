//
// Created by Théo Omnès on 27/10/2021.
//

#include "test_collect_resouces.h"

Map* MAP;
Character* PLAYER;

void testCollectResources() {
    //Rock
    testCollectRockZoneOne();
    testCollectRockZoneTwo();
    testCollectRockZoneThree();

    //Wood
}
/*
 * 0   0   0
 * 0   1   0
 * 0   0   0
 */
void setUpCollectResources() {
    Zone** zones = malloc(sizeof(Zone*));
    zones[0] = newZone(1, 3, 3, Ground, 0);
    MAP = newMap(1, zones);
    PLAYER = newCharacter(100, 1,  10, newLocation(1,1,1), newBag(5, 10));
}

void afterCollectResources() {
    freeMap(MAP);
    freeCharacter(PLAYER);
}

/*
 * .   .   .
 * .   P   rockZ1
 * .   .   .
 */
void testCollectRockZoneOne() {
    printf("Test Collect RockZoneOne");
    setUpCollectResources();
    Item woodPickaxe = newTool(WoodPickaxe, "Pioche en bois");
    PLAYER->bag->slots[0]->item = woodPickaxe;
    setZoneValueAtPosition(MAP->zones[0], 2, 1, RockZoneOne);
    int p = 0;

    collectResource(PLAYER, MAP, Right);

    p += assertEqualsInt(Stone, PLAYER->bag->slots[1]->item.id);
    p += assertBetweenInt(1, 4, PLAYER->bag->slots[1]->quantity);
    p += assertEqualsInt(9, PLAYER->bag->slots[0]->item.durability); // woodPickaxe
    p += assertEqualsInt(Ground, getZoneValueAtPosition(*(MAP->zones[0]), 2, 1));

    printResultTest(p, 4);
    afterCollectResources();
}

void testCollectRockZoneTwo() {
    printf("Test Collect RockZoneTwo");
    setUpCollectResources();
    Item stonePickaxe = newTool(StonePickaxe, "Pioche en pierre");
    PLAYER->bag->slots[0]->item = stonePickaxe;
    setZoneValueAtPosition(MAP->zones[0], 2, 1, RockZoneTwo);
    int p = 0;

    collectResource(PLAYER, MAP, Right);

    p += assertEqualsInt(Iron, PLAYER->bag->slots[1]->item.id);
    p += assertBetweenInt(1, 4, PLAYER->bag->slots[1]->quantity);
    p += assertEqualsInt(8, PLAYER->bag->slots[0]->item.durability); // stonePickaxe
    p += assertEqualsInt(Ground, getZoneValueAtPosition(*(MAP->zones[0]), 2, 1));

    printResultTest(p, 4);
    afterCollectResources();
}

void testCollectRockZoneThree() {
    printf("Test Collect RockZoneThree");
    setUpCollectResources();
    Item ironPickaxe = newTool(IronPickaxe, "Pioche en fer");
    PLAYER->bag->slots[0]->item = ironPickaxe;
    setZoneValueAtPosition(MAP->zones[0], 2, 1, RockZoneThree);
    int p = 0;

    collectResource(PLAYER, MAP, Right);

    p += assertEqualsInt(Diamond, PLAYER->bag->slots[1]->item.id);
    p += assertBetweenInt(1, 4, PLAYER->bag->slots[1]->quantity);
    p += assertEqualsInt(6, PLAYER->bag->slots[0]->item.durability); // ironPickaxe
    p += assertEqualsInt(Ground, getZoneValueAtPosition(*(MAP->zones[0]), 2, 1));

    printResultTest(p, 4);
    afterCollectResources();
}