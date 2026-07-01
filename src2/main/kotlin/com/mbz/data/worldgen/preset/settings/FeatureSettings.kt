package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

data class FeatureSettings(
    var trees: Boolean = true,
    var lakes: Boolean = true,
    var caves: Boolean = true,
    var ores: Boolean = true
) {
    companion object {
        @JvmField
        val CODEC: Codec<FeatureSettings> = RecordCodecBuilder.create { i ->
            i.group(
                Codec.BOOL.fieldOf("trees").forGetter { it.trees },
                Codec.BOOL.fieldOf("lakes").forGetter { it.lakes },
                Codec.BOOL.fieldOf("caves").forGetter { it.caves },
                Codec.BOOL.fieldOf("ores").forGetter { it.ores }
            ).apply(i, ::FeatureSettings)
        }
    }
}
