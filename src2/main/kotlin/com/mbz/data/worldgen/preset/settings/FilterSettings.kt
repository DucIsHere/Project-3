package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class FilterSettings {
    var erosion: Erosion
    var smoothing: Smoothing

    constructor(erosion: Erosion, smoothing: Smoothing) {
        this.erosion = erosion
        this.smoothing = smoothing
    }

    fun copy(): FilterSettings {
        return FilterSettings(this.erosion.copy(), this.smoothing.copy())
    }

    // --- Lớp xói mòn (Erosion) ---
    class Erosion {
        var dropletsPerChunk: Int
        var dropletLifetime: Int
        var dropletVolume: Float
        var dropletVelocity: Float
        var erosionRate: Float
        var depositeRate: Float

        constructor(perChunk: Int, lifetime: Int, volume: Float, velocity: Float, eRate: Float, dRate: Float) {
            this.dropletsPerChunk = perChunk
            this.dropletLifetime = lifetime
            this.dropletVolume = volume
            this.dropletVelocity = velocity
            this.erosionRate = eRate
            this.depositeRate = dRate
        }

        fun copy(): Erosion {
            return Erosion(this.dropletsPerChunk, this.dropletLifetime, this.dropletVolume, this.dropletVelocity, this.erosionRate, this.depositeRate)
        }

        companion object {
            @JvmField
            val CODEC: Codec<Erosion> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("dropletsPerChunk").forGetter { it.dropletsPerChunk },
                    Codec.INT.fieldOf("dropletLifetime").forGetter { it.dropletLifetime },
                    Codec.FLOAT.fieldOf("dropletVolume").forGetter { it.dropletVolume },
                    Codec.FLOAT.fieldOf("dropletVelocity").forGetter { it.dropletVelocity },
                    Codec.FLOAT.fieldOf("erosionRate").forGetter { it.erosionRate },
                    Codec.FLOAT.fieldOf("depositeRate").forGetter { it.depositeRate }
                ).apply(i, ::Erosion)
            }
        }
    }

    // --- Lớp làm mượt (Smoothing) ---
    class Smoothing {
        var iterations: Int
        var smoothingRadius: Float
        var smoothingRate: Float

        constructor(iterations: Int, radius: Float, rate: Float) {
            this.iterations = iterations
            this.smoothingRadius = radius
            this.smoothingRate = rate
        }

        fun copy(): Smoothing {
            return Smoothing(this.iterations, this.smoothingRadius, this.smoothingRate)
        }

        companion object {
            @JvmField
            val CODEC: Codec<Smoothing> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("iterations").forGetter { it.iterations },
                    Codec.FLOAT.fieldOf("smoothingRadius").forGetter { it.smoothingRadius },
                    Codec.FLOAT.fieldOf("smoothingRate").forGetter { it.smoothingRate }
                ).apply(i, ::Smoothing)
            }
        }
    }

    companion object {
        @JvmField
        val CODEC: Codec<FilterSettings> = RecordCodecBuilder.create { i ->
            i.group(
                Erosion.CODEC.fieldOf("erosion").forGetter { it.erosion },
                Smoothing.CODEC.fieldOf("smoothing").forGetter { it.smoothing }
            ).apply(i, ::FilterSettings)
        }
    }
}
