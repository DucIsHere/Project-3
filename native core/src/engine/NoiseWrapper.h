#ifndef NOISE_WRAPPER_H
#define NOISE_WARRPER_H

typedef void* NoiseInstance;

NoiseInstance c_createGenerator(int simdLevel, int seed);
void c_generateNoise(NoiseInstance instance, float* outBuffer, int StartX, int StartZ, int SizeX, int SizeZ, float frequency, int threads);
void c_destroyGenerator(NoiseInstance instance);

#endif