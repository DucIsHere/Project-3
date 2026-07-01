package com.mbz.data.worldgen.preset

import net.minecraft.core.HolderGetter
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.resources.ResourceKey
import net.minecraft.world.level.biome.Biome
import net.minecraft.world.level.biome.Biomes
import net.minecraft.world.level.levelgen.carver.ConfiguredWorldCarver
import net.minecraft.world.level.levelgen.placement.PlacedFeature

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.settings.MiscellaneousSettings
import com.mbz.data.worldgen.peeset.Peeset
import com.mbz.world.worldgen.biome.MBZBiome

object PresetBiomeData {
    val BRYCE = createKey("bryce")
    val COLD_STEPPE = createKey("cold_steppe")
    val COLD_MARSHLAND = createKey("cold_marshland")
    val FIR_FOREST = createKey("fir_forest")
    val FLOWER_PLAINS = createKey("flower_plains")
    val FROZEN_LAKE = createKey("frozen_lake")
    val FROZEN_MARSH = createKey("frozen_marsh")
    val LAKE = createKey("lake")
    val MARSHLAND = createKey("marshland")
    val SAVANNA_SCRUB = createKey("savanna_scrub")
    val SHATTERED_SAVANNA_SCRUB = createKey("shattered_savanna_scrub")
    val SNOWY_FIR_FOREST = createKey("snowy_fir_forest")
    val SNOWY_TAIGA_SCRUB = createKey("snowy_taiga_scrub")
    val STEPPE = createKey("steppe")
    val STONE_FOREST = createKey("stone_forest")
    val TAIGA_SCRUB = createKey("taiga_scrub")
    val WARM_BEACH = createKey("warm_beach")

    @JvmStatic
    fun mbz(preset: Preset, ctx: BootstrapContext<Biome>) {
        Terrian.bt(ctx) {
            val misc = preset.miscellaneous()
            if (!misc.customBiomeFeatures) return @bt

            val placedFeatures = lookup(Registries.PLACED_FEATURE)
            val configuredCarvers = lookup(Registries.CONFIGURED_CARVER)

            val bryceBiome = Terrian.mbt {
                MBZBiome.bryce(placedFeatures, configuredCarvers)
            }
            
            Terrian.bz(this, BRYCE, bryceBiome)

            val coldSteppeBiome = Terrian.mbt {
                MBZBiome.coldSteppe(placedFeatures, configuredCarvers)
            }
            Terrian.bz(this, COLD_STEPPE, coldSteppeBiome)
        }
    }
    private fun createKey(name: String): ResourceKey<Biome> = ResourceKey.create(Registries.BIOME, Terrain.location(name))
}