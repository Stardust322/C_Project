import cv2
import webbrowser
import torch
import pathlib
import warnings
warnings.filterwarnings("ignore", category=FutureWarning)
print("Embedding Python...")
temp = pathlib.PosixPath
pathlib.PosixPath = pathlib.WindowsPath
model = torch.hub.load('ultralytics/yolov5', 'custom', path=r"C:\Users\stardust\source\repos\Test\best.pt", force_reload=True)
url = 'http://192.168.35.80:81/stream' 
cap = cv2.VideoCapture(url)
webbrowser.open(url, new=2)
if not cap.isOpened():
    print("스트리밍 연결 안됨.")
food_class_id = 0
while True:
    ret, frame = cap.read()
    if not ret:
        print("프레임 캡처 실패")
        break 
    results = model(frame)
    detections = results.xyxy[0]
    for *box, confidence, class_id in detections.cpu().numpy():
        x1, y1, x2, y2 = map(int, box)
        class_id = int(class_id)
        label = f"{results.names[class_id]} {confidence:.2f}"
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
    cv2.imshow("ESP32 Stream - Object Detection", frame)
    detected_classes = results.xywh[0][:, -1].cpu().numpy()
    food_detected = sum(detected_classes == food_class_id)
    if food_detected > 1:
        print("경고: 여러 그릇에 잔반이 담겨 있습니다!")
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()