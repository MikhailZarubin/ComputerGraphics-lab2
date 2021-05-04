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
	std::unique_ptr<short[]>voxel;
public:
	Data();
	short GetMin() const;
	short GetMax() const;
	int GetHeight() const;
	int GetWidth() const;
	int GetDepth() const;
	void SetMin(short value);
	void SetMax(short value);
	short operator[](int index) const;
	void ReadFile(std::string filename);
};


