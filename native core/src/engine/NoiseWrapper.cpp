#include "NoiseWrapper.h"
#include "SimdSpecs.h"
#include "vendor/src/FastNoise/FastNoise.h"
#include <vector>
#include <thread>

extern "C"
{
    NoiseInstance c_createGenerator(int simdLevel, int seed)
    {
        FastSIMD::FeatureSet featureSet = FastSIMD::FeatureSet::Scalar;
        if (simdLevel == 2)
            featureSet =FastSIMD::FeatureSet::SSE41;
        else if (simdLevel == 3)
            featureSet = FastSIMD::FeatureSet::AVX2;
        else if (simdLevel == 4 || simdLevel == 5)
            featureSet = FastSIMD::FeatureSet::AVX512;
        
        auto node = FastNoise::New<FastNoise::Simplex>(featureSet);
        node->SetSeed(seed);
        
        auto* rawNode = new FastNoise::SmartNode<FastNode::Simplex>(node);
        return (NoiseInstance)rawNode;
    }

    void c_generateNoise(NoiseInstance instance, float* outBuffer, int startX, int startZ, int sizeX, int sizeZ, float frequency, int threads)
    {
        if (!instance)
            return;

        auto* nodePtr = (FastNoise::SmartNode<FastNoise::Simplex>*)instance;
        auto node = *nodePtr;

        std::vector<std::thread> workers;
        int rowsPerThread = sizeZ / threads;

        for (int t = 0; t < thread; ++t) 
        {
            int zStart = t * rowsPerThread;
            int zEnd = (t == threads - 1) ? sizeZ : (t + 1) * rowsPerThread;

            workers.push_back(std::thread([=, &node]() 
            {
                for (int z = zStart; z < zEnd; ++z)
                {
                    for (int x = 0; x < sizeX; ++x)
                    {
                        outBuffer[z * sizeX + x] = node->GenSingle3D((startX + x) * frequency, 0.0f, (startZ + z) * frequency);
                    }
                } 
            }
        ));
        }

        for (auto& worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    }

    void c_destroyGenerator(NoiseInstance instance) {
        if (instance) {
            auto* nodePtr = (FastNoise::SmartNode<FastNoise::Simplex>*)instance;
            delete nodePtr; // Giải phóng bộ nhớ C++
        }
    }
}