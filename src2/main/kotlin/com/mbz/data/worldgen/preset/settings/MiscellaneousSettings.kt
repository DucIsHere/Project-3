package com.mbz.data.worldgen.preset.settings

import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder

class MiscellaneousSettings {
    var smoothLayerDecorator: Boolean
    var strataRegionSize: Int
    var strataDecorator: Boolean
    var oreCompatibleStoneOnly: Boolean
    var erosionDecorator: Boolean
    var plainStoneErosion: Boolean
    var naturalSnowDecorator: Boolean
    var customBiomeFeatures: Boolean
    var vanillaSprings: Boolean
    var vanillaLavaLakes: Boolean
    var vanillaLavaSprings: Boolean
    var mountainBiomeUsage: Float
    var volcanoBiomeUsage: Float

    // Constructor dài dằng dặc đúng chuẩn Java của mày đây
    constructor(
        smoothLayerDecorator: Boolean,
        strataRegionSize: Int,
        strataDecorator: Boolean,
        oreCompatibleStoneOnly: Boolean,
        erosionDecorator: Boolean,
        plainStoneErosion: Boolean,
        naturalSnowDecorator: Boolean,
        customBiomeFeatures: Boolean,
        vanillaSprings: Boolean,
        vanillaLavaLakes: Boolean,
        vanillaLavaSprings: Boolean,
        mountainBiomeUsage: Float,
        volcanoBiomeUsage: Float
    ) {
        this.smoothLayerDecorator = smoothLayerDecorator
        this.strataRegionSize = strataRegionSize
        this.strataDecorator = strataDecorator
        this.oreCompatibleStoneOnly = oreCompatibleStoneOnly
        this.erosionDecorator = erosionDecorator
        this.plainStoneErosion = plainStoneErosion
        this.naturalSnowDecorator = naturalSnowDecorator
        this.customBiomeFeatures = customBiomeFeatures
        this.vanillaSprings = vanillaSprings
        this.vanillaLavaLakes = vanillaLavaLakes
        this.vanillaLavaSprings = vanillaLavaSprings
        this.mountainBiomeUsage = mountainBiomeUsage
        this.volcanoBiomeUsage = volcanoBiomeUsage
    }

    // Hàm copy tường minh để mày kiểm soát logic
    fun copy(): MiscellaneousSettings {
        return MiscellaneousSettings(
            this.smoothLayerDecorator,
            this.strataRegionSize,
            this.strataDecorator,
            this.oreCompatibleStoneOnly,
            this.erosionDecorator,
            this.plainStoneErosion,
            this.naturalSnowDecorator,
            this.customBiomeFeatures,
            this.vanillaSprings,
            this.vanillaLavaLakes,
            this.vanillaLavaSprings,
            this.mountainBiomeUsage,
            this.volcanoBiomeUsage
        )
    }

    companion object {
        @JvmField
        val CODEC: Codec<MiscellaneousSettings> = RecordCodecBuilder.create { i ->
            i.group(
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
            ).apply(i, ::MiscellaneousSettings)
        }
    }
}
