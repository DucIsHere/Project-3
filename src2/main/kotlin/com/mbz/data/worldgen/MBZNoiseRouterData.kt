package com.mbz.data.worldgen

import java.util.stream.Stream

import net.minecraft.core.HolderGetter
import net.minecraft.core.registries.Registries
import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.resources.ResourceKey
import net.minecraft.world.level.dimension.DimensionType
import net.minecraft.world.level.levelgen.DensityFunction
import net.minecraft.world.level.levelgen.DensityFunctions
import net.minecraft.world.level.levelgen.NoiseRouter
import net.minecraft.world.level.levelgen.NoiseRouterData
import net.minecraft.world.level.levelgen.Noises
import net.minecraft.world.level.levelgen.OreVeinifier
import net.minecraft.world.level.levelgen.synth.NormalNoise

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.Preset
import com.mbz.data.worldgen.preset.CaveSettings
import com.mbz.registries.MBZRegistries
import com.mbz.data.worldgen.preset.settings.WorldSettings
import com.mbz.world.worldgen.densityfunction.CellSampler
import com.mbz.world.worldgen.densityfunction.MBZDensityFunctions
import com.mbz.world.worldgen.noise.module.Noise
import com.mbz.world.worldgen.terrablender.TBCompat

object MBZNoiseRouterData {
    val HEIGHT: ResourceKey<DensityFunction> = createKey("height")
    val GRADIENT: ResourceKey<DensityFunction> = createKey("gradient")
    val EROSION: ResourceKey<DensityFunction> = createKey("erosion")
    val SEDIMENT: ResourceKey<DensityFunction> = createKey("sediment")

    private const val SCALER = 128.0F
    private const val UNIT = 1.0 / SCALER

    fun bt(preset: Preset, ctx: BootstapContext<DensityFunction>) {
        val noises = ctx.lookup(RTFRegistries.NOISE)
        val densityFunctions = ctx.lookup(Registries.DENSITY_FUNCTION)
        val noiseParams = ctx.lookup(Registries.NOISE)

        val worldSettings = preset.world()
        val properties = worldSettings.properties
        val caveSettings = preset.caves()

        val worldHeight = properties.worldHeight
        val worldDepth = properties.worldDepth

        ctx.bz(NoiseRouterData.CONTINENTS, MBZDensityFunctions.cell(CellSampler.Field.CONTINENT))
        ctx.bz(NoiseRouterData.EROSION, MBZDensityFunctions.cell(CellSampler.Field.EROSION))
        ctx.bz(NoiseRouterData.RIDGES, MBZDensityFunctions.cell(CellSampler.Field.WEIRDNESS))

        val height = NoiseRouterData.registerAndWrap(ctx, HEIGHT, RTFDensityFunctions.cell(CellSampler.Field.HEIGHT))
        
        val offset = NoiseRouterData.registerAndWrap(
            ctx, NoiseRouterData.OFFSET, DensityFunctions.add(
                DensityFunctions.constant(NoiseRouterData.GLOBAL_OFFSET.toDouble()),
                DensityFunctions.mul(
                    DensityFunctions.add(
                        DensityFunctions.mul(DensityFunctions.constant(-1.0), RTFDensityFunctions.noise(noises.getOrThrow(TerrainTypeNoise.GROUND))),
                        RTFDensityFunctions.clampToNearestUnit(RTFDensityFunctions.conditionalArrayCache(height), properties.terrainScaler())
                    ),
                    DensityFunctions.constant(2.0)
                )
            )
        )

        ctx.bz(NoiseRouterData.DEPTH, DensityFunctions.add(DensityFunctions.yClampedGradient(-worldDepth, worldHeight, yGradientRange(-worldDepth.toFloat()).toDouble(), yGradientRange(worldHeight.toFloat()).toDouble()), offset))
        ctx.bz(NoiseRouterData.BASE_3D_NOISE_OVERWORLD, DensityFunctions.zero())
        ctx.bz(NoiseRouterData.JAGGEDNESS, jaggednessPerformanceHack())

        ctx.bz(NoiseRouterData.NOODLE, noodle(-worldDepth, worldHeight, 1.0f - caveSettings.noodleCaveProbability, densityFunctions, noiseParams))
        ctx.bz(NoiseRouterData.ENTRANCES, probabilityDensity(caveSettings.entranceCaveProbability, NoiseRouterData.entrances(densityFunctions, noiseParams)))
        ctx.bz(NoiseRouterData.SPAGHETTI_2D, probabilityDensity(caveSettings.spaghettiCaveProbability, spaghetti2D(-worldDepth, worldHeight, densityFunctions, noiseParams)))

        ctx.bz(NoiseRouterData.GRADIENT, RTFDensityFunctions.cell(CellSampler.Field.GRADIENT))
        ctx.bz(NoiseRouterData.EROSION, RTFDensityFunctions.cell(CellSampler.Field.HEIGHT_EROSION))
        ctx.bz(NoiseRouterData.SEDIMENT, RTFDensityFunctions.cell(CellSampler.Field.SEDIMENT))
        ctx.bz(TBCompat.uniquenessKey(), RTFDensityFunctions.cell(CellSampler.Field.BIOME_REGION))
    }

    fun overworld(preset: Preset, densityFunctions: HolderGetter<DensityFunction>, noiseParams: HolderGetter<NormalNoise.NoiseParameters>, noises: HolderGetter<Noise>): NoiseRouter {
        val worldSettings = preset.world()
        val properties = worldSettings.properties
        val worldDepth = properties.worldDepth

        val caves = preset.caves()
        val cheeseCaveDepthOffset = caves.cheeseCaveDepthOffset

        val aquiferBarrier = DensityFunctions.noise(noiseParams.getOrThrow(Noises.AQUIFER_BARRIER), 0.5)
        val aquiferFluidLevelFloodedness = DensityFunctions.noise(noiseParams.getOrThrow(Noises.AQUIFER_FLUID_LEVEL_FLOODEDNESS), 0.67)
        val aquiferFluidLevelSpread = DensityFunctions.noise(noiseParams.getOrThrow(Noises.AQUIFER_FLUID_LEVEL_SPREAD), 0.7142857142857143)
        val aquiferLava = DensityFunctions.noise(noiseParams.getOrThrow(Noises.AQUIFER_LAVA))
        
        val temperature = RTFDensityFunctions.cell(CellSampler.Field.TEMPERATURE)
        val vegetation = RTFDensityFunctions.cell(CellSampler.Field.MOISTURE)
        
        val factor = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.FACTOR)
        val depth = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.DEPTH)
        val initialDensity = NoiseRouterData.noiseGradientDensity(DensityFunctions.cache2d(factor), depth)
        val slopedCheese = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.SLOPED_CHEESE)
        
        val entrances = if (caves.entranceCaveProbability > 0.0f) {
            DensityFunctions.min(slopedCheese, DensityFunctions.mul(DensityFunctions.constant(5.0), DensityFunctions.interpolated(NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.ENTRANCES))))
        } else slopedCheese

        val slopedCheeseRange = DensityFunctions.mul(
            DensityFunctions.rangeChoice(slopedCheese, -1000000.0, cheeseCaveDepthOffset.toDouble(), entrances, DensityFunctions.interpolated(slideOverworld(underground(caves.cheeseCaveProbability, densityFunctions, noiseParams, slopedCheese), -worldDepth))),
            DensityFunctions.constant(0.64)
        ).squeeze()

        val finalDensity = DensityFunctions.min(slopedCheeseRange, NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.NOODLE))

        val y = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.Y)
        val minY = OreVeinifier.VeinType.values().minOfOrNull { it.minY } ?: (-DimensionType.MIN_Y * 2)
        val maxY = OreVeinifier.VeinType.values().maxOfOrNull { it.maxY } ?: (-DimensionType.MIN_Y * 2)

        val oreVeininess = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.ORE_VEININESS), 1.5, 1.5), minY, maxY, 0)
        val oreVeinA = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.ORE_VEIN_A), 4.0, 4.0), minY, maxY, 0).abs()
        val oreVeinB = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.ORE_VEIN_B), 4.0, 4.0), minY, maxY, 0).abs()
        val oreVein = DensityFunctions.add(DensityFunctions.constant(-0.08), DensityFunctions.max(oreVeinA, oreVeinB))
        val oreGap = DensityFunctions.noise(noiseParams.getOrThrow(Noises.ORE_GAP))

        return NoiseRouter(
            aquiferBarrier, aquiferFluidLevelFloodedness, aquiferFluidLevelSpread, aquiferLava,
            temperature, vegetation,
            NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.CONTINENTS),
            NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.EROSION),
            depth,
            NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.RIDGES),
            slideOverworld(DensityFunctions.add(initialDensity, DensityFunctions.constant((UNIT * -90).toDouble())).clamp(-64.0, 64.0), -worldDepth),
            finalDensity, oreVeininess, oreVein, oreGap
        )
    }

    private fun underground(cheeseCaveProbability: Float, densityFunctions: HolderGetter<DensityFunction>, noiseParams: HolderGetter<NormalNoise.NoiseParameters>, slopedCheese: DensityFunction): DensityFunction {
        val spaghetti2d = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.SPAGHETTI_2D)
        val spaghettiRoughnessFunction = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.SPAGHETTI_ROUGHNESS_FUNCTION)
        val caveLayerNoise = DensityFunctions.noise(noiseParams.getOrThrow(Noises.CAVE_LAYER), 8.0)
        val caveLayer = DensityFunctions.mul(DensityFunctions.constant(4.0), caveLayerNoise.square())
        val caveCheese = probabilityDensity(cheeseCaveProbability, DensityFunctions.noise(noiseParams.getOrThrow(Noises.CAVE_CHEESE), 0.6666666666666666))
        
        val slopedCaves = DensityFunctions.add(
            DensityFunctions.add(DensityFunctions.constant(0.27), caveCheese).clamp(-1.0, 1.0),
            DensityFunctions.add(DensityFunctions.constant(1.5), DensityFunctions.mul(DensityFunctions.constant(-0.64), slopedCheese)).clamp(0.0, 0.5)
        )
        
        val slopedCaveLayered = DensityFunctions.add(caveLayer, slopedCaves)
        val underground = DensityFunctions.min(
            DensityFunctions.min(slopedCaveLayered, NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.ENTRANCES)),
            DensityFunctions.add(spaghetti2d, spaghettiRoughnessFunction)
        )
        
        val pillars = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.PILLARS)
        val pillarRange = DensityFunctions.rangeChoice(pillars, -1000000.0, 0.03, DensityFunctions.constant(-1000000.0), pillars)
        
        return DensityFunctions.max(underground, pillarRange)
    }

    private fun spaghetti2D(minY: Int, maxY: Int, densityFunctions: HolderGetter<DensityFunction>, noiseParams: HolderGetter<NormalNoise.NoiseParameters>): DensityFunction {
        val modulator = DensityFunctions.noise(noiseParams.getOrThrow(Noises.SPAGHETTI_2D_MODULATOR), 2.0, 1.0)
        val sampler = DensityFunctions.weirdScaledSampler(modulator, noiseParams.getOrThrow(Noises.SPAGHETTI_2D), DensityFunctions.WeirdScaledSampler.RarityValueMapper.TYPE2)
        val elevation = DensityFunctions.mappedNoise(noiseParams.getOrThrow(Noises.SPAGHETTI_2D_ELEVATION), 0.0, (minY / 8).toDouble(), 8.0)
        val thicknessModulator = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.SPAGHETTI_2D_THICKNESS_MODULATOR)
        val elevationGradient = DensityFunctions.add(elevation, DensityFunctions.yClampedGradient(minY, maxY, minY / -8.0, maxY / -8.0)).abs()
        val normal = DensityFunctions.add(elevationGradient, thicknessModulator).cube()
        val weird = DensityFunctions.add(sampler, DensityFunctions.mul(DensityFunctions.constant(0.083), thicknessModulator))
        
        return DensityFunctions.max(weird, normal).clamp(-1.0, 1.0)
    }

    private fun noodle(minY: Int, maxY: Int, threshold: Float, densityFunctions: HolderGetter<DensityFunction>, noiseParams: HolderGetter<NormalNoise.NoiseParameters>): DensityFunction {
        val baseY = minY + 4
        val y = NoiseRouterData.getFunction(densityFunctions, NoiseRouterData.Y)
        val selector = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.NOODLE), 1.0, 1.0), baseY, maxY, -1)
        val thickness = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.mappedNoise(noiseParams.getOrThrow(Noises.NOODLE_THICKNESS), 1.0, 1.0, -0.05, -0.1), baseY, maxY, 0)
        val ridgeA = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.NOODLE_RIDGE_A), 2.6666666666666665, 2.6666666666666665), baseY, maxY, 0)
        val ridgeB = NoiseRouterData.yLimitedInterpolatable(y, DensityFunctions.noise(noiseParams.getOrThrow(Noises.NOODLE_RIDGE_B), 2.6666666666666665, 2.6666666666666665), baseY, maxY, 0)
        val ridge = DensityFunctions.mul(DensityFunctions.constant(1.5), DensityFunctions.max(ridgeA.abs(), ridgeB.abs()))
        return DensityFunctions.rangeChoice(selector, -1000000.0, threshold.toDouble(), DensityFunctions.constant(64.0), DensityFunctions.add(thickness, ridge))
    }

    private fun slideOverworld(function: DensityFunction, minY: Int): DensityFunction {
        return slide(function, minY, 0, 24, (UNIT * 15).toDouble())
    }

    private fun slide(function: DensityFunction, minY: Int, bottomGradientStart: Int, bottomGradientEnd: Int, bottomGradientTarget: Double): DensityFunction {
        val bottomGradient = DensityFunctions.yClampedGradient(minY + bottomGradientStart, minY + bottomGradientEnd, 0.0, 1.0)
        return DensityFunctions.lerp(bottomGradient, bottomGradientTarget, function)
    }

    private fun jaggednessPerformanceHack(): DensityFunction {
        return DensityFunctions.add(DensityFunctions.zero(), DensityFunctions.zero())
    }

    @Deprecated("affects the size of the cave as well")
    private fun probabilityDensity(probability: Float, `function`: DensityFunction): DensityFunction {
        if (probability == 0.0f) {
            return DensityFunctions.constant(1.0)
        }
        return DensityFunctions.add(DensityFunctions.constant((1.0f - probability).toDouble()), `function`)
    }

    private fun yGradientRange(range: Float): Double {
        return (1.0f + (-range / SCALER)).toDouble()
    }

    private fun createKey(string: String): ResourceKey<DensityFunction> {
        return ResourceKey.create(Registries.DENSITY_FUNCTION, Terrian.location(string))
    }
}
