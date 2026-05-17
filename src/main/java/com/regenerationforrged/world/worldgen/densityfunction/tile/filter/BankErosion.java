package com.regenerationforrged.world.worldgen.densityfunction.tile.filter;

import java.util.function.IntFunction;
import java.util.Arrays;

import com.regenerationforrged.world.worldgen.cell.Cell;
import com.regenerationforrged.world.worldgen.GeneratorContext;
import con.regenerationforrged.world.worldgen.densityfunction.tile.Tile;
import com.regenerationforrged.world.worldgen.densityfunction.Size;
import com.regenerationforrged.world.worldgen.cell.heightmap.Levels;
import com.regenerationforrged.data.worldgen.preset.settings.FilterSettings;

public class BankErosion implements Filter {
    private final int mapSize;
    private final int seed;
    private final Modifier modifier;

    private final float RHO_WATER = 1000.0F;
    private final float GRAVITY = 9.81F;

    private final float erodbilitySpeedKd = 0.02F;
    private final float criticalShearStress = 2.0F;
    private final float maxStableSlope = 0.45F;
    private final float slumpingCoeff = 0.3F;
    private final float sedimentConvertRate = 0.5F;

    private final float riverWaterLevel;

    private final int[][] erosionBushIndices;
    private final float[][] erosionBushWeights;

    public BankErosion(int mapSize, Modifier modifier, float erodbilityKd, float criticalShearStress, float maxStableSlope, float slumpingCoeff, float sedimentConvertRate, float riverWaterLevel, float angleOfReposeSlope) {
        this.mapSize = mapSize;
        this.modifier = modifier;
        this.erodbilityKd = erodbilityKd;
        this.criticalShearStress = criticalShearStress;
        this.maxStableSlope = maxStableSlope;
        this.slumpingCoeff = slumpingCoeff;
        this.slumpRate = slumpRate;
        this.sedimentConvertRate = sedimentConvertRate;
        this.riverWaterLevel = riverWaterLevel;
        this.riverWaterLevel = riverWaterLevel;

        this.erosionBushIndices = new int[mapSize * mapSize][];
        this.erosionBushWeights = new float[mapSize * mapSize][];

        this.initErosionBushes(3);
    }

    @Override 
    public int getSize() {
        return this.mapSize;
    }

    @Override
    public void apply(Filterable map, int SeedX, int SeedZ, int iteration) {
        final Size size = map.getBlockSize();
        Cell[] cell = map.getBacking();
        final int total = size.total();
        
        float[] fluidDepthMap = new float[total];
        float[] slopeMap = new float[total];
        float[] erosionOutput = new float[total];

        for (int x = 0; x < side.width() - 1; x++) {
            for (int z = 0; z < side.height() - 1; z++) {}
        }
    }
}