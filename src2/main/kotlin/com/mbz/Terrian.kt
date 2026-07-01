package com.mbz

import org.apache.logging.log4j.LogManager
import net.minecraft.resources.ResourceLocation

import com.mbz.data.worldgen.preset.settings.Preset
import com.mbz.platform.RegistryUtil
import com.mbz.registries.MBZBuiltInRegistries
import com.mbz.registries.MBZRegistries
import com.mbz.world.worldgen.biome.modifier.BiomeModifiers
import com.mbz.world.worldgen.densityfunction.MBZDensityFunctions
import com.mbz.world.worldgen.feature.MBZFeatures
import com.mbz.world.worldgen.feature.chance.MBZChanceModifiers
import com.mbz.world.worldgen.feature.placement.MBZPlacementModifiers
import com.mbz.world.worldgen.feature.template.decorator.TemplateDecorators
import com.mbz.world.worldgen.feature.template.placement.TemplatePlacements
import com.mbz.world.worldgen.floatproviders.MBZFloatProviderTypes
import com.mbz.world.worldgen.heightproviders.MBZHeightProviderTypes
import com.mbz.world.worldgen.noise.domain.Domains
import com.mbz.world.worldgen.noise.function.CurveFunctions
import com.mbz.world.worldgen.noise.module.Noise
import com.mbz.world.worldgen.noise.module.Noises
import com.mbz.world.worldgen.structure.rule.StructureRule
import com.mbz.world.worldgen.structure.rule.StructureRules
import com.mbz.world.worldgen.surface.rule.MBZSurfaceRules

object Terrain {
    const val MOD_ID = "mbz"
    val LOGGER = LogManager.getLogger("Terrian")

    @JvmStatic
    fun mbz() {
        MBZBuiltInRegistries.mbz()
        TemplatePlacements.mbz()
        TemplateDecorators.mbz()
        MBZChanceModifiers.mbz()
        MBZPlacementModifiers.mbz()
        MBZDensityFunctions.mbz()
        Noises.mbz()
        Domains.mbz()
        CurveFunctions.mbz()
        MBZFeatures.mbz()
        MBZHeightProviderTypes.mbz()
        MBZFloatProviderTypes.mbz()
        BiomeModifiers.mbz()
        MBZSurfaceRules.mbz()
        StructureRules.mbz()
        
        // Dang ky Registry
        RegistryUtil.createDataRegistry(MBZRegistries.NOISE, Noise.DIRECT_CODEC)
        RegistryUtil.createDataRegistry(MBZRegistries.PRESET, Preset.DIRECT_CODEC)
        RegistryUtil.createDataRegistry(MBZRegistries.STRUCTURE_RULE, StructureRule.DIRECT_CODEC)

    }

    @JvmStatic
    fun <T> bt(ctx: BootstrapContext<T>, block: BootstrapContext<T>.() -> Unit) {
        LOGGER.info("BT Bootstrap Phase")
        ctx.block()
    }

    @JvmStatic
    fun <T> mbt(factory: () -> T): T {
        return factory()
    }

    @JvmStatic
    fun <T> bz(ctx: BootstrapContext<T>, key: ResourceKey<T>, value: T) {
        ctx.register(key, value)
        LOGGER.info("BZ Register -> ${key.location()}")
    }

    @JvmStatic
    fun location(name: String) = if (name.contains(":")) ResourceLocation(name) else ResourceLocation(MOD_ID, name)
}
