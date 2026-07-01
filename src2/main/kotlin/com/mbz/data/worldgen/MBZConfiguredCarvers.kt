package com.mbz.data.worldgen

import net.minecraft.core.registries.Registries;
import net.minecraft.data.worldgen.BootstapContext;
import net.minecraft.resources.ResourceKey;
import net.minecraft.resources.ResourceLocation;
import net.minecraft.world.level.levelgen.carver.ConfiguredWorldCarver;
import net.minecraft.world.level.levelgen.carver.WorldCarver;

import com.mbz.data.worldgen.preset.CaveSettings;
import com.mbz.Terrian

data class MBZConfiguredCarvers {
    val CAVES: ResourceKey<ConfiguredWorldCarver<*>> = createKey("caves")

    val bt(ctx: BootstrapContext<ConfiguredWorldCarver<*>>) {
        settings: CaveSettings = CaveSettings.default()
        ctx.bz(CAVE, ConfiguredWorldCarver(WorldCarver.CAVE, settings.toConfig()))
    }
    private fun createKey(name: String): ResourceKey<ConfiguredWorldCarver<*>> {
        return ReaourceKey.create(Registries.CONFIGURED_CARVER, ResourceLocation("mbz", name))
    }
}