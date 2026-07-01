package com.mbz.data.worldgen.preset.settings

import com.mbz.world.worldgen.noise.NoiseUtil
import com.mbz.world.worldgen.noise.module.Noise
import com.mbz.world.worldgen.noise.module.Noises
import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder
import net.minecraft.util.StringRepresentable
import java.util.function.BiFunction

class ClimateSettings(
    var temperature: RangeValue,
    var moisture: RangeValue,
    var biomeShape: BiomeShape,
    var biomeEdgeShape: BiomeNoise
) {
    fun copy() = ClimateSettings(temperature.copy(), moisture.copy(), biomeShape.copy(), biomeEdgeShape.copy())

    // --- RangeValue: Cấu hình dải giá trị Nhiệt độ/Độ ẩm ---
    class RangeValue(
        var seedOffset: Int,
        var scale: Int,
        var falloff: Int,
        var min: Float,
        var max: Float,
        var bias: Float
    ) {
        fun getMin(): Float = NoiseUtil.clamp(Math.min(min, max), 0.0f, 1.0f)
        fun getMax(): Float = NoiseUtil.clamp(Math.max(min, max), getMin(), 1.0f)
        fun getBias(): Float = NoiseUtil.clamp(bias, -1.0f, 1.0f)

        fun apply(module: Noise): Noise {
            var m = module
            val b = getBias() / 2.0f
            m = Noises.add(m, b.toDouble()) // Chú ý ép kiểu nếu Noises yêu cầu Double
            m = Noises.clamp(m, getMin().toDouble(), getMax().toDouble())
            return m
        }

        fun copy() = RangeValue(seedOffset, scale, falloff, min, max, bias)

        companion object {
            @JvmField
            val CODEC: Codec<RangeValue> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("seedOffset").forGetter { it.seedOffset },
                    Codec.INT.fieldOf("scale").forGetter { it.scale },
                    Codec.INT.fieldOf("falloff").forGetter { it.falloff },
                    Codec.FLOAT.fieldOf("min").forGetter { it.min },
                    Codec.FLOAT.fieldOf("max").forGetter { it.max },
                    Codec.FLOAT.fieldOf("bias").forGetter { it.bias }
                ).apply(i, ::RangeValue)
            }
        }
    }

    // --- BiomeShape: Cấu hình hình dạng Biome ---
    class BiomeShape(
        var biomeSize: Int,
        var macroNoiseSize: Int,
        var biomeWarpScale: Int,
        var biomeWarpStrength: Int
    ) {
        fun copy() = BiomeShape(biomeSize, macroNoiseSize, biomeWarpScale, biomeWarpStrength)

        companion object {
            @JvmField
            val CODEC: Codec<BiomeShape> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("biomeSize").forGetter { it.biomeSize },
                    Codec.INT.fieldOf("macroNoiseSize").forGetter { it.macroNoiseSize },
                    Codec.INT.fieldOf("biomeWarpScale").forGetter { it.biomeWarpScale },
                    Codec.INT.fieldOf("biomeWarpStrength").forGetter { it.biomeWarpStrength }
                ).apply(i, ::BiomeShape)
            }
        }
    }

    // --- BiomeNoise: Cấu hình nhiễu ở rìa Biome ---
    class BiomeNoise(
        var type: EdgeType,
        var scale: Int,
        var octaves: Int,
        var gain: Float,
        var lacunarity: Float,
        var strength: Int
    ) {
        fun build(seed: Int): Noise {
            return Noises.add(type.factory.apply(seed, this), Noises.constant(-0.5))
        }

        fun copy() = BiomeNoise(type, scale, octaves, gain, lacunarity, strength)

        enum class EdgeType(
            private val serializedName: String,
            val factory: BiFunction<Int, BiomeNoise, Noise>
        ) : StringRepresentable {
            BILLOW("BILLOW", { s, n -> Noises.billow(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            VORONOI("CELL", { s, n -> Noises.worley(s, n.scale) }),
            VORONOI_EDGE("CELL_EDGE", { s, n -> Noises.worleyEdge(s, n.scale) }),
            CONSTANT("CONST", { _, _ -> Noises.one() }),
            CUBIC("CUBIC", { s, n -> Noises.cubic(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            PERLIN("PERLIN", { s, n -> Noises.perlin(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            PERLIN2("PERLIN2", { s, n -> Noises.perlin2(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            PERLIN_RIDGE("RIDGE", { s, n -> Noises.perlinRidge(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            SIMPLEX("SIMPLEX", { s, n -> Noises.simplex(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            SIMPLEX2("SIMPLEX2", { s, n -> Noises.simplex2(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            SIMPLEX_RIDGE("SIMPLEX_RIDGE", { s, n -> Noises.simplex2(s, n.scale, n.octaves, n.lacunarity.toDouble(), n.gain.toDouble()) }),
            SIN("SIN", { s, _ -> Noises.sin(s, 1.0f, Noises.zero()) }),
            WHITE("RAND", { s, _ -> Noises.white(s, 1) });

            override fun getSerializedName() = serializedName

            companion object {
                @JvmField
                val CODEC: Codec<EdgeType> = StringRepresentable.fromEnum { values() }
            }
        }

        companion object {
            @JvmField
            val CODEC: Codec<BiomeNoise> = RecordCodecBuilder.create { i ->
                i.group(
                    EdgeType.CODEC.fieldOf("type").forGetter { it.type },
                    Codec.INT.fieldOf("scale").forGetter { it.scale },
                    Codec.INT.fieldOf("octaves").forGetter { it.octaves },
                    Codec.FLOAT.fieldOf("gain").forGetter { it.gain },
                    Codec.FLOAT.fieldOf("lacunarity").forGetter { it.lacunarity },
                    Codec.INT.fieldOf("strength").forGetter { it.strength }
                ).apply(i, ::BiomeNoise)
            }
        }
    }

    companion object {
        @JvmField
        val CODEC: Codec<ClimateSettings> = RecordCodecBuilder.create { i ->
            i.group(
                RangeValue.CODEC.fieldOf("temperature").forGetter { it.temperature },
                RangeValue.CODEC.fieldOf("moisture").forGetter { it.moisture },
                BiomeShape.CODEC.fieldOf("biomeShape").forGetter { it.biomeShape },
                BiomeNoise.CODEC.fieldOf("biomeEdgeShape").forGetter { it.biomeEdgeShape }
            ).apply(i, ::ClimateSettings)
        }
    }
}
