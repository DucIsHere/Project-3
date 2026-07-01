package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder
import net.minecraft.util.StringRepresentable
import com.mbz.world.worldgen.GeneratorContext
import com.mbz.world.worldgen.cell.continent.*
import com.mbz.world.worldgen.cell.continent.advanced.AdvancedContinentGenerator
import com.mbz.world.worldgen.cell.continent.fancy.FancyContinentGenerator
import com.mbz.world.worldgen.cell.continent.infinite.InfiniteContinentGenerator
import com.mbz.world.worldgen.cell.continent.simple.*
import com.mbz.world.worldgen.noise.function.DistanceFunction

data class Continent(
    var continentType: ContinentType = ContinentType.MULTI,
    var continentShape: DistanceFunction = DistanceFunction.EUCLIDEAN,
    var continentScale: Int = 500,
    var continentJitter: Float = 0.45f,
    var continentSkipping: Float = 0.25f,
    var continentSizeVariance: Float = 0.25f,
    var continentNoiseOctaves: Int = 5,
    var continentNoiseGain: Float = 0.26f,
    var continentNoiseLacunarity: Float = 4.33f
) {
    companion object {
        @JvmField
        val CODEC: Codec<Continent> = RecordCodecBuilder.create { i ->
            i.group(
                ContinentType.CODEC.fieldOf("continentType").forGetter { it.continentType },
                DistanceFunction.CODEC.optionalFieldOf("continentShape", DistanceFunction.EUCLIDEAN).forGetter { it.continentShape },
                Codec.INT.fieldOf("continentScale").forGetter { it.continentScale },
                Codec.FLOAT.fieldOf("continentJitter").forGetter { it.continentJitter },
                Codec.FLOAT.optionalFieldOf("continentSkipping", 0.25f).forGetter { it.continentSkipping },
                Codec.FLOAT.optionalFieldOf("continentSizeVariance", 0.25f).forGetter { it.continentSizeVariance },
                Codec.INT.optionalFieldOf("continentNoiseOctaves", 5).forGetter { it.continentNoiseOctaves },
                Codec.FLOAT.optionalFieldOf("continentNoiseGain", 0.26f).forGetter { it.continentNoiseGain },
                Codec.FLOAT.optionalFieldOf("continentNoiseLacunarity", 4.33f).forGetter { it.continentNoiseLacunarity }
            ).apply(i, ::Continent)
        }
    }
}

enum class ContinentType(val id: String) : StringRepresentable {
    SINGLE("SINGLE"),
    MULTI("MULTI"),
    FANCY("FANCY"),
    INFINITE("INFINITE"),
    ADVANCED("ADVANCED");

    override fun getSerializedName() = id

    companion object {
        @JvmField
        val CODEC: Codec<ContinentType> = StringRepresentable.fromEnum { values() }
    }
}
