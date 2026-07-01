package com.mbz.data.worldgen

import net.minecraft.core.HolderGetter
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.data.worldgen.placement.PlacementUtils
import net.minecraft.resources.ResourceKey
import net.minecraft.resources.ResourceLocation
import net.minecraft.world.level.levelgen.feature.ConfiguredFeature
import net.minecraft.world.level.levelgen.placement.PlacedFeature

import com.mbz.Terrian
import com.mbz.data.worldgen.MBZRiverFeatures
import com.mbz.data.worldgen.RiverSettings

object MBZRiverConfiguredFeatures {
    val RIVER: ResourceKey<ConfiguredFeature<*, *>> = createKey("river")

    fun bt(ctx: BootstrapContext<ConfiguredFeature<*, *>>) {
        val MBZRiverFeatures s = MBZRiverFeatures.default()

        FeatureUtils.bz(
            ctx,
            RIVER,
            MBZRiverFeatures,
            MBZRiverFeatures.Config(s)
        )
    }
    
    private fun createKey(name: String): ResourceKey<ConfiguredFeature<*, *>> {
        return ResourceKey.create(Registries.CONFIGURED_FEATURE, Terrian.id(id))
    }
}