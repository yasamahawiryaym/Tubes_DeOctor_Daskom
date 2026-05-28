#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===== CONSTANTS =====
#define MAX_PASIEN     100
#define MAX_DOKTER     50
#define MAX_RESERVASI  200
#define MAX_RESEP      200
#define MAX_FEEDBACK   200
#define MAX_STR        100
#define MAX_LOGIN_ATTEMPTS 3

// ===== FILE NAMES =====
#define FILE_PASIEN    "pasien.dat"
#define FILE_DOKTER    "dokter.dat"
#define FILE_RESERVASI "reservasi.dat"
#define FILE_RESEP     "resep.dat"
#define FILE_FEEDBACK  "feedback.dat"

// ===== STRUCTS =====
typedef struct {
    int id;
    char username[MAX_STR];
    char password[MAX_STR];
    char nama[MAX_STR];
    char email[MAX_STR];
    char telepon[MAX_STR];
    int aktif;
} Pasien;

typedef struct {
    int id;
    char username[MAX_STR];
    char password[MAX_STR];
    char nama[MAX_STR];
    char spesialis[MAX_STR];
    char jadwal[MAX_STR];
    int aktif;
} Dokter;

typedef struct {
    int id;
    int id_pasien;
    int id_dokter;
    char waktu[MAX_STR];
    char status[MAX_STR];
} Reservasi;

typedef struct {
    int id;
    int id_pasien;
    int id_dokter;
    int id_reservasi;
    char obat[MAX_STR];
    char dosis[MAX_STR];
    char keterangan[MAX_STR];
    char tanggal[MAX_STR];
} Resep;

typedef struct {
    int id;
    int id_pasien;
    int id_dokter;
    char isi[256];
    int rating;
    char tanggal[MAX_STR];
} Feedback;

// ===== GLOBAL DATA =====
Pasien    daftarPasien[MAX_PASIEN];
Dokter    daftarDokter[MAX_DOKTER];
Reservasi daftarReservasi[MAX_RESERVASI];
Resep     daftarResep[MAX_RESEP];
Feedback  daftarFeedback[MAX_FEEDBACK];

int jumlahPasien    = 0;
int jumlahDokter    = 0;
int jumlahReservasi = 0;
int jumlahResep     = 0;
int jumlahFeedback  = 0;

int idPasienLogin = -1;
int idDokterLogin = -1;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    printf("\n  Tekan Enter untuk melanjutkan...");
    while (getchar() != '\n');
    getchar();
}

void printLine(char c, int len) {
    for (int i = 0; i < len; i++) putchar(c);
    putchar('\n');
}

void printHeader(const char* judul) {
    clearScreen();
    printLine('=', 60);
    printf("  DeOctor - Sistem Informasi Dokter\n");
    printLine('=', 60);
    int pad = (58 - (int)strlen(judul)) / 2;
    printf("  %*s%s\n", pad, "", judul);
    printLine('-', 60);
    printf("\n");
}

void getInputString(const char* prompt, char* buffer) {
    printf("  %s : ", prompt);
    fgets(buffer, MAX_STR, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void getInputInt(const char* prompt, int* val) {
    char tmp[MAX_STR];
    printf("  %s : ", prompt);
    fgets(tmp, MAX_STR, stdin);
    *val = atoi(tmp);
}

char* getCurrentDate() {
    static char buf[20];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info);
    return buf;
}

void simpanData() {
    FILE *f;

    f = fopen(FILE_PASIEN, "wb");
    if (f) {
        fwrite(&jumlahPasien, sizeof(int), 1, f);
        fwrite(daftarPasien, sizeof(Pasien), jumlahPasien, f);
        fclose(f);
    }

    f = fopen(FILE_DOKTER, "wb");
    if (f) {
        fwrite(&jumlahDokter, sizeof(int), 1, f);
        fwrite(daftarDokter, sizeof(Dokter), jumlahDokter, f);
        fclose(f);
    }

    f = fopen(FILE_RESERVASI, "wb");
    if (f) {
        fwrite(&jumlahReservasi, sizeof(int), 1, f);
        fwrite(daftarReservasi, sizeof(Reservasi), jumlahReservasi, f);
        fclose(f);
    }

    f = fopen(FILE_RESEP, "wb");
    if (f) {
        fwrite(&jumlahResep, sizeof(int), 1, f);
        fwrite(daftarResep, sizeof(Resep), jumlahResep, f);
        fclose(f);
    }

    f = fopen(FILE_FEEDBACK, "wb");
    if (f) {
        fwrite(&jumlahFeedback, sizeof(int), 1, f);
        fwrite(daftarFeedback, sizeof(Feedback), jumlahFeedback, f);
        fclose(f);
    }
}

int loadData() {
    FILE *f;
    int berhasil = 0;

    f = fopen(FILE_PASIEN, "rb");
    if (f) {
        fread(&jumlahPasien, sizeof(int), 1, f);
        fread(daftarPasien, sizeof(Pasien), jumlahPasien, f);
        fclose(f);
        berhasil = 1;
    }

    f = fopen(FILE_DOKTER, "rb");
    if (f) {
        fread(&jumlahDokter, sizeof(int), 1, f);
        fread(daftarDokter, sizeof(Dokter), jumlahDokter, f);
        fclose(f);
    }

    f = fopen(FILE_RESERVASI, "rb");
    if (f) {
        fread(&jumlahReservasi, sizeof(int), 1, f);
        fread(daftarReservasi, sizeof(Reservasi), jumlahReservasi, f);
        fclose(f);
    }

    f = fopen(FILE_RESEP, "rb");
    if (f) {
        fread(&jumlahResep, sizeof(int), 1, f);
        fread(daftarResep, sizeof(Resep), jumlahResep, f);
        fclose(f);
    }

    f = fopen(FILE_FEEDBACK, "rb");
    if (f) {
        fread(&jumlahFeedback, sizeof(int), 1, f);
        fread(daftarFeedback, sizeof(Feedback), jumlahFeedback, f);
        fclose(f);
    }

    return berhasil;
}

void inisialisasiData() {
    jumlahDokter = 3;

    daftarDokter[0].id = 1;
    strcpy(daftarDokter[0].username,  "dokter1");
    strcpy(daftarDokter[0].password,  "dokter123");
    strcpy(daftarDokter[0].nama,      "Dr. Budi Santoso");
    strcpy(daftarDokter[0].spesialis, "Umum");
    strcpy(daftarDokter[0].jadwal,    "Senin 08:00, Rabu 13:00, Jumat 09:00");
    daftarDokter[0].aktif = 1;

    daftarDokter[1].id = 2;
    strcpy(daftarDokter[1].username,  "dokter2");
    strcpy(daftarDokter[1].password,  "dokter123");
    strcpy(daftarDokter[1].nama,      "Dr. Siti Rahayu");
    strcpy(daftarDokter[1].spesialis, "Spesialis Anak");
    strcpy(daftarDokter[1].jadwal,    "Selasa 10:00, Kamis 14:00");
    daftarDokter[1].aktif = 1;

    daftarDokter[2].id = 3;
    strcpy(daftarDokter[2].username,  "dokter3");
    strcpy(daftarDokter[2].password,  "dokter123");
    strcpy(daftarDokter[2].nama,      "Dr. Ahmad Fauzi");
    strcpy(daftarDokter[2].spesialis, "Kardiologi");
    strcpy(daftarDokter[2].jadwal,    "Senin 13:00, Kamis 09:00");
    daftarDokter[2].aktif = 1;

    jumlahPasien = 2;

    daftarPasien[0].id = 1;
    strcpy(daftarPasien[0].username, "pasien1");
    strcpy(daftarPasien[0].password, "pasien123");
    strcpy(daftarPasien[0].nama,     "Andi Wijaya");
    strcpy(daftarPasien[0].email,    "andi@email.com");
    strcpy(daftarPasien[0].telepon,  "081234567890");
    daftarPasien[0].aktif = 1;

    daftarPasien[1].id = 2;
    strcpy(daftarPasien[1].username, "pasien2");
    strcpy(daftarPasien[1].password, "pasien123");
    strcpy(daftarPasien[1].nama,     "Rina Susanti");
    strcpy(daftarPasien[1].email,    "rina@email.com");
    strcpy(daftarPasien[1].telepon,  "089876543210");
    daftarPasien[1].aktif = 1;

    jumlahReservasi = 1;
    daftarReservasi[0].id        = 1;
    daftarReservasi[0].id_pasien = 1;
    daftarReservasi[0].id_dokter = 1;
    strcpy(daftarReservasi[0].waktu,  "Senin 08:00");
    strcpy(daftarReservasi[0].status, "Diterima");

    jumlahResep = 1;
    daftarResep[0].id           = 1;
    daftarResep[0].id_pasien    = 1;
    daftarResep[0].id_dokter    = 1;
    daftarResep[0].id_reservasi = 1;
    strcpy(daftarResep[0].obat,        "Paracetamol 500mg");
    strcpy(daftarResep[0].dosis,       "3x sehari sesudah makan");
    strcpy(daftarResep[0].keterangan,  "Untuk demam dan nyeri kepala");
    strcpy(daftarResep[0].tanggal,     "2024-01-15");

    // Simpan seed data ke file .dat
    simpanData();
}

void menuUtama();
void menuPasien();
void menuDokter();

void menuRegistrasiPasien() {
    printHeader("REGISTRASI PASIEN");
    if (jumlahPasien >= MAX_PASIEN) {
        printf("  [!] Kapasitas pasien penuh!\n");
        pauseScreen(); return;
    }
    Pasien p;
    p.id = jumlahPasien + 1;
    printf("  Isi data berikut:\n\n");
    getInputString("Nama Lengkap", p.nama);
    getInputString("Username",     p.username);
    for (int i = 0; i < jumlahPasien; i++) {
        if (strcmp(daftarPasien[i].username, p.username) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            pauseScreen(); return;
        }
    }
    getInputString("Password",   p.password);
    getInputString("Email",      p.email);
    getInputString("No. Telepon",p.telepon);
    p.aktif = 1;
    daftarPasien[jumlahPasien++] = p;
    simpanData();
    printf("\n  [+] Registrasi berhasil! Silakan login.\n");
    pauseScreen();
}

// Login Pasien - While Loop (3x kesempatan)
int loginPasien() {
    printHeader("LOGIN PASIEN");
    printf("  Anda memiliki 3x kesempatan login.\n\n");
    int attempts = 0;
    char username[MAX_STR], password[MAX_STR];

    while (attempts < MAX_LOGIN_ATTEMPTS) {
        printf("  Percobaan ke-%d:\n", attempts + 1);
        getInputString("Username", username);
        getInputString("Password", password);
        for (int i = 0; i < jumlahPasien; i++) {
            if (strcmp(daftarPasien[i].username, username) == 0 &&
                strcmp(daftarPasien[i].password, password) == 0 &&
                daftarPasien[i].aktif == 1) {
                idPasienLogin = i;
                printf("\n  [+] Login berhasil! Selamat datang, %s!\n",
                       daftarPasien[i].nama);
                pauseScreen();
                return 1;
            }
        }
        attempts++;
        if (attempts < MAX_LOGIN_ATTEMPTS)
            printf("  [!] Salah! Sisa %d percobaan.\n\n",
                   MAX_LOGIN_ATTEMPTS - attempts);
    }
    printf("\n  [!] Login gagal! Akun diblokir sementara.\n");
    pauseScreen();
    return 0;
}

void menuRegistrasiDokter() {
    printHeader("REGISTRASI DOKTER");
    if (jumlahDokter >= MAX_DOKTER) {
        printf("  [!] Kapasitas dokter penuh!\n");
        pauseScreen(); return;
    }
    Dokter d;
    d.id = jumlahDokter + 1;
    printf("  Isi data berikut:\n\n");
    getInputString("Nama Lengkap (beserta gelar)", d.nama);
    getInputString("Spesialis",   d.spesialis);
    getInputString("Username",    d.username);
    for (int i = 0; i < jumlahDokter; i++) {
        if (strcmp(daftarDokter[i].username, d.username) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            pauseScreen(); return;
        }
    }
    getInputString("Password",       d.password);
    getInputString("Jadwal Praktek", d.jadwal);
    d.aktif = 1;
    daftarDokter[jumlahDokter++] = d;
    simpanData();
    printf("\n  [+] Registrasi dokter berhasil! Silakan login.\n");
    pauseScreen();
}

// Login Dokter - Do-While Loop (3x kesempatan)
int loginDokter() {
    printHeader("LOGIN DOKTER");
    printf("  Anda memiliki 3x kesempatan login.\n\n");
    int attempts = 0;
    char username[MAX_STR], password[MAX_STR];

    do {
        printf("  Percobaan ke-%d:\n", attempts + 1);
        getInputString("Username", username);
        getInputString("Password", password);
        for (int i = 0; i < jumlahDokter; i++) {
            if (strcmp(daftarDokter[i].username, username) == 0 &&
                strcmp(daftarDokter[i].password, password) == 0 &&
                daftarDokter[i].aktif == 1) {
                idDokterLogin = i;
                printf("\n  [+] Login berhasil! Selamat datang, %s!\n",
                       daftarDokter[i].nama);
                pauseScreen();
                return 1;
            }
        }
        attempts++;
        if (attempts < MAX_LOGIN_ATTEMPTS)
            printf("  [!] Salah! Sisa %d percobaan.\n\n",
                   MAX_LOGIN_ATTEMPTS - attempts);
    } while (attempts < MAX_LOGIN_ATTEMPTS);

    printf("\n  [!] Login gagal! Akun diblokir sementara.\n");
    pauseScreen();
    return 0;
}

void lihatInfoAkunPasien() {
    printHeader("INFORMASI AKUN PASIEN");
    Pasien* p = &daftarPasien[idPasienLogin];
    printLine('-', 60);
    printf("  ID Pasien  : %d\n",  p->id);
    printf("  Nama       : %s\n",  p->nama);
    printf("  Username   : %s\n",  p->username);
    printf("  Email      : %s\n",  p->email);
    printf("  Telepon    : %s\n",  p->telepon);
    printf("  Status     : %s\n",  p->aktif ? "Aktif" : "Nonaktif");
    printLine('-', 60);
    pauseScreen();
}

void gantiCredentialPasien() {
    printHeader("GANTI USERNAME & PASSWORD");
    Pasien* p = &daftarPasien[idPasienLogin];
    char newUser[MAX_STR], newPass[MAX_STR], confirm[MAX_STR];
    getInputString("Username Baru", newUser);
    for (int i = 0; i < jumlahPasien; i++) {
        if (i != idPasienLogin &&
            strcmp(daftarPasien[i].username, newUser) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            pauseScreen(); return;
        }
    }
    getInputString("Password Baru",      newPass);
    getInputString("Konfirmasi Password", confirm);
    if (strcmp(newPass, confirm) != 0) {
        printf("\n  [!] Password tidak cocok!\n");
        pauseScreen(); return;
    }
    strcpy(p->username, newUser);
    strcpy(p->password, newPass);
    simpanData();
    printf("\n  [+] Berhasil diperbarui!\n");
    pauseScreen();
}

void manajemenAkunPasien() {
    int pilihan;
    do {
        printHeader("MANAJEMEN AKUN PASIEN");
        printf("  [1] Lihat Informasi Akun\n");
        printf("  [2] Ganti Username dan Password\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatInfoAkunPasien();
        else if (pilihan == 2) gantiCredentialPasien();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void lihatDaftarDokter() {
    printHeader("DAFTAR DOKTER");
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    for (int i = 0; i < jumlahDokter; i++)
        printf("  %-4d %-25s %-20s\n",
               daftarDokter[i].id,
               daftarDokter[i].nama,
               daftarDokter[i].spesialis);
    printLine('-', 60);
    pauseScreen();
}

void cariDokter() {
    printHeader("CARI DOKTER");
    char keyword[MAX_STR];
    getInputString("Nama / Spesialis", keyword);
    printf("\n  Hasil Pencarian:\n");
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    int ditemukan = 0;
    for (int i = 0; i < jumlahDokter; i++) {
        if (strstr(daftarDokter[i].nama,      keyword) ||
            strstr(daftarDokter[i].spesialis, keyword)) {
            printf("  %-4d %-25s %-20s\n",
                   daftarDokter[i].id,
                   daftarDokter[i].nama,
                   daftarDokter[i].spesialis);
            ditemukan++;
        }
    }
    if (!ditemukan) printf("  Tidak ada dokter yang ditemukan.\n");
    printLine('-', 60);
    pauseScreen();
}

void urutDokter() {
    printHeader("URUTKAN DATA DOKTER");
    printf("  [1] Berdasarkan Nama (A-Z)\n");
    printf("  [2] Berdasarkan Spesialis (A-Z)\n");
    printLine('-', 60);
    int pilihan;
    getInputInt("Pilihan", &pilihan);
    Dokter temp;
    for (int i = 0; i < jumlahDokter - 1; i++) {
        for (int j = 0; j < jumlahDokter - 1 - i; j++) {
            int swap = 0;
            if (pilihan == 1 && strcmp(daftarDokter[j].nama,      daftarDokter[j+1].nama)      > 0) swap = 1;
            if (pilihan == 2 && strcmp(daftarDokter[j].spesialis, daftarDokter[j+1].spesialis) > 0) swap = 1;
            if (swap) { temp = daftarDokter[j]; daftarDokter[j] = daftarDokter[j+1]; daftarDokter[j+1] = temp; }
        }
    }
    printf("\n  [+] Data berhasil diurutkan!\n\n");
    lihatDaftarDokter();
}

void menuListDokter() {
    int pilihan;
    do {
        printHeader("LIST DOKTER");
        printf("  [1] Lihat Daftar Dokter\n");
        printf("  [2] Cari Dokter\n");
        printf("  [3] Urutkan Data Dokter\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatDaftarDokter();
        else if (pilihan == 2) cariDokter();
        else if (pilihan == 3) urutDokter();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void buatReservasi() {
    printHeader("BUAT RESERVASI");
    lihatDaftarDokter();
    int idDoc;
    getInputInt("Pilih ID Dokter", &idDoc);
    int idxDok = -1;
    for (int i = 0; i < jumlahDokter; i++)
        if (daftarDokter[i].id == idDoc) { idxDok = i; break; }
    if (idxDok == -1) {
        printf("  [!] Dokter tidak ditemukan!\n");
        pauseScreen(); return;
    }
    printf("\n  Jadwal tersedia: %s\n\n", daftarDokter[idxDok].jadwal);
    Reservasi r;
    r.id        = jumlahReservasi + 1;
    r.id_pasien = daftarPasien[idPasienLogin].id;
    r.id_dokter = idDoc;
    getInputString("Waktu Reservasi (contoh: Senin 08:00)", r.waktu);
    strcpy(r.status, "Menunggu");
    daftarReservasi[jumlahReservasi++] = r;
    simpanData();
    printf("\n  [+] Reservasi berhasil! Status: Menunggu konfirmasi dokter.\n");
    pauseScreen();
}

void menuReservasiDokter() {
    printHeader("RESERVASI DOKTER");
    printf("  [1] Buat Reservasi Baru\n");
    printf("  [0] Kembali\n");
    printLine('-', 60);
    int p; getInputInt("Pilihan", &p);
    if (p == 1) buatReservasi();
}

void menuStatusReservasi() {
    printHeader("STATUS RESERVASI");
    int myId = daftarPasien[idPasienLogin].id;
    printLine('-', 70);
    printf("  %-4s %-22s %-18s %-12s\n", "ID", "Dokter", "Waktu", "Status");
    printLine('-', 70);
    int ada = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].id_pasien == myId) {
            char namaDok[MAX_STR] = "?";
            for (int j = 0; j < jumlahDokter; j++)
                if (daftarDokter[j].id == daftarReservasi[i].id_dokter)
                    { strcpy(namaDok, daftarDokter[j].nama); break; }
            printf("  %-4d %-22s %-18s %-12s\n",
                   daftarReservasi[i].id, namaDok,
                   daftarReservasi[i].waktu, daftarReservasi[i].status);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada reservasi.\n");
    printLine('-', 70);
    pauseScreen();
}

void lihatResepPasien() {
    printHeader("DAFTAR RESEP OBAT");
    int myId = daftarPasien[idPasienLogin].id;
    printLine('-', 60);
    int ada = 0;
    for (int i = 0; i < jumlahResep; i++) {
        if (daftarResep[i].id_pasien == myId) {
            char namaDok[MAX_STR] = "?";
            for (int j = 0; j < jumlahDokter; j++)
                if (daftarDokter[j].id == daftarResep[i].id_dokter)
                    { strcpy(namaDok, daftarDokter[j].nama); break; }
            printf("  ID Resep    : %d\n",  daftarResep[i].id);
            printf("  Dokter      : %s\n",  namaDok);
            printf("  Obat        : %s\n",  daftarResep[i].obat);
            printf("  Dosis       : %s\n",  daftarResep[i].dosis);
            printf("  Keterangan  : %s\n",  daftarResep[i].keterangan);
            printf("  Tanggal     : %s\n",  daftarResep[i].tanggal);
            printLine('-', 60);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada resep.\n");
    pauseScreen();
}

void cariResepPasien() {
    printHeader("CARI RESEP OBAT");
    int myId = daftarPasien[idPasienLogin].id;
    char keyword[MAX_STR];
    getInputString("Nama Obat", keyword);
    printf("\n  Hasil Pencarian:\n");
    printLine('-', 60);
    int ditemukan = 0;
    for (int i = 0; i < jumlahResep; i++) {
        if (daftarResep[i].id_pasien == myId &&
            strstr(daftarResep[i].obat, keyword)) {
            printf("  [%d] %s - %s\n",
                   daftarResep[i].id,
                   daftarResep[i].obat,
                   daftarResep[i].tanggal);
            ditemukan++;
        }
    }
    if (!ditemukan) printf("  Tidak ditemukan.\n");
    printLine('-', 60);
    pauseScreen();
}

void menuResepPasien() {
    int pilihan;
    do {
        printHeader("RESEP OBAT");
        printf("  [1] Lihat Daftar Resep\n");
        printf("  [2] Cari Resep\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatResepPasien();
        else if (pilihan == 2) cariResepPasien();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void beriFeedback() {
    printHeader("BERIKAN FEEDBACK");
    lihatDaftarDokter();
    int idDoc;
    getInputInt("Pilih ID Dokter", &idDoc);
    int idxDok = -1;
    for (int i = 0; i < jumlahDokter; i++)
        if (daftarDokter[i].id == idDoc) { idxDok = i; break; }
    if (idxDok == -1) {
        printf("  [!] Dokter tidak ditemukan!\n");
        pauseScreen(); return;
    }
    Feedback f;
    f.id        = jumlahFeedback + 1;
    f.id_pasien = daftarPasien[idPasienLogin].id;
    f.id_dokter = idDoc;
    strcpy(f.tanggal, getCurrentDate());
    getInputString("Tulis feedback Anda", f.isi);
    do {
        getInputInt("Rating (1-5)", &f.rating);
        if (f.rating < 1 || f.rating > 5) printf("  [!] Rating harus 1-5!\n");
    } while (f.rating < 1 || f.rating > 5);
    daftarFeedback[jumlahFeedback++] = f;
    simpanData();
    printf("\n  [+] Feedback berhasil dikirim!\n");
    pauseScreen();
}

void lihatHistoryFeedback() {
    printHeader("HISTORY FEEDBACK");
    int myId = daftarPasien[idPasienLogin].id;
    printLine('-', 60);
    int ada = 0;
    for (int i = 0; i < jumlahFeedback; i++) {
        if (daftarFeedback[i].id_pasien == myId) {
            char namaDok[MAX_STR] = "?";
            for (int j = 0; j < jumlahDokter; j++)
                if (daftarDokter[j].id == daftarFeedback[i].id_dokter)
                    { strcpy(namaDok, daftarDokter[j].nama); break; }
            printf("  Dokter   : %s\n",  namaDok);
            printf("  Feedback : %s\n",  daftarFeedback[i].isi);
            printf("  Rating   : ");
            for (int s = 0; s < daftarFeedback[i].rating; s++) printf("*");
            printf(" (%d/5)\n", daftarFeedback[i].rating);
            printf("  Tanggal  : %s\n",  daftarFeedback[i].tanggal);
            printLine('-', 60);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada feedback.\n");
    pauseScreen();
}

void menuFeedback() {
    int pilihan;
    do {
        printHeader("FEEDBACK");
        printf("  [1] Berikan Feedback ke Dokter\n");
        printf("  [2] Lihat History Feedback\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) beriFeedback();
        else if (pilihan == 2) lihatHistoryFeedback();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void menuPasien() {
    int pilihan;
    do {
        printHeader("MENU PASIEN");
        printf("  Halo, %s!\n\n", daftarPasien[idPasienLogin].nama);
        printf("  [1] Manajemen Akun\n");
        printf("  [2] List Dokter\n");
        printf("  [3] Reservasi Dokter\n");
        printf("  [4] Status Reservasi\n");
        printf("  [5] Resep Obat\n");
        printf("  [6] Feedback\n");
        printf("  [0] Logout\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        switch (pilihan) {
            case 1: manajemenAkunPasien();  break;
            case 2: menuListDokter();       break;
            case 3: menuReservasiDokter();  break;
            case 4: menuStatusReservasi();  break;
            case 5: menuResepPasien();      break;
            case 6: menuFeedback();         break;
            case 0:
                printf("\n  [+] Logout berhasil.\n");
                idPasienLogin = -1;
                pauseScreen(); return;
            default:
                printf("  [!] Pilihan tidak valid!\n");
                pauseScreen();
        }
    } while (1);
}

void lihatInfoAkunDokter() {
    printHeader("INFORMASI AKUN DOKTER");
    Dokter* d = &daftarDokter[idDokterLogin];
    printLine('-', 60);
    printf("  ID Dokter  : %d\n", d->id);
    printf("  Nama       : %s\n", d->nama);
    printf("  Spesialis  : %s\n", d->spesialis);
    printf("  Username   : %s\n", d->username);
    printf("  Jadwal     : %s\n", d->jadwal);
    printf("  Status     : %s\n", d->aktif ? "Aktif" : "Nonaktif");
    printLine('-', 60);
    pauseScreen();
}

void gantiCredentialDokter() {
    printHeader("GANTI USERNAME & PASSWORD");
    Dokter* d = &daftarDokter[idDokterLogin];
    char newUser[MAX_STR], newPass[MAX_STR], confirm[MAX_STR];
    getInputString("Username Baru", newUser);
    for (int i = 0; i < jumlahDokter; i++) {
        if (i != idDokterLogin &&
            strcmp(daftarDokter[i].username, newUser) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            pauseScreen(); return;
        }
    }
    getInputString("Password Baru",       newPass);
    getInputString("Konfirmasi Password", confirm);
    if (strcmp(newPass, confirm) != 0) {
        printf("\n  [!] Password tidak cocok!\n");
        pauseScreen(); return;
    }
    strcpy(d->username, newUser);
    strcpy(d->password, newPass);
    simpanData();
    printf("\n  [+] Berhasil diperbarui!\n");
    pauseScreen();
}

void manajemenAkunDokter() {
    int pilihan;
    do {
        printHeader("MANAJEMEN AKUN DOKTER");
        printf("  [1] Lihat Informasi Akun\n");
        printf("  [2] Ganti Username dan Password\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatInfoAkunDokter();
        else if (pilihan == 2) gantiCredentialDokter();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void lihatDaftarReservasi() {
    printHeader("DAFTAR RESERVASI PASIEN");
    int myId = daftarDokter[idDokterLogin].id;
    printLine('-', 70);
    printf("  %-4s %-25s %-18s %-12s\n", "ID", "Nama Pasien", "Waktu", "Status");
    printLine('-', 70);
    int ada = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].id_dokter == myId) {
            char namaPas[MAX_STR] = "?";
            for (int j = 0; j < jumlahPasien; j++)
                if (daftarPasien[j].id == daftarReservasi[i].id_pasien)
                    { strcpy(namaPas, daftarPasien[j].nama); break; }
            printf("  %-4d %-25s %-18s %-12s\n",
                   daftarReservasi[i].id, namaPas,
                   daftarReservasi[i].waktu, daftarReservasi[i].status);
            ada = 1;
        }
    }
    if (!ada) printf("  Tidak ada reservasi.\n");
    printLine('-', 70);
    pauseScreen();
}

void prosesReservasi() {
    printHeader("PROSES RESERVASI");
    lihatDaftarReservasi();
    int idRes;
    getInputInt("ID Reservasi yang diproses", &idRes);
    int idx = -1;
    for (int i = 0; i < jumlahReservasi; i++)
        if (daftarReservasi[i].id == idRes &&
            daftarReservasi[i].id_dokter == daftarDokter[idDokterLogin].id)
            { idx = i; break; }
    if (idx == -1) {
        printf("  [!] Reservasi tidak ditemukan!\n");
        pauseScreen(); return;
    }
    printf("\n  Status saat ini: %s\n", daftarReservasi[idx].status);
    printf("  [1] Terima\n  [2] Tolak\n");
    int aksi; getInputInt("Pilihan", &aksi);
    if      (aksi == 1) { strcpy(daftarReservasi[idx].status, "Diterima"); printf("\n  [+] Reservasi diterima!\n"); }
    else if (aksi == 2) { strcpy(daftarReservasi[idx].status, "Ditolak");  printf("\n  [+] Reservasi ditolak!\n"); }
    else { printf("  [!] Pilihan tidak valid!\n"); pauseScreen(); return; }
    simpanData();
    pauseScreen();
}

void menuReservasiPasien() {
    int pilihan;
    do {
        printHeader("RESERVASI PASIEN");
        printf("  [1] Lihat Daftar Reservasi\n");
        printf("  [2] Terima / Tolak Reservasi\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatDaftarReservasi();
        else if (pilihan == 2) prosesReservasi();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void lihatRiwayatPasien() {
    printHeader("RIWAYAT PASIEN");
    int myId = daftarDokter[idDokterLogin].id;
    printLine('-', 65);
    printf("  %-6s %-25s %-15s\n", "Res#", "Nama Pasien", "Telepon");
    printLine('-', 65);
    int ada = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].id_dokter == myId &&
            strcmp(daftarReservasi[i].status, "Diterima") == 0) {
            char namaPas[MAX_STR] = "?", telpPas[MAX_STR] = "?";
            for (int j = 0; j < jumlahPasien; j++)
                if (daftarPasien[j].id == daftarReservasi[i].id_pasien) {
                    strcpy(namaPas, daftarPasien[j].nama);
                    strcpy(telpPas, daftarPasien[j].telepon);
                    break;
                }
            printf("  %-6d %-25s %-15s\n",
                   daftarReservasi[i].id, namaPas, telpPas);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada riwayat pasien.\n");
    printLine('-', 65);
    pauseScreen();
}

void urutDataPasien() {
    printHeader("URUTKAN DATA PASIEN");
    printf("  [1] Berdasarkan Nama (A-Z)\n");
    printf("  [2] Berdasarkan ID\n");
    printLine('-', 60);
    int pilihan; getInputInt("Pilihan", &pilihan);
    Pasien temp;
    for (int i = 0; i < jumlahPasien - 1; i++) {
        for (int j = 0; j < jumlahPasien - 1 - i; j++) {
            int swap = 0;
            if (pilihan == 1 && strcmp(daftarPasien[j].nama, daftarPasien[j+1].nama) > 0) swap = 1;
            if (pilihan == 2 && daftarPasien[j].id > daftarPasien[j+1].id)               swap = 1;
            if (swap) { temp = daftarPasien[j]; daftarPasien[j] = daftarPasien[j+1]; daftarPasien[j+1] = temp; }
        }
    }
    printf("\n  [+] Data berhasil diurutkan!\n\n");
    printLine('-', 60);
    printf("  %-4s %-25s %-15s\n", "ID", "Nama", "Telepon");
    printLine('-', 60);
    for (int i = 0; i < jumlahPasien; i++)
        printf("  %-4d %-25s %-15s\n",
               daftarPasien[i].id, daftarPasien[i].nama, daftarPasien[i].telepon);
    printLine('-', 60);
    pauseScreen();
}

void menuHistoryPasien() {
    int pilihan;
    do {
        printHeader("HISTORY PASIEN");
        printf("  [1] Lihat Riwayat Pasien\n");
        printf("  [2] Urutkan Data Pasien\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if      (pilihan == 1) lihatRiwayatPasien();
        else if (pilihan == 2) urutDataPasien();
        else if (pilihan != 0) { printf("  [!] Tidak valid!\n"); pauseScreen(); }
    } while (pilihan != 0);
}

void buatResep() {
    printHeader("BUAT RESEP OBAT");
    int myId = daftarDokter[idDokterLogin].id;
    printf("  Pilih pasien dari reservasi yang diterima:\n\n");
    printLine('-', 65);
    printf("  %-6s %-25s %-15s\n", "ID Res", "Nama Pasien", "Waktu");
    printLine('-', 65);
    int ada = 0;
    for (int i = 0; i < jumlahReservasi; i++) {
        if (daftarReservasi[i].id_dokter == myId &&
            strcmp(daftarReservasi[i].status, "Diterima") == 0) {
            char namaPas[MAX_STR] = "?";
            for (int j = 0; j < jumlahPasien; j++)
                if (daftarPasien[j].id == daftarReservasi[i].id_pasien)
                    { strcpy(namaPas, daftarPasien[j].nama); break; }
            printf("  %-6d %-25s %-15s\n",
                   daftarReservasi[i].id, namaPas, daftarReservasi[i].waktu);
            ada = 1;
        }
    }
    if (!ada) {
        printf("  Tidak ada pasien yang bisa diberi resep.\n");
        pauseScreen(); return;
    }
    printLine('-', 65);
    int idRes; getInputInt("ID Reservasi", &idRes);
    int idxRes = -1;
    for (int i = 0; i < jumlahReservasi; i++)
        if (daftarReservasi[i].id == idRes && daftarReservasi[i].id_dokter == myId)
            { idxRes = i; break; }
    if (idxRes == -1) {
        printf("  [!] Reservasi tidak valid!\n");
        pauseScreen(); return;
    }
    Resep r;
    r.id           = jumlahResep + 1;
    r.id_pasien    = daftarReservasi[idxRes].id_pasien;
    r.id_dokter    = myId;
    r.id_reservasi = idRes;
    strcpy(r.tanggal, getCurrentDate());
    getInputString("Nama Obat",    r.obat);
    getInputString("Dosis",        r.dosis);
    getInputString("Keterangan",   r.keterangan);
    daftarResep[jumlahResep++] = r;
    simpanData();
    printf("\n  [+] Resep berhasil disimpan!\n");
    pauseScreen();
}

void menuResepDokter() {
    int pilihan;
    do {
        printHeader("RESEP OBAT");
        printf("  [1] Buat Resep Baru\n");
        printf("  [2] Lihat Semua Resep\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        if (pilihan == 1) {
            buatResep();
        } else if (pilihan == 2) {
            printHeader("RESEP YANG DIBUAT");
            int myId = daftarDokter[idDokterLogin].id;
            printLine('-', 65);
            int ada = 0;
            for (int i = 0; i < jumlahResep; i++) {
                if (daftarResep[i].id_dokter == myId) {
                    char namaPas[MAX_STR] = "?";
                    for (int j = 0; j < jumlahPasien; j++)
                        if (daftarPasien[j].id == daftarResep[i].id_pasien)
                            { strcpy(namaPas, daftarPasien[j].nama); break; }
                    printf("  Resep #%-3d | Pasien: %-20s | Obat: %s\n",
                           daftarResep[i].id, namaPas, daftarResep[i].obat);
                    printf("  Dosis: %-30s | Tgl: %s\n",
                           daftarResep[i].dosis, daftarResep[i].tanggal);
                    printLine('-', 65);
                    ada = 1;
                }
            }
            if (!ada) printf("  Belum ada resep.\n");
            pauseScreen();
        } else if (pilihan != 0) {
            printf("  [!] Tidak valid!\n"); pauseScreen();
        }
    } while (pilihan != 0);
}

void menuFeedbackDokter() {
    printHeader("FEEDBACK DARI PASIEN");
    int myId = daftarDokter[idDokterLogin].id;
    printLine('-', 60);
    int ada = 0;
    for (int i = 0; i < jumlahFeedback; i++) {
        if (daftarFeedback[i].id_dokter == myId) {
            char namaPas[MAX_STR] = "?";
            for (int j = 0; j < jumlahPasien; j++)
                if (daftarPasien[j].id == daftarFeedback[i].id_pasien)
                    { strcpy(namaPas, daftarPasien[j].nama); break; }
            printf("  Dari     : %s\n",  namaPas);
            printf("  Feedback : %s\n",  daftarFeedback[i].isi);
            printf("  Rating   : ");
            for (int s = 0; s < daftarFeedback[i].rating; s++) printf("*");
            printf(" (%d/5)\n", daftarFeedback[i].rating);
            printf("  Tanggal  : %s\n",  daftarFeedback[i].tanggal);
            printLine('-', 60);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada feedback dari pasien.\n");
    pauseScreen();
}

void menuDokter() {
    int pilihan;
    do {
        printHeader("MENU DOKTER");
        printf("  Halo, %s!\n\n", daftarDokter[idDokterLogin].nama);
        printf("  [1] Manajemen Akun\n");
        printf("  [2] Reservasi Pasien\n");
        printf("  [3] History Pasien\n");
        printf("  [4] Resep Obat\n");
        printf("  [5] Feedback Pasien\n");
        printf("  [0] Logout\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        switch (pilihan) {
            case 1: manajemenAkunDokter();  break;
            case 2: menuReservasiPasien();  break;
            case 3: menuHistoryPasien();    break;
            case 4: menuResepDokter();      break;
            case 5: menuFeedbackDokter();   break;
            case 0:
                printf("\n  [+] Logout berhasil.\n");
                idDokterLogin = -1;
                pauseScreen(); return;
            default:
                printf("  [!] Pilihan tidak valid!\n");
                pauseScreen();
        }
    } while (1);
}

void menuUtama() {
    int pilihan;
    do {
        printHeader("SELAMAT DATANG");
        printf("  [1] Login / Registrasi sebagai Pasien\n");
        printf("  [2] Login / Registrasi sebagai Dokter\n");
        printf("  [0] Keluar\n");
        printLine('-', 60);
        getInputInt("Pilihan", &pilihan);
        switch (pilihan) {
            case 1: {
                int sub;
                printHeader("MENU PASIEN");
                printf("  [1] Login Pasien\n");
                printf("  [2] Registrasi Pasien\n");
                printf("  [0] Kembali\n");
                printLine('-', 60);
                getInputInt("Pilihan", &sub);
                if      (sub == 1) { if (loginPasien()) menuPasien(); }
                else if (sub == 2) menuRegistrasiPasien();
                break;
            }
            case 2: {
                int sub;
                printHeader("MENU DOKTER");
                printf("  [1] Login Dokter\n");
                printf("  [2] Registrasi Dokter\n");
                printf("  [0] Kembali\n");
                printLine('-', 60);
                getInputInt("Pilihan", &sub);
                if      (sub == 1) { if (loginDokter()) menuDokter(); }
                else if (sub == 2) menuRegistrasiDokter();
                break;
            }
            case 0:
                printHeader("SAMPAI JUMPA!");
                printf("  Terima kasih telah menggunakan DeOctor.\n\n");
                exit(0);
            default:
                printf("  [!] Pilihan tidak valid!\n");
                pauseScreen();
        }
    } while (1);
}

int main() {
    // Coba load dari file .dat dulu
    // Kalau belum ada (pertama kali), pakai data seed lalu simpan
    if (!loadData()) {
        inisialisasiData();
    }
    menuUtama();
    return 0;
}