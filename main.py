import cv2
from ultralytics import YOLO

# 🔍 Load 3 mô hình YOLOv8
model_fire = YOLO('./models/fire_model.pt')
model_smoke = YOLO('./models/smoking_best.pt')
model_weapon = YOLO('./models/weapon_best.pt')

# 🎨 Gán màu riêng cho mỗi mô hình và loại class cần bỏ qua
models = [
    (model_fire, (0, 0, 255), []),                       # không loại gì từ fire
    (model_smoke, (0, 255, 255), ['drinking']),          # loại 'drinking'
    (model_weapon, (0, 255, 0), ['smartphone'])          # loại 'smartphone'
]

# 🎥 Mở camera
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("❌ Không thể mở camera.")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("❌ Không thể đọc khung hình từ camera.")
        break

    # 🧠 Dự đoán với từng mô hình
    for model, color, exclude_classes in models:
        results = model(frame, verbose=False)
        for result in results:
            for box in result.boxes:
                conf = box.conf.item()
                if conf < 0.7:
                    continue

                cls_id = int(box.cls.item())
                label_name = model.names[cls_id]

                # ❌ Bỏ qua nếu nằm trong danh sách loại trừ
                if label_name in exclude_classes:
                    continue

                xyxy = box.xyxy[0].cpu().numpy().astype(int)
                label = f'{label_name} {conf:.2f}'

                # 📦 Vẽ bounding box và label
                cv2.rectangle(frame, (xyxy[0], xyxy[1]), (xyxy[2], xyxy[3]), color, 2)
                cv2.putText(frame, label, (xyxy[0], xyxy[1] - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

    # 👀 Hiển thị khung hình kết quả
    cv2.imshow("YOLOv8 Multi-Model Detection (conf > 0.6)", frame)

    # ⏹️ Nhấn 'q' để thoát
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 🧹 Giải phóng tài nguyên
cap.release()
cv2.destroyAllWindows()