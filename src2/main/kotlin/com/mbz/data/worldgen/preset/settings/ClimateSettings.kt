package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder
import net.minecraft.util.StringRepresentable
import com.mbz.world.worldgen.noise.NoiseUtil
import com.mbz.world.worldgen.noise.module.Noise
import com.mbz.world.worldgen.noise.module.Noises
import java.util.function.BiFunction

data class ClimateSettings(
    var temperature: RangeValue,
    var moisture: RangeValue,
    var biomeShape: BiomeShape,
    var biomeEdgeShape: BiomeNoise
) {
    companion object {
        @JvmField
        val CODEC: Codec<ClimateSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                RangeValue.CODEC.fieldOf("temperature").forGetter { it.temperature },
                RangeValue.CODEC.fieldOf("moisture").forGetter { it.moisture },
                BiomeShape.CODEC.fieldOf("biomeShape").forGetter { it.biomeShape },
                BiomeNoise.CODEC.fieldOf("biomeEdgeShape").forGetter { it.biomeEdgeShape }
            ).apply(instance, ::ClimateSettings)
        }
    }

    data class RangeValue(
        var seedOffset: Int, var scale: Int, var falloff: Int,
        var min: Float, var max: Float, var bias: Float
    ) {
        companion object {
            @JvmField
            val CODEC: Codec<RangeValue> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.INT.fieldOf("seedOffset").forGetter { it.seedOffset },
                    Codec.INT.fieldOf("scale").forGetter { it.scale },
                    Codec.INT.fieldOf("falloff").forGetter { it.falloff },
                    Codec.FLOAT.fieldOf("min").forGetter { it.min },
                    Codec.FLOAT.fieldOf("max").forGetter { it.max },
                    Codec.FLOAT.fieldOf("bias").forGetter { it.bias }
                ).apply(instance, ::RangeValue)
            }
        }

        fun getMin() = NoiseUtil.clamp(minOf(min, max), 0f, 1f)
        fun getMax() = NoiseUtil.clamp(maxOf(min, max), getMin(), 1f)
        fun getBias() = NoiseUtil.clamp(bias, -1f, 1f)

        fun apply(module: Noise): Noise {
            var m = Noises.add(module, getBias() / 2f)
            return Noises.clamp(m, getMin(), getMax())
        }
    }

    data class BiomeShape(
        var biomeSize: Int, var macroNoiseSize: Int,
        var biomeWarpScale: Int, var biomeWarpStrength: Int
    ) {
        companion object {
            @JvmField
            val CODEC: Codec<BiomeShape> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.INT.fieldOf("biomeSize").forGetter { it.biomeSize },
                    Codec.INT.fieldOf("macroNoiseSize").forGetter { it.macroNoiseSize },
                    Codec.INT.fieldOf("biomeWarpScale").forGetter { it.biomeWarpScale },
                    Codec.INT.fieldOf("biomeWarpStrength").forGetter { it.biomeWarpStrength }
                ).apply(instance, ::BiomeShape)
            }
        }
    }

    data class BiomeNoise(
        var type: EdgeType, var scale: Int, var octaves: Int,
        var gain: Float, var lacunarity: Float, var strength: Int
    ) {
        companion object {
            @JvmField
            val CODEC: Codec<BiomeNoise> = RecordCodecBuilder.create { instance ->
                instance.group(
                    EdgeType.CODEC.fieldOf("type").forGetter { it.type },
                    Codec.INT.fieldOf("scale").forGetter { it.scale },
                    Codec.INT.fieldOf("octaves").forGetter { it.octaves },
                    Codec.FLOAT.fieldOf("gain").forGetter { it.gain },
                    Codec.FLOAT.fieldOf("lacunarity").forGetter { it.lacunarity },
                    Codec.INT.fieldOf("strength").forGetter { it.strength }
                ).apply(instance, ::BiomeNoise)
            }
        }

        fun build(seed: Int): Noise = Noises.add(type.build(seed, this), Noises.constant(-0.5f))

        enum class EdgeType(private val id: String, private val factory: BiFunction<Int, BiomeNoise, Noise>) : StringRepresentable {
            BILLOW("BILLOW", { s, n -> Noises.billow(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            VORONOI("CELL", { s, n -> Noises.worley(s, n.scale) }),
            VORONOI_EDGE("CELL_EDGE", { s, n -> Noises.worleyEdge(s, n.scale) }),
            CONSTANT("CONST", { _, _ -> Noises.one() }),
            CUBIC("CUBIC", { s, n -> Noises.cubic(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            PERLIN("PERLIN", { s, n -> Noises.perlin(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            PERLIN2("PERLIN2", { s, n -> Noises.perlin2(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            PERLIN_RIDGE("RIDGE", { s, n -> Noises.perlinRidge(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            SIMPLEX("SIMPLEX", { s, n -> Noises.simplex(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            SIMPLEX2("SIMPLEX2", { s, n -> Noises.simplex2(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            SIMPLEX_RIDGE("SIMPLEX_RIDGE", { s, n -> Noises.simplex2(s, n.scale, n.octaves, n.lacunarity, n.gain) }),
            SIN("SIN", { s, _ -> Noises.sin(s, 1f, Noises.zero()) }),
            WHITE("RAND", { s, _ -> Noises.white(s, 1) });

            companion object {
                @JvmField val CODEC: Codec<EdgeType> = StringRepresentable.fromEnum { entries.toTypedArray() }
            }
            override fun getSerializedName() = id
            fun build(seed: Int, settings: BiomeNoise): Noise = factory.apply(seed, settings)
        }
    }
}
