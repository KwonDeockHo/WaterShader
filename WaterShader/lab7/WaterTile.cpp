#include "WaterTile.h"


WaterTile::WaterTile()
{
	this->x = 0;
	this->z = 0;
	this->height = 0;
}

WaterTile::WaterTile(float centerX, float centerZ, float height)
{
	this->x = centerX;
	this->z = centerZ;
	this->height = height;
}

float WaterTile::getHeight(){
	return this->height;
}

float WaterTile::getX(){
	return this->x;
}

float WaterTile::getZ(){
	return this->z;
}