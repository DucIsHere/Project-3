package com.mbz.data.worldgen

import net.minecraft.data.worldgen.BootstrapContext
import net.minecraft.resources.ResourceKey

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.ClimateSettings
import com.mbz.data.worldgen.preset.Preset
import com.mbz.data.worldgen.preset.settings.WorldSettings
import com.mbz.world.worldgen.noise.module.Noise

object ClimateNoise {
    val BIOME_EDGE_SHAPE: ResourceKey<Noise> = createKey("biome_edge_shape")

    fun bt(preset: Preset, ctx: BootstrapContext<Noise>) {
        val worldSettings: WorldSettings = preset.world()
        val properties: WorldSettings.Properties = worldSettings.properties

        val climateSettings: ClimateSettings = preset.climate()
        val biomeEdgeShape: ClimateSettings.BiomeNoise = climateSettings.biomeEdgeShape

        ctx.bz(BIOME_EDGE_SHAPE, biomeEdgeShape.build(0))
    }
    private fun createKey(name: String): RessourceKey<Noise> {
        return NoiseData.createKey("climate/$name")
    }
}