//
// Created by Théo Omnès on 27/10/2021.
//

#include "collect_resources.h"

const CollectResourceInfo COLLECT_RESOURCES_INFORMATION[NUMBER_OF_COLLECTABLE_RESOURCES] = {
        {RockZoneOne, Stone, 1, 4, 0.1, WoodPickaxe},
        {RockZoneTwo, Iron, 1, 4, 0.2, StonePickaxe},
        {RockZoneThree, Diamond, 1, 4, 0.4, IronPickaxe},
        {WoodZoneOne, FirTree, 1, 4, 0.1, WoodAxe},
        {WoodZoneTwo, BeechTree, 1, 4, 0.2, StoneAxe},
        {WoodZoneThree, OakTree, 1, 4, 0.4, IronAxe},
        {PlantZoneOne, Grass, 1, 4, 0.1, WoodBillhook},
        {PlantZoneTwo, Lavender, 1, 4, 0.2, StoneBillhook},
        {PlantZoneThree, Hemp, 1, 4, 0.4, IronBillhook}
};

/**
 * Open the collect resources file in read mode
 * The collect resource file contains the information of the collect
 * (cf struct CollectResourceInfo)
 * @return the FILE* of the collect resources file in read mode
 */
FILE* openCollectResourcesFile() {
    char path[PATH_MAX];
    char* projectDirectoryAbsolutePath = getProjectDirectory();
    sprintf(path, "%s%s%s", projectDirectoryAbsolutePath, PATH_SEPARATOR, "resources/collect_resources.txt");
    free(projectDirectoryAbsolutePath);
    return fopen(path, "r");
}

/**
 * Loop in the collect resources file and
 * get the grid cell collect information
 * (cf. struct CollectResourceInfo)
 * @param resourceToCollect GridValue
 * @return Struct CollectResourceInfo or a collect info of a GridValueError if not found
 */
CollectResourceInfo getCollectInfoByGridValue(CellValue resourceToCollect) {
    for(int i = 0; i < NUMBER_OF_COLLECTABLE_RESOURCES; i += 1) {
        if(COLLECT_RESOURCES_INFORMATION[i].gridResource == resourceToCollect) {
            return COLLECT_RESOURCES_INFORMATION[i];
        }
    }
    CollectResourceInfo notFound = {GridValueError, 0, 0 , 0, 0, 0};
    return notFound;
}

/**
 * Convert a line of the collect resources file to a struct CollectResourceInfo
 * @param line String to convert
 * @return The converted string into a CollectResourceInfo
 */
CollectResourceInfo convertCollectInfoLineToStruct(char* line) {
    CollectResourceInfo collectInfo;
    sscanf(line, "%d;%d;%d;%d;%lf;%d",
    &collectInfo.gridResource,
    &collectInfo.collectedResource,
    &collectInfo.minQuantityCollected,
    &collectInfo.maxQuantityCollected,
    &collectInfo.collectUsury,
    &collectInfo.minTool
    );

    return collectInfo;
};

/**
 * Display on stdout the information of the cell value collect
 */
void printCollectResourceInfo(CollectResourceInfo collectInfo) {
    printf(ANSI_COLOR_YELLOW);
    printf("\n-- Collect Resources Info --");
    printf("\nGridValue to collect: %d", collectInfo.gridResource);
    printf("\nItem collected: %d", collectInfo.collectedResource);
    printf("\nCollect [%d,%d] items randomly", collectInfo.minQuantityCollected, collectInfo.maxQuantityCollected);
    printf("\nUsury of the collect: %.2lf", collectInfo.collectUsury);
    printf("\nMinimum tool to collect: %d", collectInfo.minTool);
    printf("\n");
    printf(ANSI_COLOR_RESET);
}

/**
 * Check if a player is able to collect a resource:
 * if the tool in his hand is able to collect
 * @return True if the player is able to collect the resource, false if not
 */
bool isPlayerAbleToCollectResource(Player* player, CellValue resource) {
    CollectResourceInfo collectInfo = getCollectInfoByGridValue(resource);
    int8_t playerHand = player->bag->currentSlot;
    Item currentTool = player->bag->slots[playerHand]->item;
    return isToolAbleToCollectResource(currentTool, collectInfo);
}

/**
 * Check if a tool is able to collect a resource:
 *  - if the tool is of the good family (Pickaxe, Axe, Billhook)
 *  - if the material is hard enough (diamond is harder than wood)
 *  - if the tool has enough durability to collect
 */
bool isToolAbleToCollectResource(Item item, CollectResourceInfo collectInfo) {
    if(NULL == item.object) {
        return false;
    }
    Tool* tool = (Tool*)item.object;
    Tool minTool = getToolByItemId(collectInfo.minTool);
    double usury = collectInfo.collectUsury * item.maxDurability;
    int newDurability = (int)(item.durability - usury);
    bool isToolGoodFamily = tool->family == minTool.family;
    bool isMaterialHardEnough = tool->material >= minTool.material;
    bool isToolHasEnoughDurability = newDurability >= 0;
    if(!isToolGoodFamily) {
        printf("\nBad Famility tool: required = %d, given = %d", minTool.family, tool->family);
    }
    if(!isMaterialHardEnough) {
        printf("\nBad material: min required = %d, given = %d", minTool.material, tool->material);
    }
    if(!isToolHasEnoughDurability) {
        printf("\nNot enough durability: min required = %d, given = %d", (int)usury, tool->durability);
    }
    return isToolGoodFamily && isMaterialHardEnough && isToolHasEnoughDurability ;
}

/**
 * if the player is able to collect the resource,
 * generate a random number of item collected (in the specific range of the resource),
 * add the items in the bag and replace the cell value of the resource by Ground
 * @param direction Direction in which the resource is located
 */
void collectResource(Player* player, Map* map, Direction direction) {
    CellValue toCollect = getGridValueToCollect(player, map, direction);
    CollectResourceInfo info = getCollectInfoByGridValue(toCollect);
    if(!isPlayerAbleToCollectResource(player, toCollect)) {
        displayPlayerCannotCollectResource();
        return;
    }
    int numberOfItemCollected = randomIntInRange(info.minQuantityCollected, info.maxQuantityCollected);
    Item collected = getCraftResourceItemById(info.collectedResource);
    addItemsInBag(player->bag, collected, numberOfItemCollected);
    applyCollectUsuryOnTool(getCurrentBagSlot(player->bag), info.collectUsury);
    removeCellAfterCollect(player, map, direction);

    addResourceToRespawnList(toCollect, &map->toRespawn, getLocationInDirection(*player->location, direction));
}

/**
 * @return The CellValue in a direction from the location of the player
 */
CellValue getGridValueToCollect(Player* player, Map* map, Direction direction) {
    Location locationToCollect = getLocationInDirection(*player->location, direction);
    return getZoneValueAtPosition(*getZoneById(map, player->location->zoneId), locationToCollect.x, locationToCollect.y);
}

/**
 * Replace the CellValue in a direction from the location of the player by Ground
 */
void removeCellAfterCollect(Player* player, Map* map, Direction direction) {
    Location locationToCollect = getLocationInDirection(*player->location, direction);
    setZoneValueAtPosition(getZoneById(map, player->location->zoneId), locationToCollect.x, locationToCollect.y, Ground);
}

/**
 * Remove durability of the tool depending the usury of the resource to collect
 * @param toolSlot Slot which contains the tool to use
 * @param usury Double 0 <= usury <= 1, meaning the percentage of the max durability removed
 * @return The removed durability of the tool
 */
int applyCollectUsuryOnTool(BagSlot* toolSlot, double usury) {
    Item tool = toolSlot->item;
    int durabilityRemoved = (int)(usury * tool.maxDurability);
    toolSlot->item.durability = tool.durability - durabilityRemoved;
    return durabilityRemoved;
}


void displayPlayerCannotCollectResource() {
    printMessageType("\nTu n'as pas l'outil adéquat\n", Neutral);
}