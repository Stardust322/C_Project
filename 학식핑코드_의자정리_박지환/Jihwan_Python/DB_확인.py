import sqlite3

# 데이터베이스 파일 연결
conn = sqlite3.connect("sensor_data.db")
cursor = conn.cursor()

# 저장된 데이터 조회
cursor.execute("SELECT * FROM sensor_data")
rows = cursor.fetchall()

# 데이터 출력
print("Stored data:")
for row in rows:
    print(row)

# 연결 종료
conn.close()
