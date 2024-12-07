import serial
import sqlite3
from datetime import datetime
import csv
import os

# 시리얼 포트 설정
serial_port = "COM5"  # 아두이노가 연결된 포트 번호
baud_rate = 9600      # 아두이노와 동일한 Baud Rate 설정
arduino = serial.Serial(serial_port, baud_rate, timeout=1)

# SQLite3 데이터베이스 설정
db_name = "sensor_data.db"
conn = sqlite3.connect(db_name)
cursor = conn.cursor()

# 테이블 생성
cursor.execute("""
CREATE TABLE IF NOT EXISTS sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT,
    distance INTEGER
)
""")
conn.commit()

csv_file = "sensor_data.csv"
file_exists = os.path.exists(csv_file)

if not file_exists:
    with open(csv_file, mode="w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["timestamp", "distance"])  # CSV 헤더 작성

# 데이터 수집 및 저장
try:
    while True:
        # 아두이노로부터 데이터 읽기
        line = arduino.readline().decode("utf-8").strip()
        if line:
            print(f"Received: {line}")

            # 아두이노에서 받은 데이터가 특정 패턴("BUZZER_ON, distance=123")인지 확인
            if "BUZZER_ON" in line:
                
                # 초음파 센서 값 추출
                distance = int(line.split("=")[-1])
            
                # 현재 시간 가져오기
                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

                # SQLite 데이터베이스에 저장
                cursor.execute("""
                INSERT INTO sensor_data (timestamp, distance)
                VALUES (?, ?)
                """, (timestamp, distance))
                conn.commit()

                # CSV 파일에 저장
                with open(csv_file, mode="a", newline="") as file:
                    writer = csv.writer(file)
                    writer.writerow([timestamp, distance])

                # 저장 결과 출력
                print(f"Data saved: {timestamp}, {distance} cm")

except KeyboardInterrupt:
    print("Stopping data logging...")

finally:
    # 연결 종료
    arduino.close()
    conn.close()
