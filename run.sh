#!/bin/bash

# Thoát script ngay lập tức nếu có bất kỳ lệnh nào bị lỗi
set -e

echo "========================================="
echo "BẮT ĐẦU QUY TRÌNH REFACTOR SIÊU DỰ ÁN..."
echo "========================================="

# 1. Tạo bản sao lưu dự phòng (Phao cứu sinh)
echo "[1/4] Đang tạo file backup đề phòng..."
tar -czvf backup_source.tar.gz src/
echo "-> Đã sao lưu thành công vào file 'backup_source.tar.gz'"

# 2. Thay đổi package và import trong các file .java (Chỉ đụng tới com.regenerationforrged)
echo "[2/4] Đang sửa package và import bên trong 1200+ file..."
find src/main/java -name "*.java" -type f -print0 | xargs -0 sed -i -E 's/^(package|import)[[:space:]]+com\.regenerationforrged/\1 net.prj3/g'
echo "-> Đã sửa text xong sạch sẽ!"

# 3. Tạo cấu trúc thư mục mới và copy cây thư mục đồ sộ sang
echo "[3/4] Đang bốc cây thư mục đồ sộ sang vị trí net/prj3 mới..."
mkdir -p src/main/java/net/prj3
cp -r src/main/java/com/regenerationforrged/* src/main/java/net/prj3/
echo "-> Đã copy cấu trúc cũ sang nhà mới thành công!"

# 4. Thông báo hoàn tất
echo "[4/4] Hoàn tất quy trình!"
echo "========================================="
echo "CHÚC MỪNG: Dự án đã chuyển sang net.prj3 an toàn!"
echo "Lưu ý: Thư mục 'com' cũ vẫn được GIỮ NGUYÊN để cậu kiểm tra."
echo "Nếu compile thử thấy chuẩn 100%, cậu tự tay xóa folder 'com' đi nhé."
echo "========================================="