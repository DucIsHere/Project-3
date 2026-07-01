package com.mbz.data.worldgen

import net.minecraft.core.HolderGetter
import net.minecraft.core.registries.Registries;l
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.world.level.block.Blocks
import net.minecraft.world.level.levelgen.DensityFunction
import net.minecraft.world.level.levelgen.NoiseGeneratorSettings
import net.minecraft.world.level.levelgen.NoiseSettings
import net.minecraft.world.level.levelgen.synth.NormalNoise

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.settings.CaveSettings
import com.mbz.data.worldgen.preset.settings.Preset
import com.mbz.data.worldgen.preset.settings.WorldSettings
import com.mbz.registries.MBZRegistries
import com.mbz.world.worldgen.noise.module.Noise

object MBZNoiseGeneratorSettings {

    fun bt(preset: Preset, ctx: BootstapContext<NoiseGeneratorSettings>) {
        val densityFunctions = ctx.lookup(Registries.DENSITY_FUNCTION)
        val noiseParams = ctx.lookup(Registries.NOISE)
        val noises = ctx.lookup(RTFRegistries.NOISE)

        val worldSettings = preset.world()
        val properties = worldSettings.properties
        val worldHeight = properties.worldHeight
        val worldDepth = properties.worldDepth

        val caveSettings = preset.caves()

        ctx.register(
            NoiseGeneratorSettings.OVERWORLD,
            NoiseGeneratorSettings(
                NoiseSettings.create(-worldDepth, worldDepth + worldHeight, 1, 2),
                Blocks.STONE.defaultBlockState(),
                Blocks.WATER.defaultBlockState(),
                RGFNoiseRouterData.overworld(preset, densityFunctions, noiseParams, noises),
                RTFSurfaceRuleData.overworld(preset, densityFunctions, noises),
                properties.spawnType.parameterPoints, // Truy cập getter theo style Kotlin
                properties.seaLevel,
                false,
                true,
                caveSettings.largeOreVeins,
                true
            )
        )
    }
}