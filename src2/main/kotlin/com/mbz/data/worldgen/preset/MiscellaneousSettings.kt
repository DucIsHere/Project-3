package com.mbz.data.worldgen.preset

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

data class MiscellaneousSettings(
    var smoothLayerDecorator: Boolean,
    var strataRegionSize: Int,
    var oreCompatibleStoneOnly: Boolean,
    var erosionDecorator: Boolean,
    var plainStoneDecorator: Boolean,
    var naturalSnowDecorator: Boolean,
    var customeBiomeFeatures: Boolean,
    var vanlliaSprings: Boolean,
    var vanlliaLavaLakes: Boolean,
    var vanlliaLavaSprings: Boolean,
    var mountainBiomeUsage: Float,
    var volcanoBiomeUsage: Float
) {
    companion object {
        val CODEC: Codec<MiscellaneousSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                Codec.BOOL.fieldOf("smoothLayerDecorator").forGetter { it.smoothLayerDecorator },
                Codec.INT.fieldOf("strataRegionSize").forGetter { it.strataRegionSize },
                Codec.BOOL.fieldOf("strataDecorator").forGetter { it.strataDecorator },
                Codec.BOOL.fieldOf("oreCompatibleStoneOnly").forGetter { it.oreCompatibleStoneOnly },
                Codec.BOOL.fieldOf("erosionDecorator").forGetter { it.erosionDecorator },
                Codec.BOOL.fieldOf("plainStoneErosion").forGetter { it.plainStoneErosion },
                Codec.BOOL.fieldOf("naturalSnowDecorator").forGetter { it.naturalSnowDecorator },
                Codec.BOOL.fieldOf("customBiomeFeatures").forGetter { it.customBiomeFeatures },
                Codec.BOOL.fieldOf("vanillaSprings").forGetter { it.vanillaSprings },
                Codec.BOOL.fieldOf("vanillaLavaLakes").forGetter { it.vanillaLavaLakes },
                Codec.BOOL.fieldOf("vanillaLavaSprings").forGetter { it.vanillaLavaSprings },
                Codec.FLOAT.fieldOf("mountainBiomeUsage").forGetter { it.mountainBiomeUsage },
                Codec.FLOAT.fieldOf("volcanoBiomeUsage").forGetter { it.volcanoBiomeUsage }
            ).apply(instance, ::MiscellaneousSettings)
        }
    }
}