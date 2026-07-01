package com.mbz.data.worldgen.preset.settings

// Mojang & Serialization
import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

// Minecraft Core & Registries
import net.minecraft.core.HolderLookup
import net.minecraft.core.Registry
import net.minecraft.core.RegistryAccess
import net.minecraft.core.RegistrySetBuilder
import net.minecraft.core.registries.BuiltInRegistries
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.resources.ResourceKey

// MBZ Project Imports (Dẫn thẳng về package com.mbz mới của mày)
import com.mbz.data.worldgen.preset.PresetBiomeModifierData
import com.mbz.data.worldgen.preset.PresetNoiseData
import com.mbz.data.worldgen.preset.PresetBiomeData
import com.mbz.data.worldgen.preset.PresetConfiguredCarvers
import com.mbz.data.worldgen.preset.PresetConfiguredFeatures
import com.mbz.data.worldgen.preset.PresetDimensionTypes
import com.mbz.data.worldgen.preset.PresetNoiseGeneratorSettings
import com.mbz.data.worldgen.preset.PresetNoiseRouterData
import com.mbz.data.worldgen.preset.PresetPlacedFeatures
import com.mbz.data.worldgen.preset.PresetStructureRuleData
import com.mbz.registries.RTFRegistries

/**
 * Head of Preset System - Project MBZ
 * Design by: All-in Java & Kotlin Hybrid
 */
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
    
    fun copyAll(): Preset {
        return Preset(
            world.copy(), caves.copy(), climate.copy(), terrain.copy(),
            rivers.copy(), filters.copy(), structures.copy(), miscellaneous.copy()
        )
    }

    fun buildPatch(registries: RegistryAccess): HolderLookup.Provider {
        val builder = RegistrySetBuilder()
        
        // Đăng ký các Registry cho MBZ Worldgen
        this.addPatch(builder, RTFRegistries.PRESET) { preset, ctx -> ctx.register(KEY, preset) }
        this.addPatch(builder, RTFRegistries.NOISE, PresetNoiseData::bootstrap)
        this.addPatch(builder, RTFRegistries.BIOME_MODIFIER, PresetBiomeModifierData::bootstrap)
        this.addPatch(builder, RTFRegistries.STRUCTURE_RULE, PresetStructureRuleData::bootstrap)
        
        this.addPatch(builder, Registries.CONFIGURED_FEATURE) { preset, ctx -> PresetConfiguredFeatures.bootstrap(preset, ctx) }
        this.addPatch(builder, Registries.CONFIGURED_CARVER) { preset, ctx -> PresetConfiguredCarvers.bootstrap(preset, ctx) }
        
        this.addPatch(builder, Registries.PLACED_FEATURE, PresetPlacedFeatures::bootstrap)
        this.addPatch(builder, Registries.BIOME, PresetBiomeData::bootstrap)
        this.addPatch(builder, Registries.DIMENSION_TYPE, PresetDimensionTypes::bootstrap)
        this.addPatch(builder, Registries.DENSITY_FUNCTION, PresetNoiseRouterData::bootstrap)
        this.addPatch(builder, Registries.NOISE_SETTINGS, PresetNoiseGeneratorSettings::bootstrap)
        
        return builder.buildPatch(RegistryAccess.fromRegistryOfRegistries(BuiltInRegistries.REGISTRY), registries)
    }

    private fun <T> addPatch(builder: RegistrySetBuilder, key: ResourceKey<out Registry<T>>, patch: Patch<T>) {
        builder.add(key) { ctx -> patch.apply(this, ctx) }
    }

    private fun interface Patch<T> {
        fun apply(preset: Preset, ctx: BootstapContext<T>)
    }

    companion object {
        @Deprecated("Sử dụng key từ RTFRegistries")
        @JvmField
        val KEY: ResourceKey<Preset> = RTFRegistries.createKey(RTFRegistries.PRESET, "preset")

        @JvmField
        val DIRECT_CODEC: Codec<Preset> = RecordCodecBuilder.create { instance ->
            instance.group(
                WorldSettings.CODEC.fieldOf("world").forGetter { it.world },
                CaveSettings.CODEC.optionalFieldOf("caves", CaveSettings(0.0f, 1.5625f, 1.0f, 1.0f, 1.0f, 0.14285715f, 0.07f, 0.02f, true, false)).forGetter { it.caves },
                ClimateSettings.CODEC.fieldOf("climate").forGetter { it.climate },
                TerrainSettings.CODEC.fieldOf("terrain").forGetter { it.terrain },
                RiverSettings.CODEC.fieldOf("rivers").forGetter { it.rivers },
                FilterSettings.CODEC.fieldOf("filters").forGetter { it.filters },
                StructureSettings.CODEC.fieldOf("structures").forGetter { it.structures },
                MiscellaneousSettings.CODEC.fieldOf("miscellaneous").forGetter { it.miscellaneous }
            ).apply(instance, ::Preset)
        }
    }
}
