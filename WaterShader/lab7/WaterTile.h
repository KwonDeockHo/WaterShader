#ifndef _WATER_TILE_
#define _WATER_TILE_

#include <GL/glew.h>
#include "Loader.h"
#include <vector>


class WaterTile
{
private:
	float height;
	float x, z;
public:
	WaterTile();
	WaterTile(float centerX, float centerZ, float height);
	float getHeight();
	float getX();
	float getZ();
};
#endif