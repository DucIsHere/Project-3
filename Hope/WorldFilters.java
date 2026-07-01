package com.regenerationforrged.world.worldgen;

import com.regenerationforrged.data.worldgen.preset.settings.FilterSettings;
import com.regenerationforrged.world.worldgen.densityfunction.tile.Tile;

// Import toàn bộ các bộ lọc (Đã bao gồm cấu trúc IntFunction mới)
import com.regenerationforrged.world.worldgen.densityfunction.tile.filter.*;

public class WorldFilters {
    // --- BỘ LỌC CƠ BẢN (Không cần Cache phức tạp) ---
    private final Smoothing smoothing;
    private final Steepness steepness;
    private final BeachDetect beach;
    private final NoiseCorrection corrections;
    private final FilterSettings settings;
    
    // --- HỆ THỐNG CACHE ĐỊA CHẤT (Memory-Safe & Zero-Allocation) ---
    private final WorldErosion<ForceErosion> forceErosion;
    private final WorldErosion<AeroErosion> aeroErosion;
    private final WorldErosion<GlacialErosionFull> glacialErosion;
    private final WorldErosion<Erosion> hydraulicErosion;
    private final WorldErosion<ThermalErosion> thermalErosion;
    private final WorldErosion<LandSlide> landSlide;

    // --- CẤU HÌNH VÒNG LẶP ---
    private final int erosionIterations;
    private final int smoothingIterations;
    private final int glacialIterations = 40; // Có thể đưa vào preset/JSON sau

    public WorldFilters(GeneratorContext context) {
        this.settings = context.preset.filters();
        
        // 1. Khởi tạo các bộ lọc tiêu chuẩn
        this.beach = BeachDetect.make(context);
        this.smoothing = Smoothing.make(context.preset.filters().smoothing, context.levels);
        this.steepness = Steepness.make(1, 10.0F, context.levels);
        this.corrections = new NoiseCorrection(context.levels);
        
        // 2. KẾT NỐI HỆ THỐNG WORLD EROSION VỚI CÁC FACTORY TỪ CONTEXT
        // Cơ chế: (filterInstance, requestedSize) -> kiểm tra xem Cache có khớp kích thước không
        this.forceErosion = new WorldErosion<>(context.forceErosionFactory, (f, size) -> f.getSize() == size);
        this.aeroErosion = new WorldErosion<>(context.aeroErosionFactory, (a, size) -> a.getSize() == size);
        this.glacialErosion = new WorldErosion<>(context.glacialErosionFactory, (g, size) -> g.getSize() == size);
        this.hydraulicErosion = new WorldErosion<>(context.hydraulicErosionFactory, (e, size) -> e.getSize() == size);
        this.thermalErosion = new WorldErosion<>(context.thermalErosionFactory, (t, size) -> t.getSize() == size);
        this.landSlide = new WorldErosion<>(context.landSlideFactory, (l, size) -> l.getSize() == size);

        // Lấy thông số từ settings
        this.erosionIterations = context.preset.filters().erosion.dropletsPerChunk;
        this.smoothingIterations = context.preset.filters().smoothing.iterations;
    }

    public FilterSettings getSettings() {
        return this.settings;
    }

    public void apply(Tile tile, boolean optionalFilters) {
        int regionX = tile.getX();
        int regionZ = tile.getZ();

        // 1. Chạy các bộ lọc tùy chọn (Hệ thống Xói mòn chính)
        if (optionalFilters) {
            this.applyOptionalFilters(tile, regionX, regionZ);
        }
        
        // 2. Chạy các bộ lọc bắt buộc (Độ dốc, bãi biển)
        this.applyRequiredFilters(tile, regionX, regionZ);
        
        // 3. Sửa lỗi Noise (nếu có)
        if (optionalFilters) {
            this.applyCorrections(tile, regionX, regionZ);
        }
    }

    private void applyRequiredFilters(Filterable map, int seedX, int seedZ) {
        this.steepness.apply(map, seedX, seedZ, 1);
        this.beach.apply(map, seedX, seedZ, 1);
    }

    /**
     * PIPELINE ĐỊA CHẤT CHÍNH (Thứ tự thực thi cực kỳ quan trọng)
     */
    private void applyOptionalFilters(Filterable map, int seedX, int seedZ) {
        // Lấy kích thước tổng (total size) của Tile hiện tại để request Cache
        int size = map.getBlockSize().total();

        // ============================================================
        // BƯỚC 1: KIẾN TẠO NỘI LỰC (FORCE)
        // Tạo nếp gấp núi (Uplift) và hẻm vực đứt gãy (Faulting).
        // ============================================================
        this.forceErosion.get(size).apply(map, seedX, seedZ, 1);
        
        // ============================================================
        // BƯỚC 2: KHÍ QUYỂN (AERO)
        // Gió thổi bay và bào mòn các đỉnh núi nhọn hoắt do noise tạo ra.
        // ============================================================
        this.aeroErosion.get(size).apply(map, seedX, seedZ, 4);
        
        // ============================================================
        // BƯỚC 3: BĂNG HÀ (GLACIAL)
        // Khối lượng băng khổng lồ trượt xuống nạo vét các thung lũng chữ U lớn.
        // Phải chạy trước nước để tạo không gian rộng cho sông ngòi.
        // ============================================================
        this.glacialErosion.get(size).apply(map, seedX, seedZ, this.glacialIterations);

        // ============================================================
        // BƯỚC 4: THỦY VĂN (HYDRAULIC)
        // Nước mưa rơi xuống, hội tụ thành suối và xẻ rãnh chữ V bên trong thung lũng.
        // ============================================================
        this.hydraulicErosion.get(size).apply(map, seedX, seedZ, this.erosionIterations);

        // ============================================================
        // BƯỚC 5: TRỌNG LỰC & SỤP ĐỔ (THERMAL & LANDSLIDE)
        // Đất đá bị nước/băng nạo vét trở nên mất ổn định. 
        // Thermal cân bằng lại góc nghiêng (Talus). Landslide kích hoạt sạt lở mảng lớn.
        // ============================================================
        this.thermalErosion.get(size).apply(map, seedX, seedZ, 2);
        this.landSlide.get(size).apply(map, seedX, seedZ, 1);
        
        // ============================================================
        // BƯỚC 6: LÀM MƯỢT (SMOOTHING)
        // Xóa bỏ các gai nhiễu (spikes) cuối cùng trước khi chuyển sang Voxel.
        // ============================================================
        this.smoothing.apply(map, seedX, seedZ, this.smoothingIterations);
    }

    public void applyCorrections(Filterable map, int seedX, int seedZ) {
        this.corrections.apply(map, seedX, seedZ, 1);
    }
}