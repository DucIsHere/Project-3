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
}