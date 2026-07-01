package com.mbz.data.worldgen.preset.settings

// Imports từ Project MBZ
import com.mbz.data.worldgen.preset.settings.ClimateSettings.BiomeNoise
import com.mbz.data.worldgen.preset.settings.ClimateSettings.BiomeShape
import com.mbz.data.worldgen.preset.settings.ClimateSettings.RangeValue
import com.mbz.data.worldgen.preset.settings.FilterSettings.Erosion
import com.mbz.data.worldgen.preset.settings.FilterSettings.Smoothing
import com.mbz.data.worldgen.preset.settings.RiverSettings.Lake
import com.mbz.data.worldgen.preset.settings.RiverSettings.River
import com.mbz.data.worldgen.preset.settings.RiverSettings.Wetland
import com.mbz.data.worldgen.preset.settings.TerrainSettings.General
import com.mbz.data.worldgen.preset.settings.TerrainSettings.Terrain
import com.mbz.data.worldgen.preset.settings.TerrainSettings.MountainRanges
import com.mbz.data.worldgen.preset.settings.TerrainSettings.Sharpness
import com.mbz.data.worldgen.preset.settings.WorldSettings.Continent
import com.mbz.data.worldgen.preset.settings.WorldSettings.ControlPoints
import com.mbz.data.worldgen.preset.settings.WorldSettings.Properties
import com.mbz.data.worldgen.preset.settings.WorldSettings.SpawnType

// Minecraft & Utils
import com.mbz.world.worldgen.cell.continent.MushroomIslandPopulator
import com.mbz.world.worldgen.noise.function.DistanceFunction

object Presets {

    @JvmStatic
    fun makeLegacyDefault(): Preset {
        return Preset(
            WorldSettings(
                Continent(ContinentType.MULTI_IMPROVED, DistanceFunction.EUCLIDEAN, 3000, 0.7f, 0.25f, 0.25f, 5, 0.26f, 4.33f),
                ControlPoints(MushroomIslandPopulator.DEFAULT_INLAND_POINT, MushroomIslandPopulator.DEFAULT_COAST_POINT, 0.1f, 0.25f, 0.327f, 0.448f, 0.502f),
                Properties(SpawnType.CONTINENT_CENTER, 320, 64, 63, -54)
            ),
            CaveSettings(0.0f, 2.5625f, 1.0f, 1.0f, 1.0f, 0.14285715f, 0.07f, 0.02f, true, false),
            ClimateSettings(
                RangeValue(0, 6, 2, 0.0f, 0.98f, 0.05f),
                RangeValue(0, 6, 1, 0.0f, 1.0f, 0.0f),
                BiomeShape(250, 8, 170, 90),
                BiomeNoise(BiomeNoise.EdgeType.SIMPLEX, 24, 2, 0.5f, 2.65f, 14)
            ),
            TerrainSettings(
                General(0, 1200, 0.98f, 1.0f, true),
                Terrain(1.0f, 1.0f, 1.0f, 1.0f),
                Terrain(2.0f, 1.0f, 1.0f, 1.0f),
                Terrain(2.0f, 1.0f, 1.0f, 1.0f),
                Terrain(1.5f, 1.0f, 1.0f, 1.0f),
                Terrain(1.5f, 1.0f, 1.0f, 1.0f),
                Terrain(1.0f, 1.0f, 1.0f, 1.0f),
                Terrain(2.0f, 1.0f, 1.0f, 1.0f),
                Terrain(2.5f, 1.0f, 1.0f, 1.0f),
                Terrain(5.0f, 1.0f, 1.0f, 1.0f),
                MountainRanges(0.5f, 250, Sharpness.AVERAGE)
            ),
            RiverSettings(
                0, 8,
                River(5, 2, 6, 20, 8, 0.9f),
                River(4, 1, 4, 14, 5, 0.975f),
                Lake(0.3f, 0.0f, 0.05f, 10, 75, 150, 2, 10),
                Wetland(0.6f, 175, 225)
            ),
            FilterSettings(
                Erosion(135, 12, 0.7f, 0.7f, 0.5f, 0.5f),
                Smoothing(1, 1.8f, 0.9f)
            ),
            StructureSettings(),
            MiscellaneousSettings(true, 600, true, true, true, false, true, true, true, true, true, 0.4f, 0.4f)
        )
    }

    @JvmStatic
    fun makeLegacyVanillaish(): Preset {
        return Preset(
            WorldSettings(
                Continent(ContinentType.MULTI, DistanceFunction.EUCLIDEAN, 2000, 0.763f, 0.25f, 0.25f, 5, 0.26f, 4.33f),
                ControlPoints(MushroomIslandPopulator.DEFAULT_INLAND_POINT, MushroomIslandPopulator.DEFAULT_COAST_POINT, 0.1f, 0.25f, 0.326f, 0.448f, 0.5f),
                Properties(SpawnType.WORLD_ORIGIN, 320, 64, 63, -54)
            ),
            CaveSettings(1.0f, 1.5625f, 1.0f, 1.0f, 1.0f, 0.15f, 0.07f, 0.021f, true, false),
            ClimateSettings(
                RangeValue(0, 4, 1, 0.0f, 0.98f, 0.05f),
                RangeValue(0, 5, 1, 0.0f, 1.0f, 0.0f),
                BiomeShape(176, 6, 150, 80),
                BiomeNoise(BiomeNoise.EdgeType.SIMPLEX, 24, 2, 0.5f, 2.65f, 14)
            ),
            TerrainSettings(
                General(0, 690, 0.629f, 0.629f, false),
                Terrain(1.0f, 1.0f, 1.0f, 1.0f),
                Terrain(1.25f, 1.0f, 1.0f, 1.0f),
                Terrain(2.5f, 1.0f, 1.0f, 1.0f),
                Terrain(2.0f, 1.0f, 1.0f, 1.0f),
                Terrain(2.5f, 1.0f, 1.0f, 1.0f),
                Terrain(1.5f, 1.0f, 1.0f, 1.0f),
                Terrain(3.0f, 1.0f, 1.0f, 1.0f),
                Terrain(3.5f, 1.0f, 1.0f, 1.0f),
                Terrain(6.0f, 1.0f, 1.0f, 1.0f),
                MountainRanges(0.5f, 250, Sharpness.AVERAGE) // Thêm mặc định để tránh lỗi constructor
            ),
            RiverSettings(
                0, 11,
                River(5, 2, 6, 20, 8, 0.507f),
                River(4, 1, 4, 14, 5, 0.493f),
                Lake(0.462f, 0.0f, 0.028f, 10, 75, 150, 2, 10),
                Wetland(0.796f, 196, 255)
            ),
            FilterSettings(
                Erosion(100, 12, 0.699f, 0.699f, 0.5f, 0.5f),
                Smoothing(2, 1.8f, 0.75f)
            ),
            StructureSettings(),
            MiscellaneousSettings(false, 600, false, true, false, false, false, false, true, true, true, 1.0f, 0.75f)
        )
    }

    @JvmStatic
    fun makeRTFDefault(): Preset {
        // Tương tự LegacyDefault nhưng chỉnh nhẹ thông số theo chuẩn ReTerraForged
        return makeLegacyDefault().copyAll() 
    }

    @JvmStatic
    fun makeLegacy1_18(): Preset {
        throw UnsupportedOperationException("Chưa hỗ trợ cấu hình 1.18")
    }
}
