# Project Konservasi Air
## Abstrak
    Air merupakan salah satu aspek penting dalam keberlangsungan suatu ekosistem. Dalam hal ini, manusia membutuhkan air sebagai salah satu komoditas utama konsumsi agar tetap sehat. Tingkat kualitas air minum pada gedung K menjadi sebuah kekhawatiran beberapa waktu ini akibat rasa yang terkesan tidak enak. Perangkat Konservasi Kualitas Air kemudian menjadi solusi untuk sugesti yang masih belum pasti. Perangkat ini menggunakan 4 probe sensor untuk mengukur parameter air minum yang akan diambil melalui mekanisme mekanik setiap 10 menit sekali. Probe sensor yang digunakan ialah Total Dissolved Solids (TDS), Turbidity (Tingkat kekeruhan), kadar pH, dan suhu pada air minum. Perangkat akan menampilkan hasil pengukuran pada OLED sehingga pengguna dapat melihat langsung kualitas air terakhir yang diuji. 
    
    Kualitas air akan ditampilkan ke dalam webpage untuk mempermudah pengguna dalam melihat kualitas air jarak jauh. Hal ini dilakukan agar pengguna tidak perlu jalan jauh untuk memastikan kualitas air minum pada hasil uji terakhir. Webpage disimpan menggunakan fitur SPIFFS pada ESP32 yang kemudian akan dideploy menggunakan asynchronous ESP32 webserver oleh ESP32 ke IP tertentu dari WiFi UI. Data-data dari ESP32 yang diterima dengan bantuan websocket yang akan mengirimkan suatu data ke dalam webpage yang dideploy oleh ESP32. Dengan begitu, permasalahan mengenai sugesti/kekhawatiran dari kualitas air minum di gedung K akan selesai. 

## Deskripsi Permasalahan
    Fasilitas Drinking Fountain menjadi salah satu penunjang utama dalam kehidupan mahasiswa Fakultas Teknik Universitas Indonesia. Karena dengan adanya fasilitas ini, komoditas utama kebutuhan air dari mahasiswa teknik dapat terpenuhi sekaligus menjaga kondisi lingkungan dengan mengurangi konsumsi plastik sehari-hari. 

    Namun dibalik banyaknya kebutuhan warga atau mahasiswa teknik terhadap water fountain ini, terdapat beberapa kekurangan dari alat ini. Salah satu sisi yang dianggap kurang ialah utilisasi dari alat ini yang masih dianggap meresahkan bagi sebagian besar warga. Berdasarkan survei yang digelar di Fakultas Teknik, tercatat masih 60% mahasiswa fakultas teknik membeli air kemasan. Hal ini disebabkan sebagian besar karena adanya keresahan mengenai kualitas air yang tidak baik bagi kesehatan. Bahkan, dari 60% mahasiswa yang memberikan testimoni, sekitar 90% diantaranya meyakini kualitas air di gedung K merupakan kualitas air yang buruk akibat rasanya yang tidak normal. 

    Dengan adanya problematika tersebut, diperlukan suatu bentuk usaha untuk meyakinkan para warga maupun mahasiswa Fakultas Teknik Universitas Indonesia untuk memakai dan mengkonsumsi air dari water fountain di wilayah teknik. 

## Solusi
    Solusi yang ditawarkan dari permasalahan yang telah disebutkan sebelumnya adalah membuat suatu sistem pengecekan kualitas air dari drinking water fountain di Gedung K Fakultas Teknik Universitas Indonesia yang mampu diintegrasikan dengan beberapa alat lainnya dengan mengutilisasikan beberapa sensor seperti sensor pH, Turbiditas, sensor padatan (total disolved solids), dan sensor suhu yang akan secara langsung ditampilkan hasilnya ke webpage dan layar OLED dari alat. Proses pengambilan data/air yang akan dinilai akan secara otomatis berjalan setiap 10 menit. Di mana saat pengambilan data ini, program akan berjalan 4 kali untuk memperoleh hasil paling optimal. Setelah pengambilan data selesai, air akan dibuang melalui pompa yang ada di dalam alat. Harapan dari solusi ini ialah pengguna/warga FTUI dapat lebih yakin dalam memilih kualitas air. Selain itu, dengan adanya webpage diharapkan warga FTUI tidak perlu untuk jalan jauh untuk melihat bagaimana kualitas air dari water fountain yang berada di gedung lain. 

## Batasan Masalah
    Batasan Masalah dari proyek ini dirangkum dalam beberapa poin berikut: 

1. Karena alat bersifat prototipe, maka alat hanya akan ditaruh pada salah satu drinking fountain. Namun proyek bersifat scalable di mana mudah untuk diperluas/diperbanyak produksinya 
2. empat Parameter yang akan diuji yaitu pH, TDS, Turbiditas, dan suhu dari air. 
3. Desain dan peralatan aman untuk dioperasikan dengan barang yang akan dikonsumsi dengan manusia (tidak bersentuhan secara langsung dengan air konsumsi langsung). 
4. Probe pH tidak tahan untuk penggunaan jangka panjang karena adanya batasan biaya dan batasan dari komponen berdasarkan datasheet. 
5. Tidak ada peralatan yang bocor 
6. Pemasangan sistem tidak bersifat invasif dan memakan waktu yang singkat 

    Batasan masalah terkait konstrain: 

1. Ekonomi: Anggaran yang diberikan adalah Rp2.000.000,00 
2. Sosial: User Experience yang diberikan harus mampu memberikan informasi yang cukup jelas bahkan bagi pemula/orang yang baru melihat webpage maupun OLED dari alat. 
3. Politik: Izin penggunaan serta desain dari alat akan didasarkan oleh kebutuhan pemilik kepentingan di mana dalam hal ini adalah pemilik fasilitas water fountain di FTUI. 
4. Lingkungan: alat dipastikan menggunakan power supply berupa power adapter guna mengurangi sampah elektrik/baterai dan juga bertujuan untuk mengurangi penggunaan sampah plastik (air kemasan) di FTUI.

## Target
    Target yang diharapkan pada proyek ini yaitu: 

1. Perangkat mampu melakukan pengukuran untuk setiap parameter uji pada sampel air minum yang diambil. 
2. Perangkat mampu menampilkan hasil pengukuran tersebut melalui komponen/bahasa yang lebih mudah dipahami manusia (dengan menggunakan OLED) 
3. Perangkat mampu mengirimkan data hasil uji air ke dalam suatu webpage yang telah didesain. 
4. Mikrokontroller dari perangkat mampu mendeploy sebuah webpage berbasis server yang dapat diakses siapapun yang mengakses wifi UI untuk melihat informasi kualitas air dari water fountain.  

## Metode
    Dengan mempertimbangkan waktu dan beban kerja serta konstrain-konstrain terkait, metode pada perancangan perangkat ini meliputi beberapa tahap sebagai berikut: 

1. Metode perencanaan dari sistem dari sisi kelistrikan, mekanik, dan programming. 
2. Metode pembentukan UI/UX serta Front-end dari Webpage. 
3. Metode prototyping komunikasi infrastuktur terhadap webpage 
4. Metode serial prototyping 
5. Metode integrasi serial prototipe dengan komunikasi infrastruktur terhadap webpage (back-end).  
6. Metode perancangan hardware meliputi PCB dan Casing 
7. Metode produksi dan integrasi hardware 
8. Metode uji perangkat 
9. Metode Deployment 
10. User Acceptance Testing

## Requirements
For installation, the following libraries needs to be installed under Sketch -> Include Library -> Manage Libraries:
* ArduinoJson by Benoit Blanchon
* WebSockets by Markus Sattler
* WiFi 

The following to libraries need to be downloaded, unpacked and copied to the "Arduino" folder (Required for ESPAsyncWebServer)
- https://github.com/me-no-dev/ESPAsyncWebServer
- https://github.com/me-no-dev/AsyncTCP

Required to make SPIFFS.h work:
https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/

### How to Install Packages Manually
- Click on the link provided in each package
- Download zip file of the packages through the 'code' dropdown
- Unzip/extract the downloaded file
- Put the packages folder in to libraries folder in Arduino Folder (usually in C:/Users/Documents/Arduino)

### How to Install Tools Manually
- Click on the link provided in each package
- Download zip file of the tools in the asset section of the page
- Unzip/extract the downloaded file
- Put the asset folder into tools folder in Arduino Folder (usually in C:/Users/Documents/Arduino)
- After putting asset folder into tools folder, restart the Arduino IDE
- Check if in the dropdown section of Tools there is ESP32 Sketch Data Upload
- [X] if there is, then you are set to go

## Code Pre-configuration
In the ino. files in this section,
```
// SSID and password of Wifi connection:
const char* ssid = "HotSpot - UI (NEW)";
const char* password = "";
```
ssid and password need to be changed according to the router/wifi that is used.

> Before uploading the code, **please upload data sketch first using ESP32 Sketch Data Upload button on tools dropdown section**

## Created by
Bryan Indarto, Aldy Raja, Theonaldo, dan Reza Febrian
