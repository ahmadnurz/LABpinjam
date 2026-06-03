# LABpinjam System

LABpinjam System adalah aplikasi berbasis C++ yang digunakan untuk mengelola peminjaman loket laboratorium melalui Command Line Interface (CLI). Sistem ini memungkinkan pengguna untuk melihat status loket, melakukan peminjaman, mencatat data peminjam, serta mengelola pengembalian loket dengan penyimpanan data menggunakan file teks sebagai database sederhana.

## Features

* Melihat status ketersediaan loket laboratorium
* Melakukan peminjaman loket
* Menyimpan data peminjam
* Menampilkan riwayat peminjaman aktif
* Mengembalikan loket yang telah selesai digunakan
* Penyimpanan data persisten menggunakan file TXT
* Validasi input pengguna

## Technologies Used

* C++
* Standard Template Library (STL)
* File Handling
* Struct
* Vector
* Command Line Interface (CLI)

## Project Structure

```text
LABpinjam/
│
├── main.cpp
├── loket.txt
├── peminjaman.txt
└── README.md
```

## How It Works

1. Sistem akan membuat file database secara otomatis jika belum tersedia.
2. Pengguna dapat melihat status seluruh loket.
3. Pengguna memilih loket yang tersedia dan mengisi data peminjaman.
4. Data peminjaman disimpan ke file database.
5. Status loket berubah menjadi tidak tersedia.
6. Setelah selesai digunakan, loket dapat dikembalikan melalui menu riwayat peminjaman.

## Running the Project on GitHub Codespaces

### Compile Program

```bash
g++ main.cpp -o main
```

### Run Program

```bash
./main
```

## Example Menu

```text
========================================
          LABpinjam SYSTEM
========================================

[1] Lihat Loket
[2] Pinjam Loket
[3] Riwayat Peminjaman
[4] Exit
```

## Learning Objectives

Project ini dibuat untuk mempelajari:

* Dasar pemrograman C++
* Struktur data menggunakan Struct dan Vector
* Pengelolaan file sebagai database sederhana
* Validasi input pengguna
* Implementasi sistem CRUD sederhana pada aplikasi CLI

## Author

Ahmad Nur Yulhaidir

Computer Engineering Student | Backend Developer & Embedded Systems Enthusiast
