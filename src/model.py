import cv2
import numpy as np
# Python Opencv import

VideoSignal = cv2.VideoCapture(0)

YOLO_net = cv2.dnn.readNet("yolov2-tiny.weights", "yolov2-tiny.cfg")

classes = []
with open("yolo.names", "r") as f:
    classes = [line.strip() for line in f.readlines()]
for i in YOLO_net.getUnconnectedOutLayers():
    print(i)
layer_names = YOLO_net.getLayerNames()
output_layers = [layer_names[i - 1] for i in YOLO_net.getUnconnectedOutLayers()]
# Opencv와 YOLO_Tiny 모델 연결

while True:
    ret, frame = VideoSignal.read()
    h, w, c = frame.shape

    blob = cv2.dnn.blobFromImage(frame, 0.00392, (416, 416), (0, 0, 0), True, crop=False)
    YOLO_net.setInput(blob)
    outs = YOLO_net.forward(output_layers)
    # 영상 송출 준비
    class_ids = []
    confidences = []
    boxes = []

    for out in outs:
    # 영상 송출
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            if confidence > 0.5:
                center_x = int(detection[0] * w)
                center_y = int(detection[1] * h)
                dw = int(detection[2] * w)
                dh = int(detection[3] * h)
                x = int(center_x - dw / 2)
                y = int(center_y - dh / 2)
                boxes.append([x, y, dw, dh])
                confidences.append(float(confidence))
                class_ids.append(class_id)

    indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.45, 0.4)
    # 감지된 윤곽선 생성

    for i in range(len(boxes)):
        if i in indexes:
            x, y, w, h = boxes[i]
            label = str(classes[class_ids[i]])
            score = confidences[i]

            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 5)
            cv2.putText(frame, label, (x, y - 20), cv2.FONT_ITALIC, 0.5, (255, 255, 255), 1)

    cv2.imshow("YOLOv3", frame)
    # YOLO모델 연결
    if cv2.waitKey(100) > 0:
        # 일정 대기시간 이상이면 연결 해제
        break

VideoSignal.release()
cv2.destroyAllWindows()
