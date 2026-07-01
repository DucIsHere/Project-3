package com.mbz.data.worldgen.preset.settings

import com.google.common.collect.ImmutableMap
import com.mojang.serialization.Codec
import com.mojang.serialization.codecs.RecordCodecBuilder
import net.minecraft.core.registries.Registries
import net.minecraft.resources.ResourceKey
import net.minecraft.world.level.levelgen.structure.StructureSet

class StructureSettings {
    var entries: MutableMap<ResourceKey<StructureSet>, StructureSetEntry>

    // Constructor chính
    constructor(entries: Map<ResourceKey<StructureSet>, StructureSetEntry>) {
        this.entries = HashMap(entries)
    }

    // Constructor mặc định (gọi khi không có tham số)
    constructor() : this(ImmutableMap.of())

    // Hàm copy sử dụng mapValues của Kotlin cho gọn nhưng vẫn tường minh
    fun copy(): StructureSettings {
        val newEntries = entries.mapValues { it.value.copy() }
        return StructureSettings(HashMap(newEntries))
    }

    // --- Lớp con StructureSetEntry ---
    class StructureSetEntry {
        var spacing: Int
        var separation: Int
        var salt: Int
        var disabled: Boolean

        constructor(spacing: Int, separation: Int, salt: Int, disabled: Boolean) {
            this.spacing = spacing
            this.separation = separation
            this.salt = salt
            this.disabled = disabled
        }

        fun copy(): StructureSetEntry {
            return StructureSetEntry(this.spacing, this.separation, this.salt, this.disabled)
        }

        companion object {
            @JvmField
            val CODEC: Codec<StructureSetEntry> = RecordCodecBuilder.create { instance ->
                instance.group(
                    Codec.INT.fieldOf("spacing").forGetter { it.spacing },
                    Codec.INT.fieldOf("separation").forGetter { it.separation },
                    Codec.INT.fieldOf("salt").forGetter { it.salt },
                    Codec.BOOL.fieldOf("disabled").forGetter { it.disabled }
                ).apply(instance, ::StructureSetEntry)
            }
        }
    }

    companion object {
        @JvmField
        val CODEC: Codec<StructureSettings> = RecordCodecBuilder.create { instance ->
            instance.group(
                Codec.unboundedMap(ResourceKey.codec(Registries.STRUCTURE_SET), StructureSetEntry.CODEC)
                    .fieldOf("structures")
                    .forGetter { it.entries }
            ).apply(instance, ::StructureSettings)
        }
    }
}
