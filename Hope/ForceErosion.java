// Trong WorldFilters hoặc một Populator chuyên biệt
public void applyForceStandard(Filterable map, int seed) {
    // Sử dụng Ridge Noise với tần số cao để tạo vết nứt
    Noise forceNoise = Noises.ridge(seed, 150, 3); 
    
    // Warp nó để tạo sự méo mó địa chất (Tectonic Warp)
    forceNoise = Noises.warpPerlin(forceNoise, seed + 1, 80, 2, 30.0F);

    for (int x = 0; x < total; x++) {
        for (int z = 0; z < total; z++) {
            Cell cell = map.getCellRaw(x, z);
            float f = forceNoise.compute(worldX + x, worldZ + z, 0);
            
            // "Thresholding" để chỉ lấy các vết đứt gãy hẹp và sâu
            if (f > 0.85F) {
                float intensity = (f - 0.85F) / 0.15F;
                
                // Hiệu ứng phân tầng (Terracing)
                // Ép độ cao vào các mức cố định để tạo cảm giác các lớp đá bị gãy
                float depth = intensity * 0.15F;
                float tieredDepth = Math.round(depth * 10) / 10.0F; 
                
                cell.height -= tieredDepth;
                cell.terrain = TerrianType.MOUNTAIN; // Ép kiểu terrain về đá cứng
                cell.erosionMask = true; // Không cho các filter khác làm mượt vùng này
            }
        }
    }
}
