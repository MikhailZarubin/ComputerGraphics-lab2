#include "Data.h"
Data::Data()
{
	max = (short)(-1000);
	min = (short)4096;
	height = width = depth = 0;
	size_x = size_y = size_z = 0.f;
	voxel = NULL;
}
short Data::GetMin()const
{
	return min;
}
short Data::GetMax()const
{
	return max;
}
int Data::GetHeight() const
{
	return height;
}
int Data::GetWidth() const
{
	return width;
}
int Data::GetDepth()const
{
	return depth;
}
bool Data::ReadFile(std::string filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		ifs.read((char*)&width, sizeof(int));
		ifs.read((char*)&height, sizeof(int));
		ifs.read((char*)&depth, sizeof(int));
		ifs.read((char*)&size_x, sizeof(float));
		ifs.read((char*)&size_y, sizeof(float));
		ifs.read((char*)&size_z, sizeof(float));
		int size = width * height * depth;
		voxel = std::make_unique<short[]>(size);
		for (int i = 0; i < size; i++)
		{
			ifs.read((char*)&voxel[i], sizeof(short));
			min = std::min(min, voxel[i]);
			max = std::max(max, voxel[i]);
		}
		return true;
	}
	else
		return false;
}
short Data::operator[](std::size_t index) const
{
	return voxel[index];
}