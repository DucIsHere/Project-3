package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class CaveSettings(
    var entranceCaveProbability: Float,
    var cheeseCaveDepthOffset: Float,
    var cheeseCaveProbability: Float,
    var spaghettiCaveProbability: Float,
    var noodleCaveProbability: Float,
    var caveCarverProbability: Float,
    var deepCaveCarverProbability: Float,
    var ravineCarverProbability: Float,
    var largeOreVeins: Boolean,
    var legacyCarverDistribution: Boolean
) {
    // Thuộc tính TODO từ file Java của mày
    var minCaveBiomeDepth: Boolean = false

    fun copy(): CaveSettings {
        return CaveSettings(
            entranceCaveProbability,
            cheeseCaveDepthOffset,
            cheeseCaveProbability,
            spaghettiCaveProbability,
            noodleCaveProbability,
            caveCarverProbability,
            deepCaveCarverProbability,
            ravineCarverProbability,
            largeOreVeins,
            legacyCarverDistribution
        ).also { it.minCaveBiomeDepth = this.minCaveBiomeDepth }
    }

    companion object {
        @JvmField
        val CODEC: Codec<CaveSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                Codec.FLOAT.fieldOf("entranceCaveProbability").forGetter { it.entranceCaveProbability },
                Codec.FLOAT.fieldOf("cheeseCaveDepthOffset").forGetter { it.cheeseCaveDepthOffset },
                Codec.FLOAT.fieldOf("cheeseCaveProbability").forGetter { it.cheeseCaveProbability },
                Codec.FLOAT.fieldOf("spaghettiCaveProbability").forGetter { it.spaghettiCaveProbability },
                Codec.FLOAT.fieldOf("noodleCaveProbability").forGetter { it.noodleCaveProbability },
                Codec.FLOAT.fieldOf("caveCarverProbability").forGetter { it.caveCarverProbability },
                Codec.FLOAT.fieldOf("deepCaveCarverProbability").forGetter { it.deepCaveCarverProbability },
                Codec.FLOAT.fieldOf("ravineCarverProbability").forGetter { it.ravineCarverProbability },
                Codec.BOOL.fieldOf("largeOreVeins").forGetter { it.largeOreVeins },
                Codec.BOOL.fieldOf("legacyCarverDistribution").forGetter { it.legacyCarverDistribution }
            ).apply(instance, ::CaveSettings)
        }
    }
}
