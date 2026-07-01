package com.mbz.data.worldgen.preset

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuider

data class RiverSettings(
    var seedOffset: Int,
    var riverCount: Int,
    var mainRivers: River,
    var brachRivers: River,
    var lakes: Lake,
    var wetlands: Wetland
) {
    companion object {
        val CODEC: Codec<RiverSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                Codec.INT.fieldOf("seedOffset").forGetter { it.seedOffset },
                Codec.INT.fieldOf("riverCount").forGetter { it.riverCount },
                River.CODEC.fieldOf("mainRivers").forGetter { it.mainRivers },
                River.CODEC.fieldOf("branchRivers").forGetter { it.brachRivers },
                Lake.CODEC.fieldOf("lakes").forGetter { it.lakes },
                Wetland.CODEC.fieldOf("wetlands").forGetter { it.wetlands }
            ).apply(instance, ::RiverSettings)
        }
    }
}

fun deepCopy(): RiverSettings = copy(mainRivers = mainRivers.copy(), branchRivers = branchRivers.copy(), lakes = lakes.copy(), wetlands = wetlands.copy())

data class River(
    var bedDepth: Int,
    var minBankHeight: Int,
    var maxBankHeight: Int,
    var bankWidth: Int,
    var bedWidth: Int,
    var fade: Float,
    var slopeFactor: Float = 0.002f,
    var profileCurve: Float = 0.5f,
    var erosionStrength: Float = 0.01f
) {
    companion object {
        val CODEC: Codec<River> = RecordCodecBuilder.create { instance -> instance.group(
            Codec.INT.fieldOf("bedDepth").forGetter { it.bedDepth },
            Codec.INT.fieldOf("minBankHeight").forGetter { it.minBankHeight },
            Codec.INT.fieldOf("maxBankHeight").forGetter { it.maxBankHeight },
            Codec.INT.fieldOf("bankWidth").forGetter { it.bankWidth },
            Codec.INT.fieldOf("bedWidth").forGetter { it.bedWidth },
            Codec.FLOAT.fieldOf("fade").forGetter { it.fade },
            Codec.FLOAT.fieldOf("slopeFactor", 0.002).forGetter { it.slopeFactor },
            Codec.FLOAT.fieldOf("profileCurve", 0.5).forGetter { it.profileCurve },
            Codec.FLOAT.fieldOf("erosionStrength", 0.1).forGetter { it.erosionStrength }
        ).apply(instance, ::River)
        }
    }
}

data class Lake(
        var chance: Float,
        var minStartDistance: Float,
        var maxStartDistance: Float,
        var depth: Int,
        var sizeMin: Int,
        var sizeMax: Int,
        var minBankHeight: Int,
        var maxBankHeight: Int
    ) {
        companion object {
            val CODEC: Codec<Lake> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.FLOAT.fieldOf("chance").forGetter { it.chance },
                    Codec.FLOAT.fieldOf("minStartDistance").forGetter { it.minStartDistance },
                    Codec.FLOAT.fieldOf("maxStartDistance").forGetter { it.maxStartDistance },
                    Codec.INT.fieldOf("depth").forGetter { it.depth },
                    Codec.INT.fieldOf("sizeMin").forGetter { it.sizeMin },
                    Codec.INT.fieldOf("sizeMax").forGetter { it.sizeMax },
                    Codec.INT.fieldOf("minBankHeight").forGetter { it.minBankHeight },
                    Codec.INT.fieldOf("maxBankHeight").forGetter { it.maxBankHeight }
                ).apply(instance, ::Lake)
            }
        }
    }

    data class Wetland(
        var chance: Float,
        var sizeMin: Int,
        var sizeMax: Int
    ) {
        companion object {
            val CODEC: Codec<Wetland> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.FLOAT.fieldOf("chance").forGetter { it.chance },
                    Codec.INT.fieldOf("sizeMin").forGetter { it.sizeMin },
                    Codec.INT.fieldOf("sizeMax").forGetter { it.sizeMax }
                ).apply(instance, ::Wetland)
            }
        }
    }
