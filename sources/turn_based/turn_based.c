//
// Created by Théo Omnès on 23/10/2021.
//

#include "turn_based.h"

bool newGame(Player* player, Map* map) {
    int32_t turn = 0;
    bool play = true;
    while(play) {
        printf("\nTURN %d\n", turn);
        printPlayer(*player);
        displayItemInPlayerHand(player);
        displayZone(*getZoneById(map, player->location->zoneId));
        updatePlayerPossibleActions(player, map);
        displayPlayerPossibleActions(player);
        Direction nextDirection = getPlayerDirection();
        if(nextDirection == -1) { //quit the game
            play = false;
            break;
        }
        if(nextDirection == 4) {
            displayBag(*player->bag);
            playerSelectsItsHand(player);
            continue;
        }
        bool playerActionIsValid = nextDirection >= 0 && nextDirection < 4 && player->actions[nextDirection] != NULL;
        if(playerActionIsValid) {
            (*player->actions[nextDirection])(player, map, nextDirection);
            if(!playerIsAlive(*player)){
                break;
            }
        }
        turn += 1;
        updateMapWithToRespawnList(map, *player->location);
    }
    return play;
}

Direction getPlayerDirection() {
    return getPlayerDirectionByCli();
}

void displayZone(Zone zone){
    displayZoneCli(zone);
}

/**
 * Update the pointers of functions corresponding to the actions in the four directions (Left, Right, Up, Down)
 * by checking if the action is possible: the player can move in this direction
 * or the player fulfill the conditions to make the action.
 * If the action is not possible, the value is NULL.
 * ex:
 * 0 0 0
 * 0 1 2
 * 0 -1 0
 * actions[Left] : &moveLeft
 * actions[Right] : &talkToNpc
 * actions[Up] : &moveUp
 * actions[Down] : NULL , wall so impossible to move
 * @param player
 * @param map
 */
void updatePlayerPossibleActions(Player* player, Map* map) {
    player->actions[Left] = getPlayerPossibleActionByGridValueAndDirection(player, map, Left);
    player->actions[Right] = getPlayerPossibleActionByGridValueAndDirection(player, map, Right);
    player->actions[Up] = getPlayerPossibleActionByGridValueAndDirection(player, map, Up);
    player->actions[Down] = getPlayerPossibleActionByGridValueAndDirection(player, map, Down);
}

/**
 * For each value of the enum CellValue, get the function pointer of the action
 * corresponding to the grid value
 * (ex: Ground -> &moveUp or &moveDown or &moveLeft or &moveRight)
 * If the action is NOT possible (the player can't move in this direction,
 * or the player does not meet the conditions to make the action) the value is NULL.
 * (ex: PlantZoneThree -> but doest have the tool to collect)
 * @return Function pointer of the action in the wanted direction
 */
void* getPlayerPossibleActionByGridValueAndDirection(Player* player, Map* map, Direction direction) {
    CellValue value = getCellValueInDirection(player, map, direction);
    switch (value) {
        case PortalTwoThree: return getChangeZoneAction(value);
        case PortalOneTwo: return getChangeZoneAction(value);
        case Ground: return getWalkAction(direction);
        case NPC: return &talkToNPC;
        default: break;
    }
    if(value >= PlantZoneOne && value <= WoodZoneThree) {
        return &collectResource;
    }
    if(isMonster(value)) {
        return getFightAction(player, value);
    }

    return NULL;
}

/**
 * Update the list of resources to respawn, make the resources respawn if
 * there is no remaining turns before respawn.
 *  Then remove the respawned resources from the To Respawn list.
 *  If a resource is at the same location than the player's then we do not set the map cell
 *  or remove the resource from the To Respawn list.
 */
void updateMapWithToRespawnList(Map* map, Location playerLocation){
    updateToRespawnList(map->toRespawn);
    insertCellsToRespawnInMap(map, playerLocation);
    removeRespawnedCellsFromToRespawnList(&map->toRespawn, playerLocation);
}

/**
 * For each resource in the To Respawn list, check if the resource has to respawn.
 * If yes then set the resource in map at the corresponding location.
 * If the player is at this location, don't set the cell, the remaining turn of the resource
 * will decrement and at the next turn the condition 'cellHasToRespawn' will be true
 */
void insertCellsToRespawnInMap(Map* map, Location playerLocation) {
    ToRespawn* head = map->toRespawn;
    while(head != NULL) {
        bool cellHasNoRemainingTurns = head->remainingTurns <= 0;
        bool playerIsNotAtRespawnLocation = !locationsAreEquals(head->location, playerLocation);
        bool cellHasToRespawn = cellHasNoRemainingTurns && playerIsNotAtRespawnLocation;
        if(cellHasToRespawn) {
            setCellValueInMapAtLocation(head->cell, map, head->location);
        }
        head = head->next;
    }
}

void displayPlayerPossibleActions(Player* player) {
    int numberOfOptions = 6;
    char* bullets[numberOfOptions];
    for(int i = 0; i < numberOfOptions; i += 1) {
        bullets[i] = malloc(sizeof(char) * FILE_LINE_LENGTH);
    }
    strcpy(bullets[Left], "q");
    strcpy(bullets[Right], "d");
    strcpy(bullets[Up], "z");
    strcpy(bullets[Down], "s");
    strcpy(bullets[4], "e");
    strcpy(bullets[5], "x");
    char* options[numberOfOptions];
    for(int i = 0; i < numberOfOptions; i += 1) {
        options[i] = malloc(sizeof(char) * FILE_LINE_LENGTH);
    }
    strcpy(options[Left], "Gauche");
    strcpy(options[Right], "Droite");
    strcpy(options[Up], "Haut");
    strcpy(options[Down], "Bas");
    strcpy(options[4], "Sac à dos");
    strcpy(options[5], "Quitter");
    displayMenuWithCustomBullet("Vous pouvez faire :", NULL, numberOfOptions, bullets, options);
    for(int i = 0; i < numberOfOptions; i += 1) {
        free(bullets[i]);
        free(options[i]);
    }
}