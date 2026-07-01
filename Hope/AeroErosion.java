public void applyAeroStandard(Filterable map, Noise windX, Noise windZ, float strength) {
    // Duyệt qua từng Cell
    for (int x = 1; x < total - 1; x++) {
        for (int z = 1; z < total - 1; z++) {
            Cell cell = map.getCellRaw(x, z);
            
            // 1. Tính toán Pháp tuyến bề mặt (Surface Normal)
            float dx = (map.getCellRaw(x + 1, z).height - map.getCellRaw(x - 1, z).height) * 0.5f;
            float dz = (map.getCellRaw(x, z + 1).height - map.getCellRaw(x, z - 1).height) * 0.5f;
            // Vector pháp tuyến N = (-dx, 1, -dz) -> đơn vị hóa
            float len = (float) Math.sqrt(dx*dx + 1 + dz*dz);
            float nx = -dx / len;
            float nz = -dz / len;

            // 2. Lấy Vector Gió tại điểm đó
            float wx = windX.compute(worldX + x, worldZ + z, 0);
            float wz = windZ.compute(worldX + x, worldZ + z, 0);

            // 3. Tính độ phơi sáng (Exposure) = Tích vô hướng (Dot Product)
            // Nếu Dot Product > 0: Gió thổi vào bề mặt
            float exposure = Math.max(0, (wx * nx + wz * nz));

            // 4. Bào mòn dựa trên độ nhám và áp lực gió
            float erosion = exposure * strength * cell.terrain.erosionModifier();
            cell.height -= erosion;
            
            // 5. Trầm tích (Aeolian deposition): Gió mang bụi đi và rơi ở vùng khuất gió (Exposure gần 0)
            if (exposure < 0.1f) {
                cell.height += erosion * 0.5f; // Bồi đắp nhẹ ở vùng khuất
            }
        }
    }
}
