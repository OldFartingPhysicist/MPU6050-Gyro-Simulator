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


| Hoverboard Hareketi | İvmeölçer (Accel) | Jiroskop (Gyro) | Açıklama |
| --- | --- | --- | --- |
| İleri gitmek için öne eğilme | AX > 0 | GX > 0 | Kullanıcı öne doğru eğiliyor, kart ileri hareket komutu algılar.
| Geri gitmek için geriye eğilme | AX < 0 | GX < 0 | Kullanıcı geriye eğiliyor, kart geri hareket komutu algılar.
| Sağa eğilme / yatma | AY > 0 | GY > 0 | Hoverboard sağ tarafa yatıyor (dengeyi sağlamak veya dönüş başlatmak için).
| Sola eğilme / yatma | AY < 0  | GY < 0 | Hoverboard sol tarafa yatıyor.
| Yerçekimi (düz zeminde duruş | AZ ≈ +1638 | - | Cihaz sabit, Z ekseni 1g gösterir.
| Düz dururken sağa dönme (yaw) | - | GZ > 0 | Hoverboard sağa dönüyor.
| Düz dururken sola dönme (yaw) | -	| GZ < 0 | Hoverboard sola dönüyor.

## Örnek Senaryolar


| İvmeölçer (Accel) | Jiroskop (Gyro) |Jiroskop (Gyro) | İvmeölçer (Accel) | Açıklama |
| --- | --- | --- | --- | --- |
| AX = 6000 | GY = 0 | GZ = 0 | - | → Kullanıcı biraz öne eğilmiş → Hoverboard yavaşça ileri gidiyor.
| AX = 0 | GY = -300 | GZ = -500 | - | → Hoverboard sola doğru yatıp sola dönüyor.
| AZ = 16384 | AX = 0 | AY = 0 | GX=GY=GZ=0 | → Hoverboard tamamen sabit, düz zeminde duruyor.

## Detaylı Açıklama

### Özet (Programdaki Verilerin Yorumu)

AX = 5000 → Cihaz X ekseninde yaklaşık 0.3g ivmeye maruz (hafif eğim veya ivmelenme).

GY = -200 → Cihaz Y ekseninde yaklaşık −1.5°/s hızla dönüyor.

👉 Yani sen bu sahte değerleri Arduino’dan gönderdiğinde, hoverboard kartı “kullanıcı biraz öne eğilmiş ve hafif yana doğru dönüyor” gibi algılar.


### 🧭 MPU-6050 Eksenleri ve Hoverboard Hareketleri

📌 Tipik yerleşimde MPU-6050 sensörü, hoverboard’un ana kontrol kartının üzerinde şu şekilde konumlanır:

X ekseni (AX, GX) → Hoverboard’un öne/arkaya eğilmesi

Y ekseni (AY, GY) → Hoverboard’un sağa/sola yatması

Z ekseni (AZ, GZ) → Hoverboard’un yukarı/aşağı (yerçekimi) ve dikey eksende dönüşü

1️⃣ İvmeölçer (AX, AY, AZ)

AX → Hoverboard’un öne/arkaya eğimini algılar

Pozitif AX ≈ ileriye eğim

Negatif AX ≈ geriye eğim

AY → Hoverboard’un sağa/sola yatmasını algılar

Pozitif AY ≈ sağa yatma

Negatif AY ≈ sola yatma

AZ → Yerçekimini ölçer (düz zeminde ≈ +16384)

Eğer hoverboard ters çevrilirse ≈ −16384

2️⃣ Jiroskop (GX, GY, GZ)

GX → Öne/arkaya dönüş hızı (pitch hızı)

Pozitif GX ≈ öne doğru devrilme hızı

Negatif GX ≈ geriye devrilme hızı

GY → Sağa/sola yatma hızı (roll hızı)

Pozitif GY ≈ sağa doğru düşme hızı

Negatif GY ≈ sola doğru düşme hızı

GZ → Dikey eksende dönme hızı (yaw hızı)

Hoverboard’un sağa/sola dönmesi

### 🖼️ Basit Görsel Şema
         Hoverboard Üstten Görünüm
     --------------------------------
      Ön → (ileri gitme yönü)

          ↑ AX (+) ileri eğim
          ↓ AX (−) geri eğim
   Sol ← AY (−)          AY (+) → Sağ
   Z ekseni ↑↓ (AZ) = Yerçekimi
   GZ = Hoverboard’un sağa/sola dönmesi


### ✅ Özet:

AX → ileri/geri eğim

AY → sağ/sol yatma

AZ → yukarı/aşağı (yerçekimi)

GX, GY → eğilme hızları

GZ → hoverboard’un yön değiştirmesi

