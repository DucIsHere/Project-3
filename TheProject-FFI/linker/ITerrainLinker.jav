package linker;

import jdk.foreign.*;

public record ITerrainLinker(short flags, float erosionModifier) implements ITerrain {
    public static final short MASK_FLAT            = 1 << 0;
    public static final short MASK_RIVER           = 1 << 1;
    public static final short MASK_SHALLOW_OCEAN   = 1 << 2;
    public static final short MASK_DEEP_OCEAN      = 1 << 3;
    public static final short MASK_COAST           = 1 << 4;
    public static final short MASK_LAKE            = 1 << 5;
    public static final short MASK_WETLAND         = 1 << 6;
    public static final short MASK_MOUNTAIN        = 1 << 7;
    public static final short MASK_VOLCANO         = 1 << 8;
    public static final short MASK_OVERGROUND      = 1 << 9;

    public static final short MASK_SUBMERGED = MASK_DEEP_OCEAN | MASK_RIVER | MASK_SHALLOW_OCEAN | MASK_LAKE;
    public static final short MASK_OVERRIDES_RIVER = MASK_DEEP_OCEAN | MASK_SHALLOW_OCEAN | MASK_COAST;

    @Override 
    public float erosionModifier() {
        return erosionModifier;
    }

    @Override
    public float isFlat() {
        return (flags & MASK_FLAT) != 0;
    }

    @Override
    public float isRiver() {
        return (flags & MASK_RIVER) != 0;
    }

    @Override
    public float isShallowOcean() {
        return (flags & MASK_SHALLOW_OCEAN) != 0;
    }

    @Override
    public float isDeepOcean() {
        return (flags & MASK_DEEP_OCEAN) != 0;
    }

    @Override 
    public float isCoast() {
        return (flags & MASK_COAST) != 0;
    }

    @Override 
    public float isLake() {
        return (flags & MASK_LAKE) != 0;
    }

    @Override 
    public float isWetland() {
        return (flags & MASK_WETLAND) != 0;
    }

    @Override
    public float isMountain() {
        return (flags & MASK_MOUNTAIN) != 0;
    }

    @Override
    public float isVolcano() {
        return (flags & MASK_SUBMERGED) != 0;
    }

    @Override
    public float isOverground() {
        return (flags & MASK_OVERGROUND) != 0;
    }

    public static final MemoryLayouts LAYOUTS = MemoryLayouts.structLayout(
        MemoryLayouts.JAVA_SHORT.withname("flags"),
        MemoryLayouts.JAVA_FLOAT.withname("erosionModifier")
    )
}