//
// Filename: test_movement.c
//
// Made by Théo Omnès on 16 oct. 2021.
//
// Description:
//

#include "test_movement.h"

Map* MAP;
Character* PLAYER;

/**
 * create 3 zones of 5x5 with the player in the center of the zone 1 and the portals
 *  -- ZONE 1 --          -- ZONE 2 --          -- ZONE 3 --
 *  0   0   0   0   0  |  0   0   0   0   0  |  0   0   0   0   0
 *  0   0   0   0   0  |  0   0   0   0   0  |  0   0   0   0   0
 *  0   0   1   0   0  |  0   0   0   0   0  |  0   0   0   0   0
 *  0   0   0   0  -2  | -2   0   0   0  -3  | -3   0   0   0   0
 *  0   0   0   0   0  |  0   0   0   0   0  |  0   0   0   0   0
 */
void setUp(const char* testName, Location* playerLocation) {
    printf("\n%s", testName);
    Zone** zones = malloc(sizeof(Zone*) * 3);
    for(int i = 0; i < 3; i += 1) {
        zones[i] = newZone(i + 1, 5, 5, 0, -1);
    }
    zones[0]->grid[3][4] = PortalOneTwo;
    zones[1]->grid[3][0] = PortalOneTwo;
    zones[1]->grid[3][4] = PortalTwoThree;
    zones[2]->grid[3][0] = PortalTwoThree;

    PLAYER = newCharacter(0, 1, 100, playerLocation);
    int x = PLAYER->location->x;
    int y = PLAYER->location->y;
    int zone = PLAYER->location->zoneId - 1;
    zones[zone]->grid[y][x] = Player;

    MAP = newMap(3, zones);
}

void after() {
    freeMap(MAP);
    freeCharacter(PLAYER);
}

void testMovement() {
    testMoveUp();
    testMoveUpEdges();
    testMoveRight();
    testMoveRightEdges();
    testChangeZoneOneToTwo();
    testChangeZoneTwoToOne();
    testChangeZoneThreeToTwo();
    testPlayerTakePortalOneToTwo();
    testPlayerTakePortalTwoToOne();
    testPlayerTakePortalTwoToThree();
    testPlayerTakePortalOneToTwoButHisLevelIsTooLow();
}

void testMoveUp() {
    setUp("Test Move Up", newLocation(2,2,1));

    int p = 0;
    bool hasMoved = moveUp(PLAYER, MAP);

    p += assertEqualsBool(true, hasMoved);
    p += assertEqualsInt(Ground, MAP->zones[0]->grid[2][2]); // player is gone
    p += assertEqualsInt(Player, MAP->zones[0]->grid[1][2]); // player moved up

    printResultTest(p, 3);
    after();
}

void testMoveUpEdges() {
    setUp("Test Move Up On The Map Edge", newLocation(2,0,1));

    int p = 0;
    bool hasMoved = moveUp(PLAYER, MAP);

    p += assertEqualsBool(false, hasMoved);
    p += assertEqualsInt(Player, MAP->zones[0]->grid[0][2]); // player didn't move

    printResultTest(p, 2);
    after();
}

void testMoveRight() {
    setUp("Test Move Right", newLocation(2,2,1));

    int p = 0;
    bool hasMoved = moveRight(PLAYER, MAP);

    p += assertEqualsBool(true, hasMoved);
    p += assertEqualsInt(Ground, MAP->zones[0]->grid[2][2]); // now ground, player moved
    p += assertEqualsInt(Player, MAP->zones[0]->grid[2][3]); // player now right

    printResultTest(p, 3);
    after();
}

void testMoveRightEdges() {
    setUp("Test Move Right On The Map Edge", newLocation(4,2,1));

    int p = 0;
    bool hasMoved = moveRight(PLAYER, MAP);

    p += assertEqualsBool(false, hasMoved);
    p += assertEqualsInt(Player, MAP->zones[0]->grid[2][4]); // player didn't move

    printResultTest(p, 2);
    after();
}

void testChangeZoneOneToTwo() {
    setUp("Test Change Zone One To Two", newLocation(0,0,1));

    int p = 0;
    bool hasChanged = playerChangeZone(PLAYER->location, getZoneById(MAP, 2));

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(2, PLAYER->location->zoneId); // changed zone
    p += assertEqualsPoint(0, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testChangeZoneTwoToOne() {
    setUp("Test Change Zone Two To One", newLocation(0,0,2));

    int p = 0;
    bool hasChanged = playerChangeZone(PLAYER->location, getZoneById(MAP, 1));

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(1, PLAYER->location->zoneId); // changed zone
    p += assertEqualsPoint(4, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testChangeZoneThreeToTwo() {
    setUp("Test Change Zone Three To Two", newLocation(0,0,3));

    int p = 0;
    bool hasChanged = playerChangeZone(PLAYER->location, getZoneById(MAP, 2));

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(2, PLAYER->location->zoneId); // changed zone
    p += assertEqualsPoint(4, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testPlayerTakePortalOneToTwo() {
    setUp("test Player Take Portal One To Two", newLocation(2,2,1));

    int p = 0;
    bool hasChanged = playerTakesPortal(PLAYER, MAP, PortalOneTwo);

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(2, PLAYER->location->zoneId);
    p += assertEqualsPoint(0, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testPlayerTakePortalTwoToOne() {
    setUp("test Player Take Portal Two To One", newLocation(2,2,2));

    int p = 0;
    bool hasChanged = playerTakesPortal(PLAYER, MAP, PortalOneTwo);

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(1, PLAYER->location->zoneId);
    p += assertEqualsPoint(4, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testPlayerTakePortalTwoToThree() {
    setUp("test Player Take Portal Two To Three", newLocation(2,2,2));

    int p = 0;
    bool hasChanged = playerTakesPortal(PLAYER, MAP, PortalTwoThree);

    p += assertEqualsBool(true, hasChanged);
    p += assertEqualsInt(3, PLAYER->location->zoneId);
    p += assertEqualsPoint(0, 3, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}

void testPlayerTakePortalOneToTwoButHisLevelIsTooLow() {
    setUp("test Player Take Portal One To Two But His Level Is Too Low",
          newLocation(2,2,1));
    PLAYER->level = 1;
    MAP->zones[1]->minLevel = 3;

    int p = 0;
    bool hasChanged = playerTakesPortal(PLAYER, MAP, PortalOneTwo);

    p += assertEqualsBool(false, hasChanged);
    p += assertEqualsInt(1, PLAYER->location->zoneId);
    p += assertEqualsPoint(2, 2, PLAYER->location->x, PLAYER->location->y);

    printResultTest(p, 3);
    after();
}