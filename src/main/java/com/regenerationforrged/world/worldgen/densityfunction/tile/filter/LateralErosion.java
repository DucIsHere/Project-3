package com.regenerationforrged.world.worldgen.densityfunction.tile.filter;

import com.regenerationforrged.data.worldgen.preset.settings.FilterSettings;
import com.regenerationforrged.world.worldgen.GeneratorContext;
import com.regenerationforrged.world.worldgen.cell.Cell;
import com.regenerationforrged.world.worldgen.densityfunction.tile.Tile;
import com.regenerationforrged.world.worldgen.densityfunction.tile.Size;
import com.regenerationforrged.world.worldgen.noise.NoiseUtil;
import com.regenerationforrged.world.worldgen.util.FastRandom;

public class LateralErosion implements Filter {
    private final int mapSize;
    private final int seed;
    private final Modifier modifier;

    private static final float RHO_W = 1000.0F;
    private static final float G = 9.8F;
    
    private final float CoeffA; // 1.0F -> 2.0F
    
    public LaterErosion(int mapSize, int seed, Modifier modifier, float CoeffA) {
        this.mapSize = mapSize;
        this.seed = seed;
        this.modifier = modifier;
        this.CoeffA = CoeffA;
    }

    @Override
    public int getSize() {
        return this.mapSize;
    }

    public void apply(Filterable map, regionX, regionZ, int iterations) {
        final Cell[] cell = map.getBacking();
        final Size size = map.getBlockSize();

        final FastRandom random = new FastRandom(seed);
    }
    
}