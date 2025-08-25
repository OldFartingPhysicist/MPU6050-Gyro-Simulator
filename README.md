# Kullanım
(Geliştirme aşamasında, kod çalışmayabilir)

## Bu Kod Ne Yapar?

Arduino’yu I²C slave (adres 0x68) olarak kurar.

Master cihaz (ör. başka Arduino ya da hoverboard kartı) MPU-6050 registerlerini okuduğunda, Arduino sahte veriler döndürür.

WHO_AM_I registeri → 0x68 döner (gerçek MPU-6050 gibi).

ACCEL_XOUT_H → sahte ivme verileri döner.

GYRO_XOUT_H → sahte gyro verileri döner.

PWR_MGMT_1 gibi registerler yazılabilir ve okunabilir.

## Arduino’yu tak, bu kodu yükle.

Serial Monitor’u aç (115200 baud).

Komut yaz:

AX=5000 → X ivmesini değiştirir.

GY=-200 → Y jiroskop değerini değiştirir.

AZ=16384 → 1g yerçekimi simülasyonu.

Hoverboard kartı (veya diğer Arduino) bu değerleri gerçek MPU-6050 verisiymiş gibi okuyacak.
