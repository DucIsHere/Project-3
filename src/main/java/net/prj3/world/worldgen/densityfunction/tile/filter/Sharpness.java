package net.prj3.world.worldgen.densityfunction.tile.filter;

import net.prj3.world.worldgen.cell.Cell;
import net.prj3.world.worldgen.densityfunction.tile.Size;
import net.prj3.world.worldgen.biome.Sharpness; // Enum 7 mức của bạn

public record Sharpness(Sharpness sharpness) implements Filter {

    @Override
    public void apply(Filterable map, int seedX, int seedZ, int iterations) {
        float factor = sharpness.mid();
        
        // Mức 1: OFF - Ngắt ngay lập tức, cứu RAM 16GB và CPU
        if (factor == 0.0F) return;

        Size size = map.getBlockSize();
        int total = size.total();

        for (int x = 0; x < total; x++) {
            for (int z = 0; z < total; z++) {
                Cell cell = map.getCellRaw(x, z);
                
                // Mức 2-3: LOWEST/LOW (Factor âm) - Làm mượt nhẹ
                if (factor < 0) {
                    cell.height *= (1.0F + factor * 0.4F);
                } 
                // Mức 4-7: AVERAGE -> EXTREMNT (Factor dương)
                else {
                    // Công thức lũy thừa bảo toàn dấu (sign-preserving power)
                    float base = cell.height;
                    cell.height = (float) (Math.pow(Math.abs(base), 1.0 - factor * 0.9) * Math.signum(base));
                }
            }
        }
    }
}