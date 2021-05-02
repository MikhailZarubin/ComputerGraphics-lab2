#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<memory>

class Data
{
	short min, max;
	int height, width, depth;
	float size_x, size_y, size_z;
	std::unique_ptr<short[]> voxel;
public:
	Data();
	short GetMin() const;
	short GetMax() const;
	int GetHeight() const;
	int GetWidth() const;
	int GetDepth() const;
	short operator[](std::size_t index) const;
	bool ReadFile(std::string filename);
};


