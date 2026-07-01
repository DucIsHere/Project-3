package com.mbz.data.worldgen.preset

data class NoiseSettings(
    val baseFreq: Double,
    val warpStrength: Double,
    val amplifyFactor: Double
) {
    companion object {
        fun defaults(): NoiseSettings = NoiseSettings(0.01, 8.0, 2.8)
    }
}