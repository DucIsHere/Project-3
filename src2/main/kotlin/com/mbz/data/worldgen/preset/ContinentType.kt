package com.mbz.data.worldgen.preset

import com.mbz.world.worldgen.GeneratorContext
import com.mbz.world.worldgen.cell.continent.Continent
import com.mbz.world.worldgen.cell.continent.advanced.AdvancedContinentGenerator
import com.mbz.world.worldgen.cell.continent.fancy.FancyContinentGenerator
import com.mbz.world.worldgen.cell.continent.infinite.InfiniteContinentGenerator
import com.mbz.world.worldgen.cell.continent.simple.MultiContinentGenerator
import com.mbz.world.worldgen.cell.continent.simple.SingleContinentGenerator
import com.mbz.world.worldgen.util.Seed
import com.mojang.serialization.Codec
import net.minecraft.util.StringRepresentable

enum class ContinentType : StringRepresentable {
    MULTI {
        override fun create(seed: Seed, context: GeneratorContext) = MultiContinentGenerator(seed, context)
    },
    SINGLE {
        override fun create(seed: Seed, context: GeneratorContext) = SingleContinentGenerator(seed, context)
    },
    MULTI_IMPROVED {
        override fun create(seed: Seed, context: GeneratorContext) = AdvancedContinentGenerator(seed, context)
    },
    EXPERIMENTAL {
        override fun create(seed: Seed, context: GeneratorContext) = FancyContinentGenerator(seed, context)
    },
    INFINITE {
        override fun create(seed: Seed, context: GeneratorContext) = InfiniteContinentGenerator(context)
    };

    abstract fun create(seed: Seed, context: GeneratorContext): Continent

    override fun getSerializedName(): String = this.name

    companion object {
        @JvmField
        val CODEC: Codec<ContinentType> = StringRepresentable.fromEnum { values() }
    }
}
