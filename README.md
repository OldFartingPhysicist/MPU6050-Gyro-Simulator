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

# Hoverboard Hareketleri <--> MPU-6050 Sensör Ekseni
## |Hoverboard Hareketi|İvmeölçer (Accel)|Jiroskop (Gyro)|Açıklama|
## |---|---|---|---|

|İleri gitmek için öne eğilme|      AX > 0|                GX > 0|             Kullanıcı öne doğru eğiliyor, kart ileri hareket komutu algılar.|

Geri gitmek için geriye eğilme    AX < 0                GX < 0             Kullanıcı geriye eğiliyor, kart geri hareket komutu algılar.

Sağa eğilme / yatma               AY > 0                GY > 0             Hoverboard sağ tarafa yatıyor (dengeyi sağlamak veya dönüş başlatmak için).

Sola eğilme / yatma               AY < 0                GY < 0             Hoverboard sol tarafa yatıyor.

Yerçekimi (düz zeminde duruş)     AZ ≈ +16384	            -	               Cihaz sabit, Z ekseni 1g gösterir.

Düz dururken sağa dönme (yaw)	        -                 GZ > 0             Hoverboard sağa dönüyor.

Düz dururken sola dönme (yaw)      	  -	                GZ < 0             Hoverboard sola dönüyor.

## Örnek Senaryolar

AX = 6000, GY = 0, GZ = 0 → Kullanıcı biraz öne eğilmiş → Hoverboard yavaşça ileri gidiyor.

AX = 0, GY = -300, GZ = -500 → Hoverboard sola doğru yatıp sola dönüyor.

AZ = 16384, AX = 0, AY = 0, GX=GY=GZ=0 → Hoverboard tamamen sabit, düz zeminde duruyor.
