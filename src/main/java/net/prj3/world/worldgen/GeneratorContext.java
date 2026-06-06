ppackage com.regenerationforrged.world.worldgen;

import org.jetbrains.annotations.Nullable;

import java.util.function.IntFunction;

import net.minecraft.core.HolderGetter;

import net.prj3.data.worldgen.preset.settings.Preset;
import net.prj3.world.worldgen.cell.heightmap.Heightmap;
import net.prj3.world.worldgen.cell.heightmap.Levels;
import net.prj3.world.worldgen.cell.heightmap.WorldLookup;
import net.prj3.world.worldgen.densityfunction.tile.Size;
import net.prj3.world.worldgen.densityfunction.tile.TileCache;
import net.prj3.world.worldgen.densityfunction.tile.generation.TileGenerator;
import net.prj3.world.worldgen.noise.module.Noise;
import net.prj3.world.worldgen.noise.module.Noises;
import net.prj3.world.worldgen.util.Seed;
import net.prj3.world.worldgen.densityfunction.tile.filter.AdvancedSoilFluction;
import net.prj3.world.worldgen.densityfunction.tile.filter.AdvancedSubsurfaceFlow;
import net.prj3.world.worldgen.densityfunction.tile.filter.AeroErosion;
import net.prj3.world.worldgen.densityfunction.tile.filter.AquiferFilter;
import net.prj3.world.worldgen.densityfunction.tile.filter.GlacialErosion;
import net.prj3.world.worldgen.densityfunction.tile.filter.HydarulicErosion;
import net.prj3.world.worldgen.densityfunction.tile.filter.ForceErosion;
import net.prj3.world.worldgen.densityfunction.tile.filter.ThermalErosion;
import net.prj3.world.worldgen.densityfunction.tile.filter.LandSlide;

public class GeneratorContext {
    public Seed seed;
    public Levels levels;
    public Preset preset;
    public HolderGetter<Noise> noiseLookup;
    public TileGenerator generator;
    @Nullable
    public TileCache cache;
    public WorldLookup lookup;
    // CLASS GEOLOGY NOISES
    public Noise windX;
    public Noise windZ;
    public Noise faultNoise;
    public Noise upliftNoise;
    public Noise stabilityNoise;
    // FACTORY METHOS (LAZY-LOADING)
    public final IntFunction<ForceErosion> forceErosionFactory;
    public final IntFunction<AeroErosion> aeroErosionFactory;
    public final IntFunction<GlacialErosion> glacialErosionFactory;
    public final IntFunction<Erosion> hydraulicErosionFactory;
    public final IntFunction<ThermalErosion> thermalErosionFactory;
    public final IntFunction<LandSlide> landSlideFactory;
    public final IntFunction<AdvancedSoilFluction> soilFunctionFactory;
    public final IntFunction<CoastalErosion> coastalErosionFactory;
    public final IntFunction<AdvancedSubsurfaceFlow> advancedSubsurfaceFlowFactory;
    public final IntFunction<PhysicalSnowAvalanche> physicalSnowAvalancheFactory;
    public final IntFunction<AquiferFilter> aquiferFilterFactory;
    public final IntFunction<Attrition> attritionFactory;
    
    public GeneratorContext(Preset preset, HolderGetter<Noise> noiseLookup, int seed, int tileSize, int tileBorder, int batchCount, @Nullable TileCache cache) {
        this.preset = preset;
        this.noiseLookup = noiseLookup;
        this.seed = new Seed(seed);
        this.levels = new Levels(preset.world().properties.terrainScaler(), preset.world().properties.seaLevel);

        this.windX = Noises.perlin(this.seed.next(), 1500, 1);
        this.windZ = Noises.perlin(this.seed.next(), 1500, 1);

        Noise faultNoise = Noises.perlin(this.seed.next(), 1500, 2);
        this.faultNoise = Noises.warpPerlin(baseFault, this.seed.next(), 300, 2, 200.0F);

        Noise baseUplift = Noises.perlin(this.seed.next(), 2000, 3);
        this.upliftNoise = Noises.warpPerlin(baseUplift, this.seed.next(), 400, 3, 350.0F);

        
        // 1. Định nghĩa CoastalErosionFactory
        this.coastalErosionFactory = (size) -> new CoastalErosion(
            size,
            this.levels.water,
            this.windX, 
            0.75f, 
            0.65f, 
            Modifier.range(this.levels.water - 8, this.levels.water + 35)
        );

        // 2. Định nghĩa các Factory khác để khớp với file WorldFilters của bạn
        this.forceErosionFactory = (size) -> new ForceErosion(
            size, this.faultNoise, 15.0F, 0.8F, 3.0F, 
            this.upliftNoise, 35.0F, 0.6F, Modifier.DEFAULT
        );

        this.aeroErosionFactory = (size) -> new AeroErosion(
            size, this.windX, this.windZ, 0.015F, 0.3F,
            this.levels.water + 1.0F,
            Modifier.range(this.levels.ground, this.levels.ground + 120).invert()
        );

        this.hydraulicErosionFactory = (size) -> new Erosion(
            size, this.levels, this.preset.filters().erosion // Giả định constructor của Erosion
        );

        this.thermalErosionFactory = (size) -> 
            new ThermalErosion(size, 0.15F, 0.5F, Modifier.DEFAULT);

        this.landSlideFactory = (size) -> 
            new LandSlide(size, this.stabilityNoise, 0.25F, Modifier.DEFAULT);

        this.glacialErosionFactory = (size) -> 
            new GlacialErosion(size, this.preset.filters().glacial, this.seed);

        this.physicalSnowAvalancheFactory = (size) -> new PhysicalSnowAvalanche(size, this.preset.filters().snowAvalanche, this.seed);

        this.advancedSoilFluctionFactory = (size) -> new AdvancedSoilFluction(size, this.preset.filters().soilFluction, this.seed);

        this.advancedSubsurfaceFlowFactory = (size) -> new AdvancedSubsurfaceFlow(size, this.preset.filters().advancedSubsurfaceFlow, this.seed);

        this.aquiferFilterFactory = (size) -> new AquiferFilter(size, this.preset.filters().aquifer, this.seed);

        this.attritionFactory = (size) -> new Attrition.factory(this);

        this.generator = new TileGenerator(heightMap.make(this), new worldFilters(this), tileSize, tileBorder, batchCount);
        this.cache = cache;
        this.lookup = new WorldLookup(this);
        this.stabilityNoise = Noises.cellular(this.seed.next(), 250);
    }

    public static GeneratorContext makeCached(Preset preset, HolderGetter<Noise> noiseLookup, int seed, int tileSize, int batchCount, boolean queue) {
    	GeneratorContext ctx = makeUncached(preset, noiseLookup, seed, tileSize, Math.min(2, Math.max(1, preset.filters().erosion.dropletLifetime / 16)), batchCount);
    	ctx.cache = new TileCache(tileSize, queue, ctx.generator);
    	ctx.lookup = new WorldLookup(ctx);
    	return ctx;
    }
    
    public static GeneratorContext makeUncached(Preset preset, HolderGetter<Noise> noiseLookup, int seed, int tileSize, int tileBorder, int batchCount) {
    	return new GeneratorContext(preset, noiseLookup, seed, tileSize, tileBorder, batchCount, null);
    }
}
