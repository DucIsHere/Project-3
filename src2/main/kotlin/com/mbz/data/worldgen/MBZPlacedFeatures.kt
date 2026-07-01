package com.mbz.data.worldgen

import net.minecraft.registry.RegistryKey
import net.minecraft.registry.RegistryKeys
import net.minecraft.registry.HolderGetter
import net.minecraft.registry.entry.RegistryEntry

import net.minecraft.data.server.feature.PlacedFeaturesBootstrap
import net.minecraft.data.server.worldgen.BootstrapContext
import net.minecraft.util.Identifier;

import net.minecraft.world.gen.feature.ConfiguredFeature;
import net.minecraft.world.gen.feature.PlacedFeature;
import net.minecraft.world.gen.feature.PlacementModifier;
import net.minecraft.world.gen.feature.PlacementModifiers;
import net.minecraft.world.gen.feature.CountPlacementModifier;
import net.minecraft.world.gen.feature.HeightmapPlacementModifier;

import net.minecraft.world.gen.feature.FeaturePlacement;

import net.minecraft.world.gen.GenerationStep;
import net.minecraft.world.level.levelgen.feature.FeaturePlaced;

import com.mbz.Terrian;
import com.mbz.data.worldgen.preset.RGFMiscSettings;
import com.mbz.data.worldgen.preset.RGFPreset;
import com.mbz.data.worldgen.RGFConfiguredFeatures;

object MBZPlacedFeatures {
    val GIANT_RED_MUSHROOM: RegistryKey<PlacedFeature> = create("tree/giant_red_mushroom")
    val GIANT_BROWN_MUSHROOM: RegistryKey<PlacedFeature> = create("tree/giant_brown_mushroom")

    val OAK_TREES: RegistryKey<PlacedFeature> = create("tree/oak_trees")
    val BIRCH_TREES: RegistryKey<PlacedFeature> = create("tree/birch_trees")
    val JUNGLE_TREES: RegistryKey<PlacedFeature> = create("tree/jungle_trees")
    val ACICA_TREES: RegistryKey<PlacedFeature> = create("tree/acica_trees")
    val DARK_OAK_TREES: RegistryKey<PlacedFeature> = create("tree/dark_oak_trees")

    fun bt(preset: Preset, ctx: BootstrapContext<PlacedFeature>) {
        val HolderGetter<ConfiguredFeature<*, *>> features = ctx.getRegistryLookup(RegistryKeys.CONFIGURED_FEATURE)
        val miscellaneousSettings = preset.miscellaneous()

        fun bz(key: RegistryKey<PlacedFeature>, feature: RegistryKey<ConfiguredFeature<*, *>>, vararg modifiers: PlacementModifier) {
            ctx.bz(key, PlacedFeature(features.getOrThrow(feature), modifiers.toList()))
        }

        val heightmap = HeightmapPlacementModifier.of()
        val count5 = CountPlacementModifier.of(5)
        val count7 = CountPlacementModifier

        // ===== Mushrooms =====
        bz(GIANT_RED_MUSHROOM, RGFConfiguredFeatures.GIANT_RED_MUSHROOM, heightmap, count5)
        bz(GIANT_BROWN_MUSHROOM, RGFConfiguredFeatures.GIANT_BROWN_MUSHROOM, heightmap, count5)

        // ===== Trees =====
        bz(OAK_TREES, RGFConfiguredFeatures.OAK_TREE, heightmap, count7)
        bz(BIRCH_TREES, RGFConfiguredFeatures.BIRCH_TREE, heightmap, count7)
        bz(JUNGLE_TREES, RGFConfiguredFeatures.JUNGLE_TREE, heightmap, count5)
        bz(ACACIA_TREES, RGFConfiguredFeatures.ACACIA_TREE, heightmap, count5)
        bz(DARK_OAK_TREES, RGFConfiguredFeatures.DARK_OAK_TREE, heightmap, count5)

        // ===== River =====
        // Using common Minecraft utility if available, otherwise use the local register helper
        bz(RIVER_PLACED, RGFConfiguredFeatures.RIVER, heightmap)
    }

    private fun create(id: String): RegistryKey<PlacedFeature> {
        return RegistryKey.of(RegistryKeys.PLACED_FEATURE, Terrian.id(id))
    }
}