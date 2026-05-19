# Klimatik Veri Merkezi (IoT & Gömülü Sistem Projesi)

Bu proje; internet veya radyo erişimi kısıtlı endüstriyel/tarımsal alanlarda çevresel koşulların (sıcaklık, nem, ışık) anlık takibini sağlamak, kritik eşikler aşıldığında donanımsal uyarı mekanizmalarını devreye sokmak ve verileri bilgisayar arayüzünde görselleştirmek amacıyla geliştirilmiş **uçtan uca (End-to-End)** bir donanım-yazılım entegrasyonu projesidir.

Proje, mikrodenetleyici üzerinde koşan alt katman yazılım (**Firmware**) ve verileri işleyen üst katman arayüz yazılımı (**Upper Software**) olmak üzere iki ana modülden oluşmaktadır.

---

## Proje Yapısı (Repository Structure)

* `Firmware_Arduino/` : C/C++ ile yazılmış, sensör yönetimini ve veri aktarımını sağlayan mikrodenetleyici kodları.
* `UpperSoftware_CSharp/` : C# .NET WinForms mimarisiyle geliştirilmiş, seri port üzerinden gelen verileri parse eden arayüz yazılımı.

---

## Teknoloji Yığını ve Donanım Bileşenleri

### 1. Donanım & Alt Katman (Firmware)
* **Mikrodenetleyici:** Arduino UNO (Atmega328P)
* **Geliştirme Dili:** C / C++ (Arduino IDE)
* **Sensörler:** DHT11 (Sıcaklık ve Nem), LDR (Işık Sensörü)
* **Donanımsal Arayüz:** 16x2 LCD Ekran, Potansiyometre, Uyarı LED'i
* **Haberleşme Protokolü:** UART / Serial Communication (9600 Baud Rate)

### 2. Üst Katman Arayüz Yazılımı (Upper Software)
* **Geliştirme Ortamı:** C# .NET WinForms / Windows Desktop
* **Haberleşme Kütüphanesi:** `System.IO.Ports` (SerialPort API)
* **Asenkron Yönetim:** `this.Invoke` / `MethodInvoker` (Thread-Safe UI Updates)

---

## Teknik Kabiliyetler ve Çalışma Mantığı

###  Alt Katman (Firmware) Mantığı
* **Sensör Verisi İşleme:** DHT11 and LDR sensörlerinden gelen analog ve dijital sinyaller asenkron olarak okunur ve kalibre edilir. Okunan değerler eş zamanlı olarak 16x2 LCD ekrana basılır.
* **Durum Sınıflandırma Algoritması:** Sistem, sensör verilerini analiz ederek ortam durumunu otomatik olarak "Güneşli", "Yağmurlu", "Bulutlu", "Gece" veya "Gündüz" şeklinde sınıflandırır.
* **Hata ve Eşik Yönetimi:** Sıcaklığın (10°C altı / 30°C üstü) veya nemin (%30 altı / %80 üstü) kritik sınırları aşması durumunda `ledBlink()` fonksiyonu tetiklenerek donanımsal alarm verilir.
* **Seri Telemetri Aktarımı:** Tüm sensör verileri ve durum çıktıları, üst katman yazılımın anlayacağı özel bir veri formatıyla `Serial.println()` üzerinden UART hattına basılır.

### Üst Katman (C# UI) Mantığı
* **Asenkron Seri Port Dinleme:** Uygulama, belirtilen COM portunu asenkron olarak dinler. `serialPort_DataReceived` event'i tetiklendiğinde gelen ham satır verilerini arabellekten okur.
* **Veri Ayrıştırma (Data Parsing):** Gelen paketlenmiş veriler `String.Split` ve `Contains` algoritmalarıyla çözülerek anlamlı sıcaklık, nem ve ışık değişkenlerine dönüştürülür.
* **Thread-Safe UI Operasyonları:** Arka planda (Background Thread) çalışan seri veri akışının ana arayüzü kilitlemesini (freezing) önlemek amacıyla, UI güncellemeleri thread-safe (`Invoke`) mimariyle güvenli bir şekilde gerçekleştirilir.
