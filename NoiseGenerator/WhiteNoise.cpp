#include "FastNoise.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

float haltonSequence(int i, int b)
{
	float f = 1.0;
	float res = 0;
	while (i > 0) 
	{
		f = f / b;
		res = res + f * (i % b);
		i = i / b;
	}
	return res;
}

int main()
{
	FastNoise myNoise; // Create a FastNoise object
	myNoise.SetNoiseType(FastNoise::WhiteNoise); // Set the desired noise type


	string fileName = "noise";
	const int dim = 256;
	const double sparseFac = 0.000006;
	const int stepsize = 80;

	stringstream ss;
	ss << fileName << "_" << dim << "_" << stepsize << endl;
	string outputFileName;
	ss >> outputFileName;
	ofstream outs(outputFileName, ios::binary);
	
	for(int i = 0; i < 3; i++)
		outs.write(reinterpret_cast<const char*>(&dim), sizeof(dim));

	long size = dim * dim* dim;

	unsigned char* noiseMap = new unsigned char[size]; // 3D heightmap to create noisemode

	for (int x = 0; x < dim; x++)
	{
		for (int y = 0; y < dim; y++)
		{
			for (int z = 0; z < dim; z++)
			{
				long ind = x + y * dim + z * dim * dim;
				noiseMap[ind] = 0;
			}
		}
	}

	//Halton sequence
	int index = 23;
	int xbase = 17, ybase = 31, zbase = 57;
	int count = 0;
	while (index <= size)
	{
		//int stepsize = size * sparseFac;
		int step = rand() % stepsize + 1;
		int x = dim * haltonSequence(index, xbase);
		int y = dim * haltonSequence(index, ybase);
		int z = dim * haltonSequence(index, zbase);
		index += step;
		float length = sqrt(pow(x - 0.5 * dim, 2) + pow(y - 0.5 * dim, 2) + pow(z - 0.5 * dim, 2));
		//if (length < 0.35 * dim || length > 0.37 * dim)
		//	continue;
		long ind = x + y * dim + z * dim * dim;
		noiseMap[ind] = (unsigned char)(255 * myNoise.GetWhiteNoiseInt(x, y, z));
		count += 1;
	}

	for (int x = 0; x < dim; x++)
	{
		for (int y = 0; y < dim; y++)
		{
			for (int z = 0; z < dim; z++)
			{
				long ind = x + y * dim + z * dim * dim;
				outs.write(reinterpret_cast<char*>(&noiseMap[ind]), sizeof(noiseMap[ind]));
			}
		}
	}
}
