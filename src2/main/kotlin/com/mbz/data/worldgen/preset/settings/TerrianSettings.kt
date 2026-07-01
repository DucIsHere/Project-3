package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class TerrainSettings {
    var general: General
    var steppe: Terrain
    var plains: Terrain
    var hills: Terrain
    var dales: Terrain
    var plateau: Terrain
    var badlands: Terrain
    var torridonian: Terrain
    var mountains: Terrain
    var volcano: Terrain
    var blendLow: Terrain
    var blendMid: Terrain
    var blendHigh: Terrain
    var mountainRanges: MountainRanges

    constructor(
        general: General, steppe: Terrain, plains: Terrain, hills: Terrain, dales: Terrain,
        plateau: Terrain, badlands: Terrain, torridonian: Terrain, mountains: Terrain, volcano: Terrain,
        blendLow: Terrain, blendMid: Terrain, blendHigh: Terrain, mountainRanges: MountainRanges
    ) {
        this.general = general
        this.steppe = steppe
        this.plains = plains
        this.hills = hills
        this.dales = dales
        this.plateau = plateau
        this.badlands = badlands
        this.torridonian = torridonian
        this.mountains = mountains
        this.volcano = volcano
        this.blendLow = blendLow
        this.blendMid = blendMid
        this.blendHigh = blendHigh
        this.mountainRanges = mountainRanges
    }

    fun copy(): TerrainSettings {
        return TerrainSettings(
            general.copy(), steppe.copy(), plains.copy(), hills.copy(), dales.copy(),
            plateau.copy(), badlands.copy(), torridonian.copy(), mountains.copy(), volcano.copy(),
            blendLow.copy(), blendMid.copy(), blendHigh.copy(), mountainRanges.copy()
        )
    }

    // --- Cấu hình chung địa hình ---
    class General {
        var terrainSeedOffset: Int
        var terrainRegionSize: Int
        var globalVerticalScale: Float
        var globalHorizontalScale: Float
        var fancyMountains: Boolean
        var slopedTerrain: Boolean

        constructor(offset: Int, size: Int, vScale: Float, hScale: Float, fancy: Boolean, sloped: Boolean) {
            this.terrainSeedOffset = offset
            this.terrainRegionSize = size
            this.globalVerticalScale = vScale
            this.globalHorizontalScale = hScale
            this.fancyMountains = fancy
            this.slopedTerrain = sloped
        }

        fun copy() = General(terrainSeedOffset, terrainRegionSize, globalVerticalScale, globalHorizontalScale, fancyMountains, slopedTerrain)

        companion object {
            @JvmField
            val CODEC: Codec<General> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("terrainSeedOffset").forGetter { it.terrainSeedOffset },
                    Codec.INT.fieldOf("terrainRegionSize").forGetter { it.terrainRegionSize },
                    Codec.FLOAT.fieldOf("globalVerticalScale").forGetter { it.globalVerticalScale },
                    Codec.FLOAT.fieldOf("globalHorizontalScale").forGetter { it.globalHorizontalScale },
                    Codec.BOOL.fieldOf("fancyMountains").forGetter { it.fancyMountains },
                    Codec.BOOL.fieldOf("slopedTerrain").forGetter { it.slopedTerrain }
                ).apply(i, ::General)
            }
        }
    }

    // --- Cấu hình từng loại địa hình (Steppe, Plains...) ---
    class Terrain {
        var weight: Float
        var baseScale: Float
        var verticalScale: Float
        var horizontalScale: Float
        var baseHeight: Float
        var slopeScale: Float
        var coastSharpness: Float
        var valleyErosion: Float
        var valleyWeirdness: Float
        var riverErosion: Float
        var riverWeirdness: Float
        var lakeWeirdness: Float
        var beachNoiseScale: Float
        var beachHeight: Float
        var valleyDepth: Float
        var valleyWidth: Float
        var mountainSharpness: Float
        var mountainHeightScale: Float
        var mountainScale: Float
        var plateauHeight: Float
        var aquiferDepthOffset: Int

        constructor(
            weight: Float, baseScale: Float, verticalScale: Float, horizontalScale: Float, baseHeight: Float,
            slopeScale: Float, coastSharpness: Float, valleyErosion: Float, valleyWeirdness: Float,
            riverErosion: Float, riverWeirdness: Float, lakeWeirdness: Float, beachNoiseScale: Float, beachHeight: Float,
            valleyDepth: Float, valleyWidth: Float, mountainSharpness: Float, mountainHeightScale: Float,
            mountainScale: Float, plateauHeight: Float, aquiferDepthOffset: Int
        ) {
            this.weight = weight
            this.baseScale = baseScale
            this.verticalScale = verticalScale
            this.horizontalScale = horizontalScale
            this.baseHeight = baseHeight
            this.slopeScale = slopeScale
            this.coastSharpness = coastSharpness
            this.valleyErosion = valleyErosion
            this.valleyWeirdness = valleyWeirdness
            this.riverErosion = riverErosion
            this.riverWeirdness = riverWeirdness
            this.lakeWeirdness = lakeWeirdness
            this.beachNoiseScale = beachNoiseScale
            this.beachHeight = beachHeight
            this.valleyDepth = valleyDepth
            this.valleyWidth = valleyWidth
            this.mountainSharpness = mountainSharpness
            this.mountainHeightScale = mountainHeightScale
            this.mountainScale = mountainScale
            this.plateauHeight = plateauHeight
            this.aquiferDepthOffset = aquiferDepthOffset
        }

        fun copy() = Terrain(
            weight, baseScale, verticalScale, horizontalScale, baseHeight, slopeScale, coastSharpness,
            valleyErosion, valleyWeirdness, riverErosion, riverWeirdness, lakeWeirdness, beachNoiseScale, beachHeight,
            valleyDepth, valleyWidth, mountainSharpness, mountainHeightScale, mountainScale, plateauHeight, aquiferDepthOffset
        )

        companion object {
            @JvmField
            val CODEC: Codec<Terrain> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.FLOAT.fieldOf("weight").forGetter { it.weight },
                    Codec.FLOAT.fieldOf("baseScale").forGetter { it.baseScale },
                    Codec.FLOAT.fieldOf("verticalScale").forGetter { it.verticalScale },
                    Codec.FLOAT.fieldOf("horizontalScale").forGetter { it.horizontalScale },
                    Codec.FLOAT.fieldOf("baseHeight").forGetter { it.baseHeight },
                    Codec.FLOAT.fieldOf("slopeScale").forGetter { it.slopeScale },
                    Codec.FLOAT.fieldOf("coastSharpness").forGetter { it.coastSharpness },
                    Codec.FLOAT.fieldOf("valleyErosion").forGetter { it.valleyErosion },
                    Codec.FLOAT.fieldOf("valleyWeirdness").forGetter { it.valleyWeirdness },
                    Codec.FLOAT.fieldOf("riverErosion").forGetter { it.riverErosion },
                    Codec.FLOAT.fieldOf("riverWeirdness").forGetter { it.riverWeirdness },
                    Codec.FLOAT.fieldOf("lakeWeirdness").forGetter { it.lakeWeirdness },
                    Codec.FLOAT.fieldOf("beachNoiseScale").forGetter { it.beachNoiseScale },
                    Codec.FLOAT.fieldOf("beachHeight").forGetter { it.beachHeight },
                    Codec.FLOAT.fieldOf("valleyDepth").forGetter { it.valleyDepth },
                    Codec.FLOAT.fieldOf("valleyWidth").forGetter { it.valleyWidth },
                    Codec.FLOAT.fieldOf("mountainSharpness").forGetter { it.mountainSharpness },
                    Codec.FLOAT.fieldOf("mountainHeightScale").forGetter { it.mountainHeightScale },
                    Codec.FLOAT.fieldOf("mountainScale").forGetter { it.mountainScale },
                    Codec.FLOAT.fieldOf("plateauHeight").forGetter { it.plateauHeight },
                    Codec.INT.fieldOf("aquiferDepthOffset").forGetter { it.aquiferDepthOffset }
                ).apply(i, ::Terrain)
            }
        }
    }

    // --- Dãy núi (Mountain Ranges) ---
    class MountainRanges {
        var density: Int
        var rangeDensity: Float
        var rangeScale: Int
        var scale: Float
        var weight: Int
        var sharpness: Sharpness

        constructor(density: Int, rangeDensity: Float, rangeScale: Int, scale: Float, weight: Int, sharpness: Sharpness) {
            this.density = density
            this.rangeDensity = rangeDensity
            this.rangeScale = rangeScale
            this.scale = scale
            this.weight = weight
            this.sharpness = sharpness
        }

        fun copy() = MountainRanges(density, rangeDensity, rangeScale, scale, weight, sharpness)

        companion object {
            @JvmField
            val CODEC: Codec<MountainRanges> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.INT.fieldOf("density").forGetter { it.density },
                    Codec.FLOAT.fieldOf("rangeDensity").forGetter { it.rangeDensity },
                    Codec.INT.fieldOf("rangeScale").forGetter { it.rangeScale },
                    Codec.FLOAT.fieldOf("scale").forGetter { it.scale },
                    Codec.INT.fieldOf("weight").forGetter { it.weight },
                    Codec.STRING.xmap({ Sharpness.valueOf(it) }, { it.name }).fieldOf("sharpness").forGetter { it.sharpness }
                ).apply(i, ::MountainRanges)
            }
        }
    }

    // Enum cho độ sắc nhọn của núi
    enum class Sharpness {
        LOW, AVERAGE, HIGH, EXTREME
    }

    companion object {
        @JvmField
        val CODEC: Codec<TerrainSettings> = RecordCodecBuilder.create { i ->
            i.group(
                General.CODEC.fieldOf("general").forGetter { it.general },
                Terrain.CODEC.fieldOf("steppe").forGetter { it.steppe },
                Terrain.CODEC.fieldOf("plains").forGetter { it.plains },
                Terrain.CODEC.fieldOf("hills").forGetter { it.hills },
                Terrain.CODEC.fieldOf("dales").forGetter { it.dales },
                Terrain.CODEC.fieldOf("plateau").forGetter { it.plateau },
                Terrain.CODEC.fieldOf("badlands").forGetter { it.badlands },
                Terrain.CODEC.fieldOf("torridonian").forGetter { it.torridonian },
                Terrain.CODEC.fieldOf("mountains").forGetter { it.mountains },
                Terrain.CODEC.fieldOf("volcano").forGetter { it.volcano },
                Terrain.CODEC.fieldOf("blendLow").forGetter { it.blendLow },
                Terrain.CODEC.fieldOf("blendMid").forGetter { it.blendMid },
                Terrain.CODEC.fieldOf("blendHigh").forGetter { it.blendHigh },
                MountainRanges.CODEC.fieldOf("mountainRanges").forGetter { it.mountainRanges }
            ).apply(i, ::TerrainSettings)
        }
    }
}
