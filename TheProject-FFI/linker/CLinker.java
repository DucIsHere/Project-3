package linker;

import java.incubator.foreign.Arena;
import java.incubator.foreign.FunctionDescriptor;
import java.incubator.foreign.Linker;
import java.incubator.foreign.MemoryLayouts;
import java.incubator.foreign.MemorySegment;
import java.incubator.foreign.ResorceScope;
import java.incubator.foreign.SymbolLokup;
import java.lang.invoke.MethodHandle;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;

public class CLinker {
    private static MemorySegment nativeConfigContext = null;
    private static MethodHandle initHeightCore64BitHandle = null;
    private static MethodHandle nativeSettingsHandle = null;
    private static MethodHandle setNativeGeneralHandle = null;
    private static MethodHandle setNativeBiomeHandle = null;

    static {
        try {
            String os = System.getProperty("os.name").toLowerCase();
            String libName = os.contains("win") ? "core_engine.dll" : "libcore_engine.so";

            String libPathStr = System.getProperty("java.library.path", ".");
            Path libPath = Paths.get(libPathStr).resolve(libName).toAbsolutePath();

            System.out.println("[CLinker] Loading native core from: " + libPath);

            // JDK 17 dùng ResourceScope thay cho Arena trực tiếp
            ResourceScope scope = ResourceScope.newImplicitScope();
            SymbolLookup cLibraryLookup = SymbolLookup.libraryLookup(libPath, scope);
            Linker linker = Linker.nativeLinker();

            // Descriptor hệ JDK 17 sử dụng MemoryLayouts
            FunctionDescriptor initHeightDesc = FunctionDescriptor.of(
                    MemoryLayouts.ADDRESS,           // Ra: Địa chỉ con trỏ Struct
                    MemoryLayouts.JAVA_INT,          // Vào: seed
                    MemoryLayouts.JAVA_INT,          // Vào: worldHeight thô
                    MemoryLayouts.JAVA_INT,          // Vào: worldDepth thô
                    MemoryLayouts.JAVA_INT           // Vào: seaLevel thô
            );

            Optional<MemorySegment> symbol = cLibraryLookup.lookup("init_height_core_64bit");
            if (symbol.isPresent()) {
                initHeightCore64BitHandle = linker.downcallHandle(symbol.get(), initHeightDesc);
                System.out.println("[CLinker] Kết nối thành công phễu 64-bit trên JDK 17!");
            } else {
                throw new UnsatisfiedLinkError("Không tìm thấy hàm 'init_height_core_64bit'!");
            }

        } catch (Throwable e) {
            System.err.println("[CLinker] Sập khởi tạo Linker JDK 17!");
            e.printStackTrace();
        }
    }

    public static void loadHeightSettingsToNative(int seed, int worldHeight, int worldDepth, int seaLevel) {
        if (initHeightCore64BitHandle == null) return;
        try {
            nativeConfigContext = (MemorySegment) initHeightCore64BitHandle.invokeExact(
                    seed, worldHeight, worldDepth, seaLevel
            );
            System.out.println("[CLinker] Đã nạp móng cao độ 64-bit Off-Heap!");
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    public static MemorySegment getNativeConfigContext() {
        return nativeConfigContext;
    }

    static {
        System.loadLibrary("core_engine");
        
        SymbolLookup lookup = SymbolLookup.loaderLookup();
        Linker linker = Linker.nativeLinker();

        // 1. Ánh xạ hàm tạo Struct tổng
        nativeSettingsHandle = linker.downcallHandle(
            lookup.lookup("native_settings").get(),
            FunctionDescriptor.of(ValueLayout.ADDRESS)
        );

        // 2. Ánh xạ hàm nạp cấu hình chung
        setNativeGeneralHandle = linker.downcallHandle(
            lookup.lookup("set_native_general").get(),
            FunctionDescriptor.ofVoid(
                ValueLayout.ADDRESS,
                ValueLayout.JAVA_INT, ValueLayout.JAVA_INT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_BYTE // bool gửi xuống C nhận dạng 1 byte
            )
        );

        // 3. Ánh xạ hàm nạp Biome (Bắn chuỗi tham số phẳng)
        setNativeBiomeHandle = linker.downcallHandle(
            lookup.lookup("set_native_biome").get(),
            FunctionDescriptor.ofVoid(
                ValueLayout.ADDRESS, ValueLayout.JAVA_INT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT, ValueLayout.JAVA_FLOAT,
                ValueLayout.JAVA_INT
            )
        );
    }

    public static MemoryAddress injectToNative(main.java.net.prj3.data.worldgen.preset.settings.TerrainSettings javaSettings) {
        try {
            MemoryAddress settingsPtr = (MemoryAddress) nativeSettingsHandle.invokeExact();

            var g = javaSettings.general;
            nativeSettingsHandle.invokeExact(settingsPtr, g.terrainOffSet, g.terrainRegionSize, g.globalVerticalScale, g.globalHorizontalScale, (byte)(g.fancymMountain ? 1 : 0));

            push(settingsPtr, 0, javaSettings.steppe);
            push(settingsPtr, 1, javaSettings.plains);
            push(settingsPtr, 2, javaSettings.hills);
            push(settingsPtr, 3, javaSettings.dales);
            push(settingsPtr, 4, javaSettings.plateau);
            push(settingsPtr, 5, javaSettings.badlands);
            push(settingsPtr, 6, javaSettings.torridonian);
            push(settingsPtr, 7, javaSettings.mountains);
            push(settingsPtr, 8, javaSettings.volcano);
            push(settingsPtr, 9, javaSettings.blendLow);
            push(settingsPtr, 10, javaSettings.blendMid);
            push(settingsPtr, 11, javaSettings.blendHigh);

            return settingsPtr;
        } catch (Throwable t) {
            t.printStackTrace();
            return MemoryAddress.NULL;
        }
    }

    private static void push(MemoryAddress ptr, int id, main.java.net.prj3.data.worldgen.preset.settings.TerrainSettings.Terrain t) throws Throwable {
        setNativeBiomeHandle.invokeExact(
            ptr, id,
            t.weight, t.baseScale, t.verticalScale, t.horizontalScale, t.baseHeight, t.valleyDepth, t.valleyWidth,
            t.mountainHeightScale, t.mountainScale, t.plateauHeight, t.coastSharpness, t.valleyErosion, t.valleyWeirdness,
            t.riverErosion, t.riverWeirdness, t.lakeWeirdness, t.beachNoiseScale, t.beachHeight,
            t.aquiferDepthOffset
        );
    }
}