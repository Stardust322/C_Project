import torch
import cv2
import pathlib
import warnings
warnings.filterwarnings("ignore", category=FutureWarning)
temp = pathlib.PosixPath
pathlib.PosixPath = pathlib.WindowsPath
model = torch.hub.load('ultralytics/yolov5', 'custom', path=r'C:\Users\stardust\source\repos\Test\best.pt', force_reload=True)
cap = cv2.VideoCapture(0)
food_class_id = 0
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("프레임 캡처 실패")
        break
    results = model(frame)
    for *box, conf, cls in results.xyxy[0]:
        label = f'{model.names[int(cls)]} {conf:.2f}'
        cv2.rectangle(frame, (int(box[0]), int(box[1])), (int(box[2]), int(box[3])), (0, 255, 0), 2)
        cv2.putText(frame, label, (int(box[0]), int(box[1]) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    cv2.imshow('YOLOv5 Detection', frame)
    detected_classes = results.xywh[0][:, -1].cpu().numpy()
    food_detected = sum(detected_classes == food_class_id)
    if food_detected > 1:
        print("경고: 여러 그릇에 잔반이 담겨 있습니다!")
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()