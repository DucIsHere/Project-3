package com.mbz.data.worldgen.preset

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder
import net.minecraft.core.HolderLookup
import net.minecraft.core.Registry
import net.minecraft.core.RegistryAccess
import net.minecraft.core.RegistrySetBuilder
import net.minecraft.core.registries.BuiltInRegistries
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.resources.ResourceKey
import com.mbz.data.worldgen.BiomeModifierData
import com.mbz.data.worldgen.NoiseData
import com.mbz.data.worldgen.MBZBiomeData
import com.mbz.data.worldgen.MBZConfiguredCarvers
import com.mbz.data.worldgen.MBZConfiguredFeatures
import com.mbz.data.worldgen.MBZDimensionTypes
import com.mbz.data.worldgen.MBZNoiseGenerationSettings
import com.mbz.data.worldgen.MBZNoiseRouterData
import com.mbz.data.worldgen.MBZPlacedFeatures
import com.mbz.data.worldgen.MBZStructureRuleData
import com.mbz.registries.MBZRegistries

data class Preset(
    val world: WorldSettings,
    val caves: CaveSettings,
    val climate: ClimateSettings,
    val terrain: TerrainSettings,
    val rivers: RiverSettings,
    val filters: FilterSettings,
    val structures: StructureSettings,
    val miscellaneous: MiscellaneousSettings
) {
    companion object {
        val DIRECT_CODEC: Codec<Preset> = RecordCodecBuilder.create { instance ->
            instance.group(
                WorldSettings.CODEC.fieldOf("world").forGetter(Preset::world),
                CaveSettings.CODEC.optionalFieldOf(
                    "caves", 
                    CaveSettings(0.0f, 1.5625f, 1.0f, 1.0f, 1.0f, 0.14285715f, 0.07f, 0.02f, true, false)
                ).forGetter(Preset::caves),
                ClimateSettings.CODEC.fieldOf("climate").forGetter(Preset::climate),
                TerrainSettings.CODEC.fieldOf("terrain").forGetter(Preset::terrain),
                RiverSettings.CODEC.fieldOf("rivers").forGetter(Preset::rivers),
                FilterSettings.CODEC.fieldOf("filters").forGetter(Preset::filters),
                StructureSettings.CODEC.fieldOf("structures").forGetter(Preset::structures),
                MiscellaneousSettings.CODEC.fieldOf("miscellaneous").forGetter(Preset::miscellaneous)
            ).apply(instance, ::Preset)
        }

        @Deprecated("Sử dụng RGFRegistries thay thế")
        val KEY: ResourceKey<Preset> = RGFRegistries.createKey(RGFRegistries.PRESET, "preset")
    }

    // data class đã có sẵn hàm copy(), nhưng nếu các object con cần deep copy:
    fun deepCopy() = Preset(
        world.copy(), caves.copy(), climate.copy(), terrain.copy(),
        rivers.copy(), filters.copy(), structures.copy(), miscellaneous.copy()
    )

    fun buildPatch(registries: RegistryAccess): HolderLookup.Provider {
        val builder = RegistrySetBuilder()
        
        // Cách viết gọn gàng với Trailing Lambda
        addPatch(builder, RGFRegistries.PRESET) { preset, ctx -> ctx.register(KEY, preset) }
        addPatch(builder, RGFRegistries.NOISE, NoiseData::bootstrap)
        addPatch(builder, RGFRegistries.BIOME_MODIFIER, BiomeModifierData::bootstrap)
        addPatch(builder, RGFRegistries.STRUCTURE_RULE, StructureRuleData::bootstrap)
        
        addPatch(builder, Registries.CONFIGURED_FEATURE) { preset, ctx -> RGFConfiguredFeatures.bootstrap(preset, ctx) }
        addPatch(builder, Registries.CONFIGURED_CARVER) { preset, ctx -> RGFConfiguredCarvers.bootstrap(preset, ctx) }
        
        addPatch(builder, Registries.PLACED_FEATURE, RGPlacedFeatures::bootstrap)
        addPatch(builder, Registries.BIOME, RGFBiomeData::bootstrap)
        addPatch(builder, Registries.DIMENSION_TYPE, RGFDimensionTypes::bootstrap)
        addPatch(builder, Registries.DENSITY_FUNCTION, RGFNoiseRouterData::bootstrap)
        addPatch(builder, Registries.NOISE_SETTINGS, RGFNoiseGeneratorSettings::bootstrap)

        return builder.buildPatch(RegistryAccess.fromRegistryOfRegistries(BuiltInRegistries.REGISTRY), registries)
    }

    // Generic function trong Kotlin dùng dấu <T> trước tên hàm
    private fun <T> addPatch(
        builder: RegistrySetBuilder, 
        key: ResourceKey<out Registry<T>>, 
        patch: Patch<T>
    ) {
        builder.add(key) { ctx ->
            patch.apply(this, ctx)
        }
    }

    // Functional Interface (SAM Conversion) giúp truyền Lambda cực mượt
    fun interface Patch<T> {
        fun apply(preset: Preset, ctx: BootstapContext<T>)
    }
}