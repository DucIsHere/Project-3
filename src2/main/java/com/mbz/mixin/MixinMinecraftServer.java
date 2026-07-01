package com.mbz.mixin;

import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

import net.minecraft.server.MinecraftServer;
import net.minecraft.server.level.ServerLevel;
import net.minecraft.world.level.biome.Climate;
import net.minecraft.world.level.levelgen.RandomState;
import net.minecraft.world.level.storage.ServerLevelData;
import com.regenerationforrged.data.worldgen.preset.settings.Preset;
import kotlin.com.mbz.registries.RGFRegistries;

@Mixin(MinecraftServer.class)
class MixinMinecraftServer {

	@Inject(at = @At("HEAD"), method = "setInitialSpawn")
    private static void findSpawnPosition(ServerLevel serverLevel, ServerLevelData serverLevlData, boolean bl, boolean bl2, CallbackInfo callback) {
    serverLevel.registryAccess().lookup(RGFRegistries.PRESET).flatMap(r -> r.get(Preset.KEY)).ifPresent(preset -> {
		
    });
    }
}
