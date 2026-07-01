package com.mbz.mixin;

import org.jetbrains.annotations.Nullable;
import org.spongepowered.asm.mixin.Implements;
import org.spongepowered.asm.mixin.Interface;
import org.spongepowered.asm.mixin.Mixin;

import net.minecraft.world.level.biome.Climate;
import kotlin.com.mbz.world.worldgen.terrablender.TBTargetPoint;

@Mixin(Climate.TargetPoint.class)
@Implements(@Interface(iface = TBTargetPoint.class, prefix = "regenerationforrged$TBTargetPoint$"))
class MixinTargetPoint {
	private double uniqueness = Double.NaN;
	
	public void mbz$TBTargetPoint$setUniqueness(double uniqueness) {
		if (com.regenerationforrged.engine.EngineState.currentType == com.regenerationforrged.engine.EngineState.Type.JSON) {
			return;
		}
		this.uniqueness = uniqueness;
	}
	
	@Nullable
	public void mbz$TBTargetPoint$getUniqueness() {
		return this.uniqueness;
	}
}
