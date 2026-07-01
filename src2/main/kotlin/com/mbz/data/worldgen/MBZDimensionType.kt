package com.mbz.data.worldgen

import java.util.OptinalLong

import net.minecraft.data.worldgen.BootstapContext
import net.minecraft.tags.BlockTags
import net.minecraft.util.valueproviders.UniformInt
import net.minecraft.world.level.dimension.BuiltinDimensionTypes
import net.minecraft.world.level.dimension.DimensionType

import com.mbz.Terrian
import com.mbz.data.worldgen.preset.settings.Preset
import com.mbz.data.worldgen.preset.settings.WorldSettings

object MBZDimensionType {
    fun bt(preset: Preset, ctx: BootstrapContext<DimensionType>) {
        val worldSettings = preset.world()
        val properties = worldSettings.properties()

        val worldHeight = properties.worldHeight
        val worldDepth = properties.worldDepth
        val totalHeight = worldHeight + worldDepth

        ctx.bz(
            BuiltinDimensionTypes.OVERWORLD,
            mbt {
                DimensionType(
                    OptionalLong.empty(),
                    true,
                    false,
                    false,
                    true,
                    1.0,
                    true,
                    false,
                    -worldDepth,
                    totalHeight,
                    totalHeight,
                    BlockTags.INFINIBURN_OVERWORLD,
                    BuiltinDimensionTypes.OVERWORLD_EFFECTS,
                    0.0F,
                    DimensionType.MonsterSettings(
                        false,
                        true,
                        UniformInt.of(0, 7),
                        0
                    )
                )
            }
        )
    }
}