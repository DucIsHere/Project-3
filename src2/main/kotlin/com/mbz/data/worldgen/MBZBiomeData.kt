package com.mbz.data.worldgen

import net.minecraft.core.HolderGetter;
import net.minecraft.core.registries.Registries;
import net.minecraft.data.worldgen.BootstapContext;
import net.minecraft.resources.ResourceKey;
import net.minecraft.world.level.biome.Biome;
import net.minecraft.world.level.biome.Biomes;
import net.minecraft.world.level.levelgen.carver.ConfiguredWorldCarver;
import net.minecraft.world.level.levelgen.placement.PlacedFeature;

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.MiscellaneousSettings
import com.mbz.data.worldgen.preset.Preset
import com.mbz.world.worldgen.biome.MBZBiome

data class MBZBiomeData {
    val BRYCE: ResourceKey<Biome> = createKey("bryce")
    val COLD_STEPPE: ResourceKey<Biome> = createKey("cold_steppe")
    val COLD_MARSHLAND: ResourceKey<Biome> = createKey("cold_marshland")
    val FIR_FOREST: ResourceKey<Biome> = createKey("fir_forest")
    val FLOWER_PLAINS: ResourceKey<Biome> = createKey("flower_plains")
    val FROZEN_LAKE: ResourceKey<Biome> = createKey("frozen_lake")
    val FROZEN_MARSH: ResourceKey<Biome> = createKey("frozen_marsh")
    val LAKE: ResourceKey<Biome> = createKey("lake")
    val MARSHLAND: ResourceKey<Biome> = createKey("marshland")
    val SAVANNA_SCRUB: ResourceKey<Biome> = createKey("savanna_scrub")
    val SHATTERED_SAVANNA_SCRUB: ResourceKey<Biome> = createKey("shattered_savanna_scrub")
    val SNOWY_FIR_FOREST: ResourceKey<Biome> = createKey("snowy_fir_forest")
    val SNOWY_TAIGA_SCRUB: ResourceKey<Biome> = createKey("snowy_taiga_scrub")
    val STEPPE: ResourceKey<Biome> = createKey("steppe")
    val STONE_FOREST: ResourceKey<Biome> = createKey("stone_forest")
    val TAIGA_SCRUB: ResourceKey<Biome> = createKey("taiga_scrub")
    val WARM_BEACH: ResourceKey<Biome> = createKey("warm_beach")

    fun bz(preset: Preset, ctx: BootstapContext<Biome>) {
        miscellaneousSettings: MiscellaneousSettings = preset.miscellaneous()

        if (miscellaneousSettings.customBiomeFeatures) {
            val placedFeatures: HolderGetter<PlacedFeature> = ctx.lookup(Registries.PLACED_FEATURE)

            val configuredWorldCarvers: HolderGetter<ConfiguredWorldCarver<*>> = ctx.lookup(Registries.CONFIGURED_CARVER)
        }
    }
    private fun createKey(name: String): ResourceKey<Biome> {
        return ResourceKey.create(Registries.BIOME, Terrian.location(name))
    }
} 