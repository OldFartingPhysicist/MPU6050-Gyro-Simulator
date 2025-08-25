#include <Wire.h>

#define MPU_ADDR 0x68  // MPU-6050'in default I2C adresi

// --- MPU-6050 Register Map (kısaltılmış) ---
#define REG_WHO_AM_I     0x75
#define REG_PWR_MGMT_1   0x6B
#define REG_ACCEL_XOUT_H 0x3B
#define REG_GYRO_XOUT_H  0x43

// Sahte sensör verileri (başlangıç değerleri)
int16_t ax = 0, ay = 0, az = 16384; // ivme (1g)
int16_t gx = 0, gy = 0, gz = 0;     // gyro
uint8_t whoami = 0x68;
uint8_t pwr_mgmt_1 = 0x00;

volatile uint8_t regAddr = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Fake MPU-6050 baslatildi. Komut formati:");
  Serial.println("AX=deger | AY=deger | AZ=deger | GX=deger | GY=deger | GZ=deger");

  Wire.begin(MPU_ADDR);               // I2C slave başlat
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  // Seri porttan komut oku
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Basit parser
    if (input.startsWith("AX=")) ax = input.substring(3).toInt();
    else if (input.startsWith("AY=")) ay = input.substring(3).toInt();
    else if (input.startsWith("AZ=")) az = input.substring(3).toInt();
    else if (input.startsWith("GX=")) gx = input.substring(3).toInt();
    else if (input.startsWith("GY=")) gy = input.substring(3).toInt();
    else if (input.startsWith("GZ=")) gz = input.substring(3).toInt();

    Serial.print("Yeni Degerler -> ");
    Serial.print("AX="); Serial.print(ax);
    Serial.print(" AY="); Serial.print(ay);
    Serial.print(" AZ="); Serial.print(az);
    Serial.print(" GX="); Serial.print(gx);
    Serial.print(" GY="); Serial.print(gy);
    Serial.print(" GZ="); Serial.println(gz);
  }
}

// Master yazdığında (örn. register adresi gönderir)
void receiveEvent(int howMany) {
  if (howMany >= 1) {
    regAddr = Wire.read();
    howMany--;
  }
  if (howMany >= 1) {
    uint8_t data = Wire.read();
    if (regAddr == REG_PWR_MGMT_1) {
      pwr_mgmt_1 = data;
    }
  }
}

// Master okuma istediğinde
void requestEvent() {
  switch (regAddr) {
    case REG_WHO_AM_I:
      Wire.write(whoami);
      break;

    case REG_PWR_MGMT_1:
      Wire.write(pwr_mgmt_1);
      break;

    case REG_ACCEL_XOUT_H:
      Wire.write((uint8_t)(ax >> 8)); Wire.write((uint8_t)(ax & 0xFF));
      Wire.write((uint8_t)(ay >> 8)); Wire.write((uint8_t)(ay & 0xFF));
      Wire.write((uint8_t)(az >> 8)); Wire.write((uint8_t)(az & 0xFF));
      Wire.write((uint8_t)0); Wire.write((uint8_t)0); // Temp (sahte)
      Wire.write((uint8_t)(gx >> 8)); Wire.write((uint8_t)(gx & 0xFF));
      Wire.write((uint8_t)(gy >> 8)); Wire.write((uint8_t)(gy & 0xFF));
      Wire.write((uint8_t)(gz >> 8)); Wire.write((uint8_t)(gz & 0xFF));
      break;

    case REG_GYRO_XOUT_H:
      Wire.write((uint8_t)(gx >> 8)); Wire.write((uint8_t)(gx & 0xFF));
      Wire.write((uint8_t)(gy >> 8)); Wire.write((uint8_t)(gy & 0xFF));
      Wire.write((uint8_t)(gz >> 8)); Wire.write((uint8_t)(gz & 0xFF));
      break;

    default:
      Wire.write((uint8_t)0x00);
      break;
  }
}
