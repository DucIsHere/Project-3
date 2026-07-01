package com.mbz.data.worldgen.preset.settings

import com.mbz.world.worldgen.cell.continent.MushroomIslandPopulator
import com.mbz.world.worldgen.noise.function.DistanceFunction
import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class WorldSettings(
    var continent: Continent,
    var controlPoints: ControlPoints,
    var properties: Properties
) {
    fun copy() = WorldSettings(continent.copy(), controlPoints.copy(), properties.copy())

    // --- Cấu hình Lục địa ---
    class Continent(
        var continentType: ContinentType,
        var continentShape: DistanceFunction,
        var continentScale: Int,
        var continentJitter: Float,
        var continentSkipping: Float,
        var continentSizeVariance: Float,
        var continentNoiseOctaves: Int,
        var continentNoiseGain: Float,
        var continentNoiseLacunarity: Float,
        var continentWarpStrength: Float,
        var continentWarpScale: Float
    ) {
        fun copy() = Continent(
            continentType, continentShape, continentScale, continentJitter, continentSkipping,
            continentSizeVariance, continentNoiseOctaves, continentNoiseGain, continentNoiseLacunarity,
            continentWarpStrength, continentWarpScale
        )

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
                    Codec.FLOAT.optionalFieldOf("continentNoiseLacunarity", 4.33f).forGetter { it.continentNoiseLacunarity },
                    Codec.FLOAT.optionalFieldOf("continentWarpStrength", 25f).forGetter { it.continentWarpStrength },
                    Codec.FLOAT.optionalFieldOf("continentWarpScale", 200f).forGetter { it.continentWarpScale }
                ).apply(i, ::Continent)
            }
        }
    }

    // --- Các điểm kiểm soát độ cao (Ocean, Beach, Coast, Inland) ---
    class ControlPoints(
        var mushroomFieldsInland: Float,
        var mushroomFieldsCoast: Float,
        var deepOcean: Float,
        var shallowOcean: Float,
        var beach: Float,
        var coast: Float,
        var inland: Float,
        var coastLineBlend: Float
    ) {
        fun copy() = ControlPoints(
            mushroomFieldsInland, mushroomFieldsCoast, deepOcean, shallowOcean, 
            beach, coast, inland, coastLineBlend
        )

        companion object {
            @JvmField
            val CODEC: Codec<ControlPoints> = RecordCodecBuilder.create { i ->
                i.group(
                    Codec.FLOAT.optionalFieldOf("mushroomFieldsInland", MushroomIslandPopulator.DEFAULT_INLAND_POINT).forGetter { it.mushroomFieldsInland },
                    Codec.FLOAT.optionalFieldOf("mushroomFieldsCoast", MushroomIslandPopulator.DEFAULT_COAST_POINT).forGetter { it.mushroomFieldsCoast },
                    Codec.FLOAT.fieldOf("deepOcean").forGetter { it.deepOcean },
                    Codec.FLOAT.fieldOf("shallowOcean").forGetter { it.shallowOcean },
                    Codec.FLOAT.fieldOf("beach").forGetter { it.beach },
                    Codec.FLOAT.fieldOf("coast").forGetter { it.coast },
                    Codec.FLOAT.fieldOf("inland").forGetter { it.inland },
                    Codec.FLOAT.optionalFieldOf("coastLineBlend", 0.25f).forGetter { it.coastLineBlend }
                ).apply(i, ::ControlPoints)
            }
        }
    }

    // --- Thuộc tính vật lý của thế giới ---
    class Properties(
        var spawnType: SpawnType,
        var worldHeight: Int,
        var worldMinY: Int,
        var riverScale: Float,
        var structureSeparation: Int,
        var worldDepth: Int,
        var seaLevel: Int,
        var lavaLevel: Int,
        var javaEngineWorld: Boolean
    ) {
        fun copy() = Properties(
            spawnType, worldHeight, worldMinY, riverScale, structureSeparation, 
            worldDepth, seaLevel, lavaLevel, javaEngineWorld
        )

        @Deprecated("Sử dụng worldHeight trực tiếp")
        fun terrainScaler(): Int = Math.min(this.worldHeight, 640)

        companion object {
            @JvmField
            val CODEC: Codec<Properties> = RecordCodecBuilder.create { i ->
                i.group(
                    SpawnType.CODEC.fieldOf("spawnType").forGetter { it.spawnType },
                    Codec.INT.fieldOf("worldHeight").forGetter { it.worldHeight },
                    Codec.INT.fieldOf("worldMinY").forGetter { it.worldMinY },
                    Codec.FLOAT.fieldOf("riverScale").forGetter { it.riverScale },
                    Codec.INT.fieldOf("structureSeparation").forGetter { it.structureSeparation },
                    Codec.INT.optionalFieldOf("worldDepth", -64).forGetter { it.worldDepth },
                    Codec.INT.fieldOf("seaLevel").forGetter { it.seaLevel },
                    Codec.INT.optionalFieldOf("lavaLevel", -54).forGetter { it.lavaLevel },
                    Codec.BOOL.optionalFieldOf("JavaEngineWorld", true).forGetter { it.javaEngineWorld }
                ).apply(i, ::Properties)
            }
        }
    }

    companion object {
        @JvmField
        val CODEC: Codec<WorldSettings> = RecordCodecBuilder.create { i ->
            i.group(
                Continent.CODEC.fieldOf("continent").forGetter { it.continent },
                ControlPoints.CODEC.fieldOf("controlPoints").forGetter { it.controlPoints },
                Properties.CODEC.fieldOf("properties").forGetter { it.properties }
            ).apply(i, ::WorldSettings)
        }
    }
}
