//
// Filename: map.h
//
// Made by Théo Omnès on 09 oct. 2021.
//
// Description:
//

#ifndef MALLOCWORLD_MAP_H
#define MALLOCWORLD_MAP_H

#include "../zone/zone.h"
#include <stdint.h>
#include "../resources_reappearance/resources_reappearance.h"

typedef struct Map{
    int8_t numberOfZones;
    Zone** zones;
    ToRespawn* toRespawn;
}Map;



Map* newMap(int8_t numberOfZones, Zone** zones);
void printMap(Map map);
void freeMap(Map* map);
int8_t findNumberOfZones();
Map* createMap();
Zone* getZoneById(Map* map, int8_t zoneId);
void setCellValueInMapAtLocation(CellValue cell, Map* map, Location location);
Location* getPlayerLocationInMap(Map* map);

#endif //MALLOCWORLD_MAP_H