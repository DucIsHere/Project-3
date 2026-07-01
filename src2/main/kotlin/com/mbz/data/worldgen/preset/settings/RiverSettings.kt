package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class RiverSettings {
    var seedOffset: Int
    var riverCount: Int
    var mainRivers: River
    var branchRivers: River
    var lakes: Lake
    var wetlands: Wetland

    constructor(seedOffset: Int, riverCount: Int, mainRivers: River, branchRivers: River, lakes: Lake, wetlands: Wetland) {
        this.seedOffset = seedOffset
        this.riverCount = riverCount
        this.mainRivers = mainRivers
        this.branchRivers = branchRivers
        this.lakes = lakes
        this.wetlands = wetlands
    }

    fun copy(): RiverSettings {
        return RiverSettings(
            this.seedOffset,
            this.riverCount,
            this.mainRivers.copy(),
            this.branchRivers.copy(),
            this.lakes.copy(),
            this.wetlands.copy()
        )
    }

    // --- Cấu hình Sông (River) ---
    class River {
        var bedDepth: Int
        var minBankHeight: Int
        var maxBankHeight: Int
        var bankWidth: Int
        var bedWidth: Int
        var fade: Float
        var slopeFactor: Float
        var profileCurve: Float
        var erosionStrength: Float

        constructor(
            bedDepth: Int, minBankHeight: Int, maxBankHeight: Int, bankWidth: Int, bedWidth: Int, fade: Float,
            slopeFactor: Float = 0.002f, profileCurve: Float = 0.5f, erosionStrength: Float = 0.01f
        ) {
            this.bedDepth = bedDepth
            this.minBankHeight = minBankHeight
            this.maxBankHeight = maxBankHeight
            this.bankWidth = bankWidth
            this.bedWidth = bedWidth
            this.fade = fade
            this.slopeFactor = slopeFactor
            this.profileCurve = profileCurve
            this.erosionStrength = erosionStrength
        }

        fun copy(): River {
            return River(bedDepth, minBankHeight, maxBankHeight, bankWidth, bedWidth, fade, slopeFactor, profileCurve, erosionStrength)
        }

        companion object {
            @JvmField
            val CODEC: Codec<River> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("bedDepth").forGetter { it.bedDepth },
                    Codec.INT.fieldOf("minBankHeight").forGetter { it.minBankHeight },
                    Codec.INT.fieldOf("maxBankHeight").forGetter { it.maxBankHeight },
                    Codec.INT.fieldOf("bankWidth").forGetter { it.bankWidth },
                    Codec.INT.fieldOf("bedWidth").forGetter { it.bedWidth },
                    Codec.FLOAT.fieldOf("fade").forGetter { it.fade },
                    Codec.FLOAT.optionalFieldOf("slopeFactor", 0.002f).forGetter { it.slopeFactor },
                    Codec.FLOAT.optionalFieldOf("profileCurve", 0.5f).forGetter { it.profileCurve },
                    Codec.FLOAT.optionalFieldOf("erosionStrength", 0.01f).forGetter { it.erosionStrength }
                ).apply(i, ::River)
            }
        }
    }

    // --- Cấu hình Hồ (Lake) ---
    class Lake {
        var chance: Float
        var minStartDistance: Float
        var maxStartDistance: Float
        var depth: Int
        var sizeMin: Int
        var sizeMax: Int
        var minBankHeight: Int
        var maxBankHeight: Int

        constructor(chance: Float, minStartDist: Float, maxStartDist: Float, depth: Int, sizeMin: Int, sizeMax: Int, minBank: Int, maxBank: Int) {
            this.chance = chance
            this.minStartDistance = minStartDist
            this.maxStartDistance = maxStartDist
            this.depth = depth
            this.sizeMin = sizeMin
            this.sizeMax = sizeMax
            this.minBankHeight = minBank
            this.maxBankHeight = maxBank
        }

        fun copy(): Lake {
            return Lake(chance, minStartDistance, maxStartDistance, depth, sizeMin, sizeMax, minBankHeight, maxBankHeight)
        }

        companion object {
            @JvmField
            val CODEC: Codec<Lake> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.FLOAT.fieldOf("chance").forGetter { it.chance },
                    Codec.FLOAT.fieldOf("minStartDistance").forGetter { it.minStartDistance },
                    Codec.FLOAT.fieldOf("maxStartDistance").forGetter { it.maxStartDistance },
                    Codec.INT.fieldOf("depth").forGetter { it.depth },
                    Codec.INT.fieldOf("sizeMin").forGetter { it.sizeMin },
                    Codec.INT.fieldOf("sizeMax").forGetter { it.sizeMax },
                    Codec.INT.fieldOf("minBankHeight").forGetter { it.minBankHeight },
                    Codec.INT.fieldOf("maxBankHeight").forGetter { it.maxBankHeight }
                ).apply(i, ::Lake)
            }
        }
    }

    // --- Cấu hình Đầm lầy (Wetland) ---
    class Wetland {
        var chance: Float
        var sizeMin: Int
        var sizeMax: Int

        constructor(chance: Float, sizeMin: Int, sizeMax: Int) {
            this.chance = chance
            this.sizeMin = sizeMin
            this.sizeMax = sizeMax
        }

        fun copy(): Wetland {
            return Wetland(chance, sizeMin, sizeMax)
        }

        companion object {
            @JvmField
            val CODEC: Codec<Wetland> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.FLOAT.fieldOf("chance").forGetter { it.chance },
                    Codec.INT.fieldOf("sizeMin").forGetter { it.sizeMin },
                    Codec.INT.fieldOf("sizeMax").forGetter { it.sizeMax }
                ).apply(i, ::Wetland)
            }
        }
    }

    companion object {
        @JvmField
        val CODEC: Codec<RiverSettings> = RecordCodecBuilder.create { i ->
            i.group(
                Codec.INT.fieldOf("seedOffset").forGetter { it.seedOffset },
                Codec.INT.fieldOf("riverCount").forGetter { it.riverCount },
                River.CODEC.fieldOf("mainRivers").forGetter { it.mainRivers },
                River.CODEC.fieldOf("branchRivers").forGetter { it.branchRivers },
                Lake.CODEC.fieldOf("lakes").forGetter { it.lakes },
                Wetland.CODEC.fieldOf("wetlands").forGetter { it.wetlands }
            ).apply(i, ::RiverSettings)
        }
    }
}
