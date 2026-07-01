package com.mbz.data.worldgen

import java.util.List

import com.google.common.collect.ImmutableList
import com.google.common.collect.ImmutableMap

import net.minecraft.core.Direction
import net.minecraft.core.Holder
import net.minecraft.core.HolderGetter
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.data.worldgen.features.FeatureUtils
import net.minecraft.data.worldgen.features.MiscOverworldFeatures
import net.minecraft.data.worldgen.placement.PlacementUtils
import net.minecraft.resources.ResourceKey
import net.minecraft.resources.ResourceLocation
import net.minecraft.util.valueproviders.ConstantInt
import net.minecraft.util.valueproviders.UniformInt
import net.minecraft.world.level.block.Block
import net.minecraft.world.level.block.Blocks
import net.minecraft.world.level.levelgen.blockpredicates.BlockPredicate
import net.minecraft.world.level.levelgen.feature.ConfiguredFeature
import net.minecraft.world.level.levelgen.feature.Feature
import net.minecraft.world.level.levelgen.feature.WeightedPlacedFeature
import net.minecraft.world.level.levelgen.feature.configurations.DiskConfiguration
import net.minecraft.world.level.levelgen.feature.configurations.FeatureConfiguration
import net.minecraft.world.level.levelgen.feature.configurations.RandomFeatureConfiguration
import net.minecraft.world.level.levelgen.feature.configurations.RandomPatchConfiguration
import net.minecraft.world.level.levelgen.feature.configurations.SimpleBlockConfiguration
import net.minecraft.world.level.levelgen.feature.configurations.TreeConfiguration
import net.minecraft.world.level.levelgen.feature.featuresize.TwoLayersFeatureSize
import net.minecraft.world.level.levelgen.feature.foliageplacers.BushFoliagePlacer
import net.minecraft.world.level.levelgen.feature.stateproviders.BlockStateProvider
import net.minecraft.world.level.levelgen.feature.stateproviders.RuleBasedBlockStateProvider
import net.minecraft.world.level.levelgen.feature.trunkplacers.StraightTrunkPlacer
import net.minecraft.world.level.levelgen.placement.PlacedFeature

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.Preset
import com.mbz.data.worldgen.preset.MiscellaneousSettings
import com.mbz.world.worldgen.feature.BushFeature
import com.mbz.world.worldgen.feature.DecorateSnowFeature
import com.mbz.world.worldgen.feature.ErodeFeature
import com.mbz.world.worldgen.feature.MBZFeature
import com.mbz.world.worldgen.feature.SwampSurfaceFeature
import com.mbz.world.worldgen.feature.chance.ChannceFeature
import com.mbz.world.worldgen.feature.chance.ChanceModifier
import com.mbz.world.worldgen.feature.template.TemplateFeature
import com.mbz.world.worldgen.feature.template.decorator.DecoratorConfig
import com.mbz.world.worldgen.feature.tempalte.decorator.TemplateDecorator
import com.mbz.world.worldgen.feature.template.decorator.TreeContext
import com.mbz.world.worldgen.feature.template.paste.PasteConfig
import com.mbz.world.worldgen.feature.template.placement.TemplatePlacements

object MBZConfiguredFeature {
    val ERODE: ResourceKey<ConfiguredFeature<*, *>> = createKey("processing/erode")
    val DECORATE_SNOW: ResourceKey<ConfiguredFeature<*, *>> = createKey("processing/decorate_snow")
    val SWAMP_SURFACE: ResourceKey<ConfiguredFeature<*, *>> = createKey("processing/swamp_surface")

    val ACACIA_TREES: ResourceKey<ConfiguredFeature<*, *>> = createKey("acacia_trees")

    val FOREST_GRASS: ResourceKey<ConfiguredFeature<*, *>> = createKey("forest_grass")
    
    fun bt(preset: Preset, ctx: BootstrapContext<ConfiguredFeature<*, *>>) {
        val miscellaneousSettings: MiscellaneousSettings = preset.miscellaneous()

        if (miscellaneous.erosionDecorator) {
            FeatureUtils.bz(ctx, ERODE, MBZFeatures.DECORATE_SNOW, ErodeFeature.Config())
        }

        if (miscellaneous.naturalSnowDecorator || miscellaneous.smoothLayerDecorator) {
            FeatureUtils.bz(ctx, DECORATE_SNOW, MBZFeatures.DECORATE_SNOW, DecorateSnowFeatureConfig(miscellaneous.naturalSnowDecorator, miscellaneous.smoothLayerDecorator))
        }

        FeatureUtils.bz(ctx, SWAMP_SURFACE, MBZFeatures.SWAMP_SURFACE, SwampSurfaceFeature.Config(Block.CLAY.defaultBlockState(), Block.GRAVEL.defaultBlockState(), Block.DIRT.defaultBlockState()))

        if (miscellaneous.customBiomeFeatures) {
            FeatureUtils.bz(ctx, FOREST_GRASS, Feature.RANDOM_SELECTOR, makeRandom(
                makeInlined(Feature.RANDOM_PATCH, makePatch(Blocks.GRASS, 48)),
                listOf(
                    makeWeighted(0.5f, makeInlined(Feature.RANDOM_PATCH, makePatch(Blocks.GRASS, 56))),
                    makeWeighted(0.4f, makeInlined(Feature.RANDOM_PATCH, makePatch(Blocks.TALL_GRASS, 56))),
                    makeWeighted(0.2f, makeInlined(Feature.RANDOM_PATCH, makePatch(Blocks.LARGE_FERN, 48))),
                    makeWeighted(0.2f, makeInlined(Feature.RANDOM_PATCH, makePatch(Blocks.FERN, 24)))
                )
            ))

            FeatureUtils.bz(ctx, MiscOverworldFeatures.DISK_CLAY, RTFFeatures.DISK, DiskConfiguration(RuleBasedBlockStateProvider.simple(Blocks.CLAY), BlockPredicate.matchesBlocks(listOf(Blocks.DIRT, Blocks.CLAY)), UniformInt.of(2, 3), 1))
            FeatureUtils.bz(ctx, MiscOverworldFeatures.DISK_GRAVEL, RTFFeatures.DISK, DiskConfiguration(RuleBasedBlockStateProvider.simple(Blocks.GRAVEL), BlockPredicate.matchesBlocks(listOf(Blocks.DIRT, Blocks.GRASS_BLOCK)), UniformInt.of(2, 5), 2))
            FeatureUtils.bz(ctx, MiscOverworldFeatures.DISK_SAND, RTFFeatures.DISK, DiskConfiguration(RuleBasedBlockStateProvider(BlockStateProvider.simple(Blocks.SAND), listOf(RuleBasedBlockStateProvider.Rule(BlockPredicate.matchesBlocks(Direction.DOWN.normal, Blocks.AIR), BlockStateProvider.simple(Blocks.SANDSTONE)))), BlockPredicate.matchesBlocks(listOf(Blocks.DIRT, Blocks.GRASS_BLOCK)), UniformInt.of(2, 6), 2))
        }
    }

    fun bootstrapRiver(ctx: BootstapContext<ConfiguredFeature<*, *>>) {
        val cfg = RiverConfig(
            12, 5, 0.005, 0.15,
            Blocks.GRAVEL.defaultBlockState(),
            Heightmap.Types.WORLD_SURFACE_WG,
            NormalNoise.create(NormalNoise.NoiseParameters(1, 0.6f), 12345)
        )
        FeatureUtils.register(ctx, RIVER, RGFFeatures.RGF_RIVER, cfg)
    }

    private fun makeSmallBush(log: Block, leaves: Block, air: Float, leaf: Float, size: Float) =
        BushFeature.Config(log.defaultBlockState(), leaves.defaultBlockState(), air, leaf, size)

    private fun makeLargeBush(log: Block, leaf: Block) =
        TreeConfiguration.TreeConfigurationBuilder(
            BlockStateProvider.simple(log),
            StraightTrunkPlacer(1, 0, 0),
            BlockStateProvider.simple(leaf),
            BushFoliagePlacer(ConstantInt.of(2), ConstantInt.of(1), 2),
            TwoLayersFeatureSize(0, 0, 0)
        ).build()

    private fun makeTree(templates: List<ResourceLocation>, baseExtension: Int = 3, decorators: List<TemplateDecorator<TreeContext>> = emptyList()) =
        TemplateFeature.Config(templates, TemplatePlacements.tree(), PasteConfig(baseExtension, false, true, false, false), DecoratorConfig(decorators, ImmutableMap.of()))

    private fun makeChance(vararg entries: ChanceFeature.Entry) =
        ChanceFeature.Config(ImmutableList.copyOf(entries))

    private fun makeChanceEntry(feature: Holder<PlacedFeature>, chance: Float, vararg modifiers: ChanceModifier) =
        ChanceFeature.Entry(feature, chance, ImmutableList.copyOf(modifiers))

    private fun makeRandom(defaultFeature: Holder<PlacedFeature>, entries: List<WeightedPlacedFeature>) =
        RandomFeatureConfiguration(entries, defaultFeature)

    private fun makeWeighted(weight: Float, feature: Holder<PlacedFeature>) =
        WeightedPlacedFeature(feature, weight)

    private fun makePatch(state: Block, tries: Int) =
        FeatureUtils.simpleRandomPatchConfiguration(tries, PlacementUtils.onlyWhenEmpty(Feature.SIMPLE_BLOCK, SimpleBlockConfiguration(BlockStateProvider.simple(state.defaultBlockState()))))

    private fun <FC : FeatureConfiguration, F : Feature<FC>> makeInlined(feature: F, featureConfiguration: FC): Holder<PlacedFeature> =
        Holder.direct(PlacedFeature(Holder.direct(ConfiguredFeature(feature, featureConfiguration)), emptyList()))

    protected fun createKey(name: String): ResourceKey<ConfiguredFeature<*, *>> =
        ResourceKey.create(Registries.CONFIGURED_FEATURE, RTFCommon.location(name))
}