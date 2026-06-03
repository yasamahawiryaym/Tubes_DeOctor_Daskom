#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//yasagendut
// ===== CONSTANTS =====
#define MAX_PASIEN     100 //konstan
#define MAX_DOKTER     50
#define MAX_RESERVASI  200
#define MAX_RESEP      200
#define MAX_FEEDBACK   200
#define MAX_STR        100
#define MAX_LOGIN_ATTEMPTS 3

// ===== FILE NAMES =====
#define FILE_PASIEN    "pasien.dat" //konstan file pasien
#define FILE_DOKTER    "dokter.dat"
#define FILE_RESERVASI "reservasi.dat"
#define FILE_RESEP     "resep.dat"
#define FILE_FEEDBACK  "feedback.dat"

// ===== STRUCTS =====
typedef struct { // typedef untuk mempersingkat waktu coding, tidak perlu tulis struct lagi
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

void printLine(char c, int len) {
    for (int i = 0; i < len; i++) putchar(c);
    putchar('\n');
}

void printHeader(const char* judul) {
    system("cls");
    printLine('=', 60);
    printf("  DeOctor - Sistem Informasi Dokter\n");
    printLine('=', 60);
    int pad = (58 - (int)strlen(judul)) / 2;
    printf("  %*s%s\n", pad, "", judul);
    printLine('-', 60);
    printf("\n");
}

char* getCurrentDate() {
    static char buf[20];
    time_t t = time(NULL); //nyimpan waktu dari komputer namanya epoch 
    struct tm* tm_info = localtime(&t); 
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info); // tahun bulan hari dalam lokal time
    return buf;
}

void loadData() { //untuk membaca isi file
    FILE *f;

    f = fopen(FILE_PASIEN, "rb");
    if (f) {
        fread(&jumlahPasien, sizeof(int), 1, f);
        fread(daftarPasien, sizeof(Pasien), jumlahPasien, f);
        fclose(f);
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
}

//===== MENU FORWARD DECLARATIONS =====
void menuUtama(); //Yasa
void menuPasien(); //Yasa
void menuDokter(); //Lulu

void menuRegistrasiPasien() { //Zalfa
    printHeader("REGISTRASI PASIEN");
    if (jumlahPasien >= MAX_PASIEN) {
        printf("  [!] Kapasitas pasien penuh!\n");
        system("pause");
        return;
    }
    
    Pasien p;
    p.id = jumlahPasien + 1;
    printf("  Isi data berikut:\n\n");

    printf("  Nama Lengkap : ");
    gets(p.nama);

    printf("  Username     : ");
    gets(p.username);

    for (int i = 0; i < jumlahPasien; i++) {
        if (strcmp(daftarPasien[i].username, p.username) == 0) { 
            printf("\n  [!] Username sudah digunakan!\n");
            system("pause");
            return;
        }
    }
    printf("  Password     : ");
    gets(p.password);

    printf("  Email        : ");
    gets(p.email);

    printf("  No. Telepon  : ");
    gets(p.telepon);

    p.aktif = 1;
    daftarPasien[jumlahPasien++] = p;

    FILE *fp = fopen(FILE_PASIEN, "wb");
    if (fp) {
        fwrite(&jumlahPasien, sizeof(int), 1, fp);
        fwrite(daftarPasien, sizeof(Pasien), jumlahPasien, fp);
        fclose(fp);
    }

    printf("\n  [+] Registrasi berhasil! Silakan login.\n");
    system("pause");
}

int loginPasien() { //Zalfa
    printHeader("LOGIN PASIEN");
    printf("  Anda memiliki 3x kesempatan login.\n\n");
    
    int attempts = 0;
    char username[MAX_STR], password[MAX_STR];

    while (attempts < MAX_LOGIN_ATTEMPTS) {
        printf("  Percobaan ke-%d:\n", attempts + 1);

        printf("  Username : ");
        gets(username);

        printf("  Password : ");
        gets(password);

        for (int i = 0; i < jumlahPasien; i++) {
            if (strcmp(daftarPasien[i].username, username) == 0 && //strcmp nilai ASCII dari string, kalau 0 berarti sama
                strcmp(daftarPasien[i].password, password) == 0 &&
                daftarPasien[i].aktif == 1) { //kalo bukan nol berarti beda
                idPasienLogin = i;
                printf("\n  [+] Login berhasil! Selamat datang, %s!\n", daftarPasien[i].nama);
                system("pause");
                return 1;
            }
        }

        attempts++;
        if (attempts < MAX_LOGIN_ATTEMPTS)
                        printf("  [!] Salah! Sisa %d percobaan.\n\n", MAX_LOGIN_ATTEMPTS - attempts);
    }

    printf("\n  [!] Login gagal! Akun diblokir sementara.\n");
   system("pause");
    return 0;
}

void lihatInfoAkunPasien() { //Rara
    printHeader("INFORMASI AKUN PASIEN");
    Pasien* p = &daftarPasien[idPasienLogin];
    printLine('-', 60);
    printf("  ID Pasien  : %d\n",  p->id);
    printf("  Nama       : %s\n",  p->nama);
    printf("  Username   : %s\n",  p->username);
    printf("  Email      : %s\n",  p->email);
    printf("  Telepon    : %s\n",  p->telepon);
    printf("  Status     : %s\n",  p->aktif ? "Aktif" : "Nonaktif");//seperti if atau else, aktif itu ngecek aktif
    printLine('-', 60);
   system("pause");
}

void gantiCredentialPasien() { //Rara
    printHeader("GANTI USERNAME & PASSWORD");
    Pasien* p = &daftarPasien[idPasienLogin];
    char newUser[MAX_STR], newPass[MAX_STR], confirm[MAX_STR];

    printf("  Username Baru        : ");
    gets(newUser);

    for (int i = 0; i < jumlahPasien; i++) {
        if (i != idPasienLogin && strcmp(daftarPasien[i].username, newUser) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            system("pause");
            return;
        }
    }
    
    printf("  Password Baru        : ");
    gets(newPass);

    printf("  Konfirmasi Password  : ");
    gets(confirm);

    if (strcmp(newPass, confirm) != 0) {
        printf("\n  [!] Password tidak cocok!\n");
        system("pause");
        return;
    }
    
    strcpy(p->username, newUser);
    strcpy(p->password, newPass);
    
    FILE *fp = fopen(FILE_PASIEN, "wb");
    if (fp) {
        fwrite(&jumlahPasien, sizeof(int), 1, fp);
        fwrite(daftarPasien, sizeof(Pasien), jumlahPasien, fp);
        fclose(fp);
    }

    printf("\n  [+] Berhasil diperbarui!\n");
    system("pause");
}

void manajemenAkunPasien() {//Rara
    int pilihan;
    do {
        printHeader("MANAJEMEN AKUN PASIEN");
        printf("  [1] Lihat Informasi Akun\n");
        printf("  [2] Ganti Username dan Password\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatInfoAkunPasien(); break;
            case 2: gantiCredentialPasien(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void lihatDaftarDokter() {//Lulu
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
    system("pause");
}

void cariDokter() {//Lulu
    printHeader("CARI DOKTER");
    char keyword[MAX_STR];
    printf("  Nama / Spesialis : ");
    gets(keyword);

    printf("\n  Hasil Pencarian:\n");
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    
    int ditemukan = 0;
    for (int i = 0; i < jumlahDokter; i++) {
        if (strstr(daftarDokter[i].nama, keyword) ||
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
    system("pause");
}

void urutDokter() { //Lulu
    printHeader("URUTKAN DATA DOKTER");
    printf("  [1] Berdasarkan Nama (A-Z)\n");
    printf("  [2] Berdasarkan Spesialis (A-Z)\n");
    printLine('-', 60);
    printf("  Pilihan : ");
    int pilihan;
    scanf("%d", &pilihan);
    getchar();
    
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
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    for (int i = 0; i < jumlahDokter; i++)
        printf("  %-4d %-25s %-20s\n",
               daftarDokter[i].id,
               daftarDokter[i].nama,
               daftarDokter[i].spesialis);
    printLine('-', 60);
    system("pause");
}

void menuListDokter() { //Lulu
    int pilihan;
    do {
        printHeader("LIST DOKTER");
        printf("  [1] Lihat Daftar Dokter\n");
        printf("  [2] Cari Dokter\n");
        printf("  [3] Urutkan Data Dokter\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatDaftarDokter(); break;
            case 2: cariDokter(); break;
            case 3: urutDokter(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void buatReservasi() { //Yasa
    printHeader("BUAT RESERVASI");

    // Tampilkan daftar dokter inline
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    for (int i = 0; i < jumlahDokter; i++)
        printf("  %-4d %-25s %-20s\n",
               daftarDokter[i].id,
               daftarDokter[i].nama,
               daftarDokter[i].spesialis);
    printLine('-', 60);

    int idDoc;
    printf("  Pilih ID Dokter : ");
    scanf("%d", &idDoc);
    getchar();

    int idxDok = -1;
    for (int i = 0; i < jumlahDokter; i++)
        if (daftarDokter[i].id == idDoc) { idxDok = i; break; }
    
    if (idxDok == -1) {
        printf("  [!] Dokter tidak ditemukan!\n");
        system("pause");
        return;
    }
    
    printf("\n  Jadwal tersedia: %s\n\n", daftarDokter[idxDok].jadwal);
    
    Reservasi r;
    r.id        = jumlahReservasi + 1;
    r.id_pasien = daftarPasien[idPasienLogin].id;
    r.id_dokter = idDoc;
    
    printf("  Waktu Reservasi (contoh: Senin 08:00) : ");
    gets(r.waktu);

    strcpy(r.status, "Menunggu");
    daftarReservasi[jumlahReservasi++] = r;

    FILE *fp = fopen(FILE_RESERVASI, "wb");
    if (fp) {
        fwrite(&jumlahReservasi, sizeof(int), 1, fp);
        fwrite(daftarReservasi, sizeof(Reservasi), jumlahReservasi, fp);
        fclose(fp);
    }

    printf("\n  [+] Reservasi berhasil! Status: Menunggu konfirmasi dokter.\n");
    system("pause");
}

void menuReservasiDokter() { //Yasa
    int pilihan;
    printHeader("RESERVASI DOKTER");
    printf("  [1] Buat Reservasi Baru\n");
    printf("  [0] Kembali\n");
    printLine('-', 60);
    printf("  Pilihan : ");
    scanf("%d", &pilihan);
    getchar();

    switch (pilihan) {
        case 1: buatReservasi(); break;
        case 0: break;
        default:
            printf("  [!] Pilihan tidak valid!\n");
            system("pause");
    }
}

void menuStatusReservasi() { //Yasa
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
    system("pause");
}

void lihatResepPasien() { //Naila
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
    system("pause");
}

void cariResepPasien() { //Naila
    printHeader("CARI RESEP OBAT");
    int myId = daftarPasien[idPasienLogin].id;
    char keyword[MAX_STR];
    
    printf("  Nama Obat : ");
    gets(keyword);

    printf("\n  Hasil Pencarian:\n");
    printLine('-', 60);
    int ditemukan = 0;
    for (int i = 0; i < jumlahResep; i++) {
       if (daftarResep[i].id_pasien == myId && strstr(daftarResep[i].obat, keyword)) {
            printf("  [%d] %s - %s\n",
                   daftarResep[i].id,
                   daftarResep[i].obat,
                   daftarResep[i].tanggal);
            ditemukan++;
        }
    }
    if (!ditemukan) printf("  Tidak ditemukan.\n");
    printLine('-', 60);
    system("pause");
}

void menuResepPasien() { //Naila
    int pilihan;
    do {
        printHeader("RESEP OBAT");
        printf("  [1] Lihat Daftar Resep\n");
        printf("  [2] Cari Resep\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatResepPasien(); break;
            case 2: cariResepPasien(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void beriFeedback() { //Fadel
    printHeader("BERIKAN FEEDBACK");
    printLine('-', 60);
    printf("  %-4s %-25s %-20s\n", "ID", "Nama Dokter", "Spesialis");
    printLine('-', 60);
    for (int i = 0; i < jumlahDokter; i++)
        printf("  %-4d %-25s %-20s\n",
               daftarDokter[i].id,
               daftarDokter[i].nama,
               daftarDokter[i].spesialis);
    printLine('-', 60);

    int idDoc;
    printf("  Pilih ID Dokter : ");
    scanf("%d", &idDoc);
    getchar();

    int idxDok = -1;
    for (int i = 0; i < jumlahDokter; i++)
        if (daftarDokter[i].id == idDoc) { idxDok = i; break; }
    
    if (idxDok == -1) {
        printf("  [!] Dokter tidak ditemukan!\n");
        system("pause");
        return;
    }
    
    Feedback f;
    f.id        = jumlahFeedback + 1;
    f.id_pasien = daftarPasien[idPasienLogin].id;
    f.id_dokter = idDoc;
    strcpy(f.tanggal, getCurrentDate());

    printf("  Tulis feedback Anda : ");
    gets(f.isi);

    do {
        printf("  Rating (1-5) : ");
        scanf("%d", &f.rating);
        getchar();
        if (f.rating < 1 || f.rating > 5) printf("  [!] Rating harus 1-5!\n");
    } while (f.rating < 1 || f.rating > 5);
    daftarFeedback[jumlahFeedback++] = f;

    FILE *fp = fopen(FILE_FEEDBACK, "wb");
    if (fp) {
        fwrite(&jumlahFeedback, sizeof(int), 1, fp);
        fwrite(daftarFeedback, sizeof(Feedback), jumlahFeedback, fp);
        fclose(fp);
    }

    printf("\n  [+] Feedback berhasil dikirim!\n");
    system("pause");
}

void lihatHistoryFeedback() { //Fadel
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
    system("pause");
}

void menuFeedback() { //Fadel
    int pilihan;
    do {
        printHeader("FEEDBACK");
        printf("  [1] Berikan Feedback ke Dokter\n");
        printf("  [2] Lihat History Feedback\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: beriFeedback(); break;
            case 2: lihatHistoryFeedback(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void menuPasien() { //Yasa
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
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: manajemenAkunPasien(); break;
            case 2: menuListDokter(); break;
            case 3: menuReservasiDokter(); break;
            case 4: menuStatusReservasi(); break;
            case 5: menuResepPasien(); break;
            case 6: menuFeedback(); break;
            case 0:
                printf("\n  [+] Logout berhasil.\n");
                idPasienLogin = -1;
                system("pause");
                return;
            default:
                printf("  [!] Pilihan tidak valid!\n");
                system("pause");
        }
    } while (1);
}

void menuRegistrasiDokter() { //Zalfa
    printHeader("REGISTRASI DOKTER");
    if (jumlahDokter >= MAX_DOKTER) {
        printf("  [!] Kapasitas dokter penuh!\n");
        system("pause");
        return;
    }

    Dokter d;
    d.id = jumlahDokter + 1;
    printf("  Isi data berikut:\n\n");

    printf("  Nama Lengkap (beserta gelar) : ");
    gets(d.nama);

    printf("  Spesialis                    : ");
    gets(d.spesialis);

    printf("  Username                     : ");
    gets(d.username);

    for (int i = 0; i < jumlahDokter; i++) {
        if (strcmp(daftarDokter[i].username, d.username) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            system("pause");
            return;
        }
    }

    printf("  Password                     : ");
    gets(d.password);

    printf("  Jadwal Praktek               : ");
    gets(d.jadwal);

    d.aktif = 1;
    daftarDokter[jumlahDokter++] = d;

    FILE *fp = fopen(FILE_DOKTER, "wb");
    if (fp) {
        fwrite(&jumlahDokter, sizeof(int), 1, fp);
        fwrite(daftarDokter, sizeof(Dokter), jumlahDokter, fp);
        fclose(fp);
    }

    printf("\n  [+] Registrasi dokter berhasil! Silakan login.\n");
    system("pause");
}

int loginDokter() { //Zalfa
    printHeader("LOGIN DOKTER");
    printf("  Anda memiliki 3x kesempatan login.\n\n");

    int attempts = 0;
    char username[MAX_STR], password[MAX_STR];

    do {
        printf("  Percobaan ke-%d:\n", attempts + 1);

        printf("  Username : ");
        gets(username);

        printf("  Password : ");
        gets(password);

        for (int i = 0; i < jumlahDokter; i++) {
            if (strcmp(daftarDokter[i].username, username) == 0 &&
                strcmp(daftarDokter[i].password, password) == 0 &&
                daftarDokter[i].aktif == 1) {
                idDokterLogin = i;
                printf("\n  [+] Login berhasil! Selamat datang, %s!\n", daftarDokter[i].nama);
                system("pause");
                return 1;
            }
        }

        attempts++;
        if (attempts < MAX_LOGIN_ATTEMPTS)
            printf("  [!] Salah! Sisa %d percobaan.\n\n", MAX_LOGIN_ATTEMPTS - attempts);

    } while (attempts < MAX_LOGIN_ATTEMPTS);

    printf("\n  [!] Login gagal! Akun diblokir sementara.\n");
    system("pause");
    return 0;
}

void lihatInfoAkunDokter() { //Rara
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
    system("pause");
}

void gantiCredentialDokter() { //Rara
    printHeader("GANTI USERNAME & PASSWORD");
    Dokter* d = &daftarDokter[idDokterLogin];
    char newUser[MAX_STR], newPass[MAX_STR], confirm[MAX_STR];

    printf("  Username Baru        : ");
    gets(newUser);

    for (int i = 0; i < jumlahDokter; i++) {
        if (i != idDokterLogin && strcmp(daftarDokter[i].username, newUser) == 0) {
            printf("\n  [!] Username sudah digunakan!\n");
            system("pause");
            return;
        }
    }

    printf("  Password Baru        : ");
    gets(newPass);

    printf("  Konfirmasi Password  : ");
    gets(confirm);

    if (strcmp(newPass, confirm) != 0) {
        printf("\n  [!] Password tidak cocok!\n");
        system("pause");
        return;
    }
    
    strcpy(d->username, newUser);
    strcpy(d->password, newPass);
    
    FILE *fp = fopen(FILE_DOKTER, "wb");
    if (fp) {
        fwrite(&jumlahDokter, sizeof(int), 1, fp);
        fwrite(daftarDokter, sizeof(Dokter), jumlahDokter, fp);
        fclose(fp);
    }

    printf("\n  [+] Berhasil diperbarui!\n");
    system("pause");
}

void manajemenAkunDokter() { //Rara
    int pilihan;
    do {
        printHeader("MANAJEMEN AKUN DOKTER");
        printf("  [1] Lihat Informasi Akun\n");
        printf("  [2] Ganti Username dan Password\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatInfoAkunDokter(); break;
            case 2: gantiCredentialDokter(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void lihatDaftarReservasi() { //Lulu
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
    system("pause");
}

void prosesReservasi() {//Lulu
    printHeader("PROSES RESERVASI");
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
    if (!ada) { printf("  Tidak ada reservasi.\n"); printLine('-', 70); system("pause"); return; }
    printLine('-', 70);

    int idRes;
    printf("  ID Reservasi yang diproses : ");
    scanf("%d", &idRes);
    getchar();

    int idx = -1;
    for (int i = 0; i < jumlahReservasi; i++)
        if (daftarReservasi[i].id == idRes &&
            daftarReservasi[i].id_dokter == daftarDokter[idDokterLogin].id)
            { idx = i; break; }
    
    if (idx == -1) {
        printf("  [!] Reservasi tidak ditemukan!\n");
        system("pause");
        return;
    }
    
    printf("\n  Status saat ini: %s\n", daftarReservasi[idx].status);
    printf("  [1] Terima\n  [2] Tolak\n");
    printf("  Pilihan : ");
    int aksi;
    scanf("%d", &aksi);
    getchar();

    switch (aksi) {
        case 1:
            strcpy(daftarReservasi[idx].status, "Diterima");
            printf("\n  [+] Reservasi diterima!\n");
            break;
        case 2:
            strcpy(daftarReservasi[idx].status, "Ditolak");
            printf("\n  [+] Reservasi ditolak!\n");
            break;
        default:
            printf("  [!] Pilihan tidak valid!\n");
            system("pause");
            return;
    }

    FILE *fp = fopen(FILE_RESERVASI, "wb");
    if (fp) {
        fwrite(&jumlahReservasi, sizeof(int), 1, fp);
        fwrite(daftarReservasi, sizeof(Reservasi), jumlahReservasi, fp);
        fclose(fp);
    }

    system("pause");
}

void menuReservasiPasien() { //Lulu
    int pilihan;
    do {
        printHeader("RESERVASI PASIEN");
        printf("  [1] Lihat Daftar Reservasi\n");
        printf("  [2] Terima / Tolak Reservasi\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatDaftarReservasi(); break;
            case 2: prosesReservasi(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void lihatRiwayatPasien() { //Yasa
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
            printf("  %-6d %-25s %-15s\n", daftarReservasi[i].id, namaPas, telpPas);
            ada = 1;
        }
    }
    if (!ada) printf("  Belum ada riwayat pasien.\n");
    printLine('-', 65);
    system("pause");
}

void urutDataPasien() { //Yasa
    printHeader("URUTKAN DATA PASIEN");
    printf("  [1] Berdasarkan Nama (A-Z)\n");
    printf("  [2] Berdasarkan ID\n");
    printLine('-', 60);
    printf("  Pilihan : ");
    int pilihan;
    scanf("%d", &pilihan);
    getchar();

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
    system("pause");
}

void menuHistoryPasien() { //Yasa
    int pilihan;
    do {
        printHeader("HISTORY PASIEN");
        printf("  [1] Lihat Riwayat Pasien\n");
        printf("  [2] Urutkan Data Pasien\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatRiwayatPasien(); break;
            case 2: urutDataPasien(); break;
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void buatResep() { //Naila
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
        system("pause");
        return;
    }
    printLine('-', 65);
    int idRes;
    printf("  ID Reservasi : ");
    scanf("%d", &idRes);
    getchar();

    int idxRes = -1;
    for (int i = 0; i < jumlahReservasi; i++)
        if (daftarReservasi[i].id == idRes && daftarReservasi[i].id_dokter == myId)
            { idxRes = i; break; }
    
    if (idxRes == -1) {
        printf("  [!] Reservasi tidak valid!\n");
        system("pause");
        return;
    }
    
    Resep r;
    r.id           = jumlahResep + 1;
    r.id_pasien    = daftarReservasi[idxRes].id_pasien;
    r.id_dokter    = myId;
    r.id_reservasi = idRes;
    strcpy(r.tanggal, getCurrentDate());
    printf("  Nama Obat   : ");
    gets(r.obat);

    printf("  Dosis       : ");
    gets(r.dosis);

    printf("  Keterangan  : ");
    gets(r.keterangan);

    daftarResep[jumlahResep++] = r;
    FILE *fp = fopen(FILE_RESEP, "wb");
    if (fp) {
        fwrite(&jumlahResep, sizeof(int), 1, fp);
        fwrite(daftarResep, sizeof(Resep), jumlahResep, fp);
        fclose(fp);
    }

    printf("\n  [+] Resep berhasil disimpan!\n");
    system("pause");
}

void menuResepDokter() { //Naila
    int pilihan;
    do {
        printHeader("RESEP OBAT");
        printf("  [1] Buat Resep Baru\n");
        printf("  [2] Lihat Semua Resep\n");
        printf("  [0] Kembali\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1:
                buatResep();
                break;
            case 2: {
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
                system("pause");
                break;
            }
            case 0: break;
            default:
                printf("  [!] Tidak valid!\n");
                system("pause");
        }
    } while (pilihan != 0);
}

void menuFeedbackDokter() { //Fadel
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
    system("pause");
}

void menuDokter() { //Lulu
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
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: manajemenAkunDokter(); break;
            case 2: menuReservasiPasien(); break;
            case 3: menuHistoryPasien(); break;
            case 4: menuResepDokter(); break;
            case 5: menuFeedbackDokter(); break;
            case 0:
                printf("\n  [+] Logout berhasil.\n");
                idDokterLogin = -1;
                system("pause");
                return;
            default:
                printf("  [!] Pilihan tidak valid!\n");
                system("pause");
        }
    } while (1);
}

void menuUtama() { //Yasa
    int pilihan;
    do {
        printHeader("SELAMAT DATANG");
        printf("  [1] Login / Registrasi sebagai Pasien\n");
        printf("  [2] Login / Registrasi sebagai Dokter\n");
        printf("  [0] Keluar\n");
        printLine('-', 60);
        printf("  Pilihan : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: {
                int sub;
                printHeader("MENU PASIEN");
                printf("  [1] Login Pasien\n");
                printf("  [2] Registrasi Pasien\n");
                printf("  [0] Kembali\n");
                printLine('-', 60);
                printf("  Pilihan : ");
                scanf("%d", &sub);
                getchar();

                switch (sub) {
                    case 1: if (loginPasien()) menuPasien(); break;
                    case 2: menuRegistrasiPasien(); break;
                    case 0: break;
                    default:
                        printf("  [!] Pilihan tidak valid!\n");
                        system("pause");
                }
                break;
            }
            case 2: {
                int sub;
                printHeader("MENU DOKTER");
                printf("  [1] Login Dokter\n");
                printf("  [2] Registrasi Dokter\n");
                printf("  [0] Kembali\n");
                printLine('-', 60);
                printf("  Pilihan : ");
                scanf("%d", &sub);
                getchar();

                switch (sub) {
                    case 1: if (loginDokter()) menuDokter(); break;
                    case 2: menuRegistrasiDokter(); break;
                    case 0: break;
                    default:
                        printf("  [!] Pilihan tidak valid!\n");
                        system("pause");
                }
                break;
            }
            case 0:
                system("cls");
                printf("  Terima kasih telah menggunakan DeOctor.\n\n");
                exit(0);
            default:
                printf("  [!] Pilihan tidak valid!\n");
                system("pause");
        }
    } while (1);
}

int main() {
    loadData();
    menuUtama();
    return 0;
} 

//YASABANYAKBACOT
