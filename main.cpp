/*
 * ========================================
 *         LABpinjam SYSTEM
 *   Sistem Peminjaman Loket Laboratorium
 * ========================================
 * File   : main.cpp
 * Bahasa : C++
 * OS     : Windows (CMD / Windows Terminal)
 * ========================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ============================================================
//  KONSTANTA
// ============================================================
const int  TOTAL_LOKET  = 12;
const string FILE_LOKET = "loket.txt";
const string FILE_DATA  = "peminjaman.txt";

// ============================================================
//  STRUKTUR DATA
// ============================================================
struct Peminjam {
    int    nomorLoket;
    string nama;
    string noHp;
    string kelas;
};

// ============================================================
//  DEKLARASI FUNGSI
// ============================================================
void  clearScreen();
void  printLine();
void  printHeader(const string& judul);
void  pause();

void  inisialisasiFile();
bool  bacaStatusLoket(bool status[]);
bool  simpanStatusLoket(const bool status[]);
bool  bacaDataPeminjaman(vector<Peminjam>& daftar);
bool  simpanDataPeminjaman(const vector<Peminjam>& daftar);

void  menuLihatLoket();
void  menuPinjamLoket();
void  menuRiwayat();

string trim(const string& s);
bool   isValidNomor(const string& input, int& out, int minVal, int maxVal);

// ============================================================
//  FUNGSI UTILITAS
// ============================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine() {
    cout << "========================================" << endl;
}

void printHeader(const string& judul) {
    printLine();
    // Rata tengah dalam lebar 40 karakter
    int lebar  = 40;
    int panjang = (int)judul.size();
    int spasi  = (lebar - panjang) / 2;
    if (spasi < 0) spasi = 0;
    cout << string(spasi, ' ') << judul << endl;
    printLine();
}

void pause() {
    cout << endl;
    cout << "Tekan Enter untuk kembali...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Hapus spasi di awal dan akhir string
string trim(const string& s) {
    size_t awal  = s.find_first_not_of(" \t\r\n");
    size_t akhir = s.find_last_not_of(" \t\r\n");
    if (awal == string::npos) return "";
    return s.substr(awal, akhir - awal + 1);
}

// Validasi input angka dalam rentang [minVal, maxVal]
bool isValidNomor(const string& input, int& out, int minVal, int maxVal) {
    if (input.empty()) return false;
    for (char c : input) {
        if (!isdigit(c)) return false;
    }
    try {
        out = stoi(input);
    } catch (...) {
        return false;
    }
    return (out >= minVal && out <= maxVal);
}

// ============================================================
//  FUNGSI FILE DATABASE
// ============================================================

// Buat file database jika belum ada
void inisialisasiFile() {
    // Cek file loket
    ifstream cekLoket(FILE_LOKET);
    if (!cekLoket.is_open()) {
        ofstream buatLoket(FILE_LOKET);
        if (buatLoket.is_open()) {
            for (int i = 1; i <= TOTAL_LOKET; i++) {
                buatLoket << i << " 1" << endl; // 1 = tersedia
            }
            buatLoket.close();
        }
    } else {
        cekLoket.close();
    }

    // Cek file peminjaman
    ifstream cekData(FILE_DATA);
    if (!cekData.is_open()) {
        ofstream buatData(FILE_DATA);
        buatData.close();
    } else {
        cekData.close();
    }
}

// Baca status loket dari file (true = tersedia, false = tidak tersedia)
bool bacaStatusLoket(bool status[]) {
    ifstream file(FILE_LOKET);
    if (!file.is_open()) return false;

    // Inisialisasi default tersedia
    for (int i = 0; i < TOTAL_LOKET; i++) status[i] = true;

    int nomor, nilai;
    while (file >> nomor >> nilai) {
        if (nomor >= 1 && nomor <= TOTAL_LOKET) {
            status[nomor - 1] = (nilai == 1);
        }
    }
    file.close();
    return true;
}

// Simpan status loket ke file
bool simpanStatusLoket(const bool status[]) {
    ofstream file(FILE_LOKET);
    if (!file.is_open()) return false;
    for (int i = 0; i < TOTAL_LOKET; i++) {
        file << (i + 1) << " " << (status[i] ? 1 : 0) << endl;
    }
    file.close();
    return true;
}

// Baca data peminjaman dari file
// Format per baris: noLoket|nama|noHp|kelas
bool bacaDataPeminjaman(vector<Peminjam>& daftar) {
    daftar.clear();
    ifstream file(FILE_DATA);
    if (!file.is_open()) return false;

    string baris;
    while (getline(file, baris)) {
        baris = trim(baris);
        if (baris.empty()) continue;

        // Pisah dengan delimiter '|'
        vector<string> token;
        stringstream ss(baris);
        string bagian;
        while (getline(ss, bagian, '|')) {
            token.push_back(bagian);
        }
        if (token.size() < 4) continue;

        Peminjam p;
        try {
            p.nomorLoket = stoi(token[0]);
        } catch (...) {
            continue;
        }
        p.nama  = token[1];
        p.noHp  = token[2];
        p.kelas = token[3];
        daftar.push_back(p);
    }
    file.close();
    return true;
}

// Simpan semua data peminjaman ke file
bool simpanDataPeminjaman(const vector<Peminjam>& daftar) {
    ofstream file(FILE_DATA);
    if (!file.is_open()) return false;
    for (const Peminjam& p : daftar) {
        file << p.nomorLoket << "|"
             << p.nama       << "|"
             << p.noHp       << "|"
             << p.kelas      << endl;
    }
    file.close();
    return true;
}

// ============================================================
//  MENU 1 - LIHAT LOKET
// ============================================================
void menuLihatLoket() {
    clearScreen();
    printHeader("STATUS LOKET");

    bool status[TOTAL_LOKET];
    if (!bacaStatusLoket(status)) {
        cout << "  [ERROR] Gagal membaca file database." << endl;
        pause();
        return;
    }

    cout << endl;
    for (int i = 0; i < TOTAL_LOKET; i++) {
        string label = "Loket " + to_string(i + 1);
        // Padding agar rata
        cout << "  " << left << setw(10) << label;
        if (status[i]) {
            cout << "[TERSEDIA]" << endl;
        } else {
            cout << "[TIDAK TERSEDIA]" << endl;
        }
    }

    pause();
}

// ============================================================
//  MENU 2 - PINJAM LOKET
// ============================================================
void menuPinjamLoket() {
    clearScreen();
    printHeader("PINJAM LOKET");
    cout << endl;

    bool status[TOTAL_LOKET];
    if (!bacaStatusLoket(status)) {
        cout << "  [ERROR] Gagal membaca file database." << endl;
        pause();
        return;
    }

    // Input nomor loket
    string inputNomor;
    int nomorLoket = 0;
    cout << "  Nomor Loket (1-" << TOTAL_LOKET << ") : ";
    getline(cin, inputNomor);
    inputNomor = trim(inputNomor);

    if (!isValidNomor(inputNomor, nomorLoket, 1, TOTAL_LOKET)) {
        cout << endl;
        cout << "  [ERROR] Nomor loket tidak valid." << endl;
        pause();
        return;
    }

    // Cek apakah loket tersedia
    if (!status[nomorLoket - 1]) {
        cout << endl;
        cout << "  [INFO] Loket " << nomorLoket << " sedang digunakan." << endl;
        pause();
        return;
    }

    // Input data peminjam
    Peminjam p;
    p.nomorLoket = nomorLoket;

    cout << "  Nama             : ";
    getline(cin, p.nama);
    p.nama = trim(p.nama);

    cout << "  Nomor Handphone  : ";
    getline(cin, p.noHp);
    p.noHp = trim(p.noHp);

    cout << "  Kelas            : ";
    getline(cin, p.kelas);
    p.kelas = trim(p.kelas);

    // Validasi tidak boleh kosong
    if (p.nama.empty() || p.noHp.empty() || p.kelas.empty()) {
        cout << endl;
        cout << "  [ERROR] Semua field wajib diisi." << endl;
        pause();
        return;
    }

    // Simpan data peminjaman
    vector<Peminjam> daftar;
    bacaDataPeminjaman(daftar);
    daftar.push_back(p);

    if (!simpanDataPeminjaman(daftar)) {
        cout << endl;
        cout << "  [ERROR] Gagal menyimpan data peminjaman." << endl;
        pause();
        return;
    }

    // Update status loket
    status[nomorLoket - 1] = false;
    if (!simpanStatusLoket(status)) {
        cout << endl;
        cout << "  [ERROR] Gagal memperbarui status loket." << endl;
        pause();
        return;
    }

    cout << endl;
    printLine();
    cout << "  [SUKSES] Loket " << nomorLoket << " berhasil dipinjam." << endl;
    printLine();
    pause();
}

// ============================================================
//  MENU 3 - RIWAYAT PEMINJAMAN
// ============================================================
void menuRiwayat() {
    clearScreen();
    printHeader("RIWAYAT PEMINJAMAN");
    cout << endl;

    vector<Peminjam> daftar;
    bacaDataPeminjaman(daftar);

    if (daftar.empty()) {
        cout << "  Belum ada data peminjaman aktif." << endl;
    } else {
        for (const Peminjam& p : daftar) {
            cout << "  Loket : " << p.nomorLoket << endl;
            cout << "  Nama  : " << p.nama       << endl;
            cout << "  No HP : " << p.noHp       << endl;
            cout << "  Kelas : " << p.kelas      << endl;
            cout << "  ----------------------------------------" << endl;
        }
    }

    // Input selesai
    cout << endl;
    cout << "  Input loket selesai digunakan (0 = batal) : ";
    string inputSelesai;
    getline(cin, inputSelesai);
    inputSelesai = trim(inputSelesai);

    if (inputSelesai == "0" || inputSelesai.empty()) {
        return; // Batal, kembali ke menu utama
    }

    int nomorSelesai = 0;
    if (!isValidNomor(inputSelesai, nomorSelesai, 1, TOTAL_LOKET)) {
        cout << endl;
        cout << "  [ERROR] Nomor loket tidak valid." << endl;
        pause();
        return;
    }

    // Cari data peminjaman dengan nomor loket tersebut
    bool ditemukan = false;
    vector<Peminjam> daftarBaru;
    for (const Peminjam& p : daftar) {
        if (p.nomorLoket == nomorSelesai) {
            ditemukan = true; // Hapus dari daftar (tidak dimasukkan ke daftarBaru)
        } else {
            daftarBaru.push_back(p);
        }
    }

    if (!ditemukan) {
        cout << endl;
        cout << "  [ERROR] Data peminjaman untuk Loket " << nomorSelesai << " tidak ditemukan." << endl;
        pause();
        return;
    }

    // Simpan data baru
    if (!simpanDataPeminjaman(daftarBaru)) {
        cout << endl;
        cout << "  [ERROR] Gagal memperbarui data peminjaman." << endl;
        pause();
        return;
    }

    // Update status loket menjadi tersedia
    bool status[TOTAL_LOKET];
    bacaStatusLoket(status);
    status[nomorSelesai - 1] = true;
    if (!simpanStatusLoket(status)) {
        cout << endl;
        cout << "  [ERROR] Gagal memperbarui status loket." << endl;
        pause();
        return;
    }

    cout << endl;
    printLine();
    cout << "  [SUKSES] Loket " << nomorSelesai << " telah dikembalikan." << endl;
    printLine();
    pause();
}

// ============================================================
//  MENU UTAMA
// ============================================================
void tampilMenuUtama() {
    clearScreen();
    printHeader("LABpinjam SYSTEM");
    cout << endl;
    cout << "  [1] Lihat Loket"          << endl;
    cout << "  [2] Pinjam Loket"         << endl;
    cout << "  [3] Riwayat Peminjaman"   << endl;
    cout << "  [4] Exit"                 << endl;
    cout << endl;
    printLine();
    cout << "  Pilih menu : ";
}

// ============================================================
//  MAIN
// ============================================================
int main() {
    // Inisialisasi file database jika belum ada
    inisialisasiFile();

    string pilihan;
    bool berjalan = true;

    while (berjalan) {
        tampilMenuUtama();
        getline(cin, pilihan);
        pilihan = trim(pilihan);

        if (pilihan == "1") {
            menuLihatLoket();
        } else if (pilihan == "2") {
            menuPinjamLoket();
        } else if (pilihan == "3") {
            menuRiwayat();
        } else if (pilihan == "4") {
            clearScreen();
            printLine();
            cout << "  Terima kasih telah menggunakan LABpinjam SYSTEM." << endl;
            printLine();
            cout << endl;
            berjalan = false;
        } else {
            clearScreen();
            printHeader("LABpinjam SYSTEM");
            cout << endl;
            cout << "  [ERROR] Pilihan tidak valid. Masukkan angka 1-4." << endl;
            pause();
        }
    }

    return 0;
}