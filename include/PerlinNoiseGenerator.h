#ifndef PERLIN_NOISE_GENERATOR_H
#define PERLIN_NOISE_GENERATOR_H

class PerlinNoiseGenerator
{
public:

	static void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);

};

#endif // !PERLIN_NOISE_GENERATOR_H