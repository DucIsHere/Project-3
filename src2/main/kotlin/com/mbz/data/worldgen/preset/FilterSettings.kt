package com.mbz.data.worldgen.preset

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

data class FilterSettings(
    var erosion: Erosion,
    var smoothing: Smoothing,
) {
    companion object {
        val CODEC: Codec<FilterSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                Erosion.CODEC.fieldOf("erosion").forGetter { it.erosion},
                Smoothing.CODEC.fieldOf("smoothing").forGetter { it.smoothing}
            ).apply(instance, ::FilterSettings)
        }
    }

    fun deepCopy(): FilterSettings = FilterSettings(erosion.copy(), smoothing.copy())

    data class Ersoion(
        var dropletsPerChunk: Int,
        var dropletLifetime: Int,
        var dropletVolume: Float,
        var dropletVelocity: Float,
        var erosionRate: Float,
        var depositeRate: Float
    ) {
        companion object {
            val CODEC: Codec<Erosion> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.INT.fieldOf("dropletsPerChunk").forGetter { it.dropletsPerChunk },
                    Codec.INT.fieldOf("dropletLifetime").forGetter { it.dropletLifetime },
                    Codec.FLOAT.fieldOf("dropletVolume").forGetter { it.dropletVolume },
                    Codec.FLOAT.fieldOf("dropletVelocity").forGetter { it.dropletVelocity },
                    Codec.FLOAT.fieldOf("erosionRate").forGetter { it.erosionRate },
                    Codec.FLOAT.fieldOf("depositeRate").forGetter { it.depositeRate }
                ).apply(instance, ::Erosion)
            }
        }
    }

    fun deepCopy(): FilterSettings = FilterSettings(dropletsPerChunl.copy(), dropletLifetime.copy(), dropletVolume.copy(), dropletVelocity.copy(), erosionRate.copy(), depositeRate.copy())

    data class Smoothing(
        var Interations: Int,
        var smoothingRadius: Float,
        var smoothingRate: Float
    ) {
        companion object {
            val CODEC: Codec<Smoothing> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.INT.fieldOf("Iterations").forGetter { it.Interations },
                    Codec.FLOAT.fieldOf("smoothingRadius").forGetter { it.smoothingRadius },
                    Codec.FLOAT.fieldOf("smoothingRate").forGetter { it.smoothingRate }
                ).apply(instance, ::Smoothing)
            }
        }
    }
    fun deepCopy(): FilterSettings = FilterSettings(Interations.copy(), smoothingRate.copy(), smoothingRadius.copy())
}