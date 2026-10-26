# Analisis Akar Masalah Kegagalan Boot SD Card — Project `/home/maruf/fsbl/`

## 1. Ringkasan Eksekutif & Akar Masalah Utama
Project `/home/maruf/fsbl/` mengalami kegagalan boot dari SD card (`BOOT.bin` tidak tereksekusi, lampu DONE FPGA tidak menyala / CPU hang) karena kombinasi faktor berikut:

1. **Card Detect (CD) Tidak Aktif (`XPAR_XSDPS_0_HAS_CD = 0`):**
   * Berdasarkan header BSP (`xparameters.h`), `XPAR_XSDPS_0_HAS_CD` bernilai `0`.
   * Pengontrol SDIO Zynq membaca register `Present State (0xE0100024)` dengan status `Card Inserted = 0`, sehingga FSBL membatalkan proses pembacaan SD card.
   * **Penyebab Vivado:** Pin MIO 34 pada Zynq PS7 tidak dikonfigurasi sebagai *SD0 Card Detect* (CD) dengan *pull-up* di Vivado Block Design.

2. **Mode Bus Width 4-bit Menyebabkan Timeout (`Error 0x0010`):**
   * File `/home/maruf/fsbl/fsbl.sdk/fsblnya/src/sd.c` menggunakan driver standar Xilinx (vanilla) tanpa penyesuaian *1-bit fallback*.
   * Pada board EBAZ4205 (modifikasi slot microSD solderan eksternal), jalur data DAT1..DAT3 sering mengalami *signal integrity issues* atau *timeout* saat negosiasi *High Speed / 4-bit mode* (`CMD6`).

3. **Format Partisi SD Card / Filesystem:**
   * Parser FAT32 BootROM Zynq sangat sensitif terhadap *cluster size* dan partisi MBR. Jika diformat dengan cluster > 4KB (misal 16KB/32KB) atau menggunakan exFAT/NTFS, BootROM gagal membaca sektor 0 / MBR.

4. **Konfigurasi BIF & Image Generation:**
   * File `bootimage/output.bif` sudah mengarah ke file ELF dan bitstream yang benar, namun jika partisi SD card atau FSBL gagal di awal, `BOOT.bin` tidak akan pernah terbaca.

---

## 2. Detail Konfigurasi BSP & Kode (`fsblnya`)
* **MSS / BSP:** `xilffs` terpasang, namun `xsdps` mendeteksi `HAS_CD = 0` dan `BUS_WIDTH = 0`.
* **Driver SD (`sd.c`):** Belum dimodifikasi untuk memaksa mode 1-bit (`SD_ED_WIDTH_1_BIT`) dan 25 MHz clock stability.

---

## 3. Rekomendasi Solusi & Perbaikan
1. **Vivado:** Buka block design Zynq PS7, aktifkan **SD 0** dengan MIO 40..45, dan aktifkan **Card Detect (CD) pada MIO 34** dengan *Internal Pull-up*. Re-export HDF.
2. **SDK BSP / FSBL Source:** Modifikasi `sd.c` di `fsblnya/src/` agar memaksa **1-bit bus width** dan menurunkan clock ke **25 MHz** guna menghindari *Data Timeout Error* (`0x0010`) pada slot microSD EBAZ4205.
3. **Format SD Card:** Format ulang kartu MicroSD dengan skema MBR, partisi FAT32, dan **Cluster Size 4096 bytes (4 KB)**. Simpan file boot sebagai `BOOT.bin`.
