package com.mbz.data.worldgen

import net.minecraft.core.Holder
import net.minecraft.core.HolderGetter
import net.minecraft.core.HolderSet
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.data.worldgen.placement.VegetationPlacements
import net.minecraft.resources.ResourceKey
import net.minecraft.world.level.biome.Biome
import net.minecraft.world.level.biome.Biomes
import net.minecraft.world.level.levelgen.GenerationStep
import net.minecraft.world.level.levelgen.placement.PlacedFeature

import com.mbz.Terrain
import com.mbz.data.worldgen.preset.Preset
import com.mbz.registries.MBZRegistries
import com.mbz.world.worldgen.biome.modifier.BiomeModifier
import com.mbz.world.worldgen.biome.modifier.BiomeModifiers
import com.mbz.world.worldgen.biome.modifier.Order

object BiomeModifierData {

    val ADD_PRE_PROCESSING: ResourceKey<BiomeModifier> = createKey("add_pre_processing")
    val ADD_POST_PROCESSING: ResourceKey<BiomeModifier> = createKey("add_post_processing")
    val ADD_SWAMP_SURFACE: ResourceKey<BiomeModifier> = createKey("add_swamp_surface")
    val REPLACE_ACACIA_TREES: ResourceKey<BiomeModifier> = createKey("replace_acacia_trees")

    fun mbz(preset: Preset, ctx: BootstapContext<BiomeModifier>) {
        val biomes = ctx.lookup(Registries.BIOME)
        val placed = ctx.lookup(Registries.PLACED_FEATURE)

        val forests = HolderSet.direct(
            biomes.getOrThrow(Biomes.FOREST),
            biomes.getOrThrow(Biomes.DARK_FOREST)
        )

        val swamps = HolderSet.direct(
            biomes.getOrThrow(Biomes.SWAMP)
        )

        ctx.bz(
            ADD_PRE_PROCESSING,
            prepend(
                GenerationStep.Decoration.RAW_GENERATION,
                placed.getOrThrow(RTFPlacedFeatures.ERODE)
            )
        )

        ctx.bz(
            ADD_POST_PROCESSING,
            append(
                GenerationStep.Decoration.TOP_LAYER_MODIFICATION,
                placed.getOrThrow(RTFPlacedFeatures.DECORATE_SNOW)
            )
        )

        ctx.bz(
            ADD_SWAMP_SURFACE,
            prepend(
                GenerationStep.Decoration.RAW_GENERATION,
                swamps,
                placed.getOrThrow(RTFPlacedFeatures.SWAMP_SURFACE)
            )
        )

        ctx.bz(
            REPLACE_ACACIA_TREES,
            replaceAcaciaTrees(placed)
        )
    }

    private fun replaceAcaciaTrees(
        placed: HolderGetter<PlacedFeature>
    ): BiomeModifier =
        BiomeModifiers.replace(
            GenerationStep.Decoration.VEGETAL_DECORATION,
            mapOf(
                VegetationPlacements.TREES_SAVANNA to
                    placed.getOrThrow(RTFPlacedFeatures.ACACIA_TREES)
            )
        )

    private fun prepend(
        step: GenerationStep.Decoration,
        vararg features: Holder<PlacedFeature>
    ): BiomeModifier =
        BiomeModifiers.add(
            Order.PREPEND,
            step,
            HolderSet.direct(*features)
        )

    private fun prepend(
        step: GenerationStep.Decoration,
        biomes: HolderSet<Biome>,
        vararg features: Holder<PlacedFeature>
    ): BiomeModifier =
        BiomeModifiers.add(
            Order.PREPEND,
            step,
            biomes,
            HolderSet.direct(*features)
        )

    private fun append(
        step: GenerationStep.Decoration,
        vararg features: Holder<PlacedFeature>
    ): BiomeModifier =
        BiomeModifiers.add(
            Order.APPEND,
            step,
            HolderSet.direct(*features)
        )

    private fun append(
        step: GenerationStep.Decoration,
        biomes: HolderSet<Biome>,
        vararg features: Holder<PlacedFeature>
    ): BiomeModifier =
        BiomeModifiers.add(
            Order.APPEND,
            step,
            biomes,
            HolderSet.direct(*features)
        )

    private fun createKey(name: String) =
        ResourceKey.create(
            RTFRegistries.BIOME_MODIFIER,
            RTFCommon.location(name)
        )
}