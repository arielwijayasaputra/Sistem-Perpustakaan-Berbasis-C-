#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

// Struktur petugas
struct petugas
{
    int id_petugas;
    string usnm;
    string password;
    string confirm_password;
    string nama;
};

// Struktur anggota
struct anggota
{
    int id_anggota;
    string kode_anggota;
    string nama;
    string alamat;
    struct ttl
    {
        int hari;
        int bulan;
        int tahun;
    } ttl;
    string email;
    int status;
};

// Struktur peminjaman
struct peminjaman
{
    int id_peminjaman;
    int id_anggota;
    int id_buku;

    struct tgl_pjm
    {
        int hari;
        int bulan;
        int tahun;
    } tgl_pjm;

    struct tgl_kmb
    {
        int hari;
        int bulan;
        int tahun;
    };

    int status;
    int denda;
};

// Struktur buku
struct buku
{
    int id_buku;
    string isbn;
    string judul;
    string pengarang;
    string penerbit;
    int thn_terbit;
    int stok;
};

// Struktur perringkat buku yang paling banyak dipinjam
struct BukuPinjam
{
    string judul;
    string peminjam[100]; // Batas penampilan peminjam sampai 100 orang
    int count;
    int totalPinjam;
    int peminjam_per_orang[100]; // Batas buku dipinjam per orang sampai 100 orang
};

bool kabisat(int tahun)
{
    return (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
}

bool tanggal_benar(int hari, int bulan, int tahun)
{
    if (bulan < 1 || bulan > 12 || hari < 1)
        return false;

    int maxHari;
    switch (bulan)
    {
    // Bulan dengan 31 hari
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        maxHari = 31;
        break;

    // Bulan dengan 30 hari
    case 4:
    case 6:
    case 9:
    case 11:
        maxHari = 30;
        break;

    // Bulan dengan 28 atau 29 hari (kabisat)
    case 2:
        maxHari = kabisat(tahun) ? 29 : 28;
        break;
    default:
        return false;
    }
    return hari <= maxHari;
}

// Function buat ID petugas
int buatID(int nomor)
{
    return (100000 + nomor);
}

// Function buat kode anggota
string buatKodeanggota(int tahun, int bulan, int hari, int urutan)
{
    string id = to_string(tahun);
    if (bulan < 10)
        id += "0";
    id += to_string(bulan);
    if (hari < 10)
        id += "0";
    id += to_string(hari);

    if (urutan < 10)
        id += "00";
    else if (urutan < 100)
        id += "0";
    id += to_string(urutan);
    return id;
}

// Function konversi nama penerbit ke nomor
int konversinama(string nama)
{
    int kode = 0;
    for (char c : nama)
        kode += c;
    return 1000 + (kode % 9000);
}

// Function buat ISBN
string buatISBN(int urutan, string namaPenerbit)
{
    string prefix = "978";
    string negara = "602"; // Khusus Indonesia

    int nomorpenerbit = konversinama(namaPenerbit);
    string penerbit = to_string(nomorpenerbit);

    string judulCode = (urutan < 10) ? "0" + to_string(urutan) : to_string(urutan);

    string isbn12 = prefix + negara + penerbit + judulCode;

    // Hitung check digit
    int sum = 0;
    for (int i = 0; i < 12; i++)
    {
        int digit = isbn12[i] - '0';
        sum += (i % 2 == 0) ? digit : digit * 3;
    }
    int checkDigit = (10 - (sum % 10)) % 10;

    string isbn = prefix + "-" + negara + "-" + penerbit + "-" + judulCode + "-" + to_string(checkDigit);

    return isbn;
}

// Function fitur daftar petugas
void daftarPetugas()
{
    petugas p;
    int count = 0;
    string line;

    ifstream inFile("petugas.txt");
    while (getline(inFile, line))
    {
        if (line.find("ID:") != string::npos)
            count++;
    }
    inFile.close();

    p.id_petugas = buatID(count + 1);

    cout << "=== DAFTAR PETUGAS ===\n";
    cout << "Username: ";
    cin >> p.usnm;
    cout << "Password: ";
    cin >> p.password;
    cout << "Confirm Password: ";
    cin >> p.confirm_password;
    while (p.confirm_password != p.password)
    {
        cout << "Password tidak cocok! Silakan coba lagi.\n";
        cout << "Confirm Password: ";
        cin >> p.confirm_password;
    }
    ofstream outFile("petugas.txt", ios::app);
    outFile << "ID:" << p.id_petugas << endl;
    outFile << "Username:" << p.usnm << endl;
    outFile << "Password:" << p.password << endl;
    outFile << "----" << endl;
    outFile.close();

    cout << "\nPendaftaran berhasil! ID petugas: " << p.id_petugas << endl;
}

// Function fitur login petugas
bool loginPetugas()
{
    string username, password, line;
    bool ditemukan = false;

    cout << "=== LOGIN PETUGAS ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream inFile("petugas.txt");
    string id, uname, pass;

    while (getline(inFile, line))
    {
        if (line.find("ID:") == 0)
            id = line.substr(3);
        else if (line.find("Username:") == 0)
            uname = line.substr(9);
        else if (line.find("Password:") == 0)
            pass = line.substr(9);
        else if (line == "----")
        {
            if (uname == username && pass == password)
            {
                cout << "\nLogin berhasil! Selamat datang, " << uname << " (ID: " << id << ")\n";
                ditemukan = true;
                break;
            }
        }
    }
    inFile.close();

    if (!ditemukan)
        cout << "\nLogin gagal! Username atau password salah.\n";
    cout << "\n------------------------------------------\n";

    return ditemukan;
}

// Tampilan menu awal
int daftarlogin()
{
    petugas pg;
    int no;
    cout << "\n--------------------------------------\n";
    cout << "Silahkan masuk terlebih dahulu\n";
    cout << "1. Login petugas\n";
    cout << "2. Daftar petugas\n";
    cout << "Pilih menu : ";
    cin >> no;
    cout << "\n--------------------------------------\n";
    return no;
}

// Tampilan menu utama petugas
int menu_petugas()
{
    int pilih;
    cout << "1. Tambah Buku\n";
    cout << "2. Tambah Anggota\n";
    cout << "3. Tambah Peminjaman\n";
    cout << "4. Tampilkan data buku\n";
    cout << "5. Tampilkan data anggota\n";
    cout << "6. Tampilkan data buku peminjaman\n";
    cout << "7. Tampilakan data peminjaman buku yang belum di kembalikan\n";
    cout << "8. Tampilkan data petugas\n";
    cout << "9. Pengembalian buku\n";
    cout << "10. Cari buku\n";
    cout << "11. Cari anggota\n";
    cout << "12. Cari peminjam\n";
    cout << "13. Hapus buku\n";
    cout << "14. Tambah stok buku\n";
    cout << "15. Hapus peminjam\n";
    cout << "16. Logout\n";
    cout << "\n--------------------------------------\n";
    cout << "Pilih menu : ";
    cin >> pilih;
    cout << "\n--------------------------------------\n";
    return pilih;
}

// Function fitur tambah buku
void tambahBuku()
{
    buku b;
    int count = 0;
    string line;

    ifstream inFile("buku.txt");
    while (getline(inFile, line))
    {
        if (line.find("ID:") != string::npos)
            count++;
    }
    inFile.close();

    b.id_buku = 200000 + (count + 1);

    cout << "\n=== TAMBAH BUKU ===\n";
    cout << "Judul Buku     : ";
    cin.ignore();
    getline(cin, b.judul);
    cout << "Pengarang      : ";
    getline(cin, b.pengarang);
    cout << "Penerbit       : ";
    getline(cin, b.penerbit);
    cout << "ISBN           : ";
    cin >> b.isbn;
    cout << "Tahun Terbit   : ";
    cin >> b.thn_terbit;
    cout << "Stok Buku      : ";
    cin >> b.stok;

    ofstream outFile("buku.txt", ios::app);
    outFile << "ID:" << b.id_buku << endl;
    outFile << "ISBN:" << b.isbn << endl;
    outFile << "Judul:" << b.judul << endl;
    outFile << "Pengarang:" << b.pengarang << endl;
    outFile << "Penerbit:" << b.penerbit << endl;
    outFile << "Tahun:" << b.thn_terbit << endl;
    outFile << "Stok:" << b.stok << endl;
    outFile << "----" << endl;
    outFile.close();

    cout << "\nBuku berhasil ditambahkan dengan ID: " << b.id_buku << endl;
    cout << "------------------------------------------------------------------\n";
}

// Function fitur tambah anggota
void tambahAnggota()
{
    anggota a;

    int count = 0;
    string line;

    ifstream inFile("anggota.txt");
    while (getline(inFile, line))
    {
        if (line.find("Kode:") != string::npos)
            count++;
    }
    inFile.close();

    cout << "\n=== TAMBAH ANGGOTA ===\n";
    cout << "Nama Anggota   : ";
    cin.ignore();
    getline(cin, a.nama);
    cout << "Alamat         : ";
    getline(cin, a.alamat);
    do
    {
        cout << "Tanggal Lahir  : \n";
        cout << "Hari           : ";
        cin >> a.ttl.hari;
        cout << "Bulan          : ";
        cin >> a.ttl.bulan;
        cout << "Tahun          : ";
        cin >> a.ttl.tahun;
        if (!tanggal_benar(a.ttl.hari, a.ttl.bulan, a.ttl.tahun))
        {
            cout << "---------------------------\n";
            cout << "Tanggal lahir tidak valid!.\n";
            cout << "---------------------------\n";
        }

    } while (!tanggal_benar(a.ttl.hari, a.ttl.bulan, a.ttl.tahun));

    cout << "Email          : ";
    cin >> a.email;
    a.status = 1;

    a.id_anggota = 300000 + (count + 1);
    a.kode_anggota = buatKodeanggota(a.ttl.tahun, a.ttl.bulan, a.ttl.hari, count + 1);

    ofstream outFile("anggota.txt", ios::app);
    outFile << "Kode:" << a.kode_anggota << endl;
    outFile << "ID:" << a.id_anggota << endl;
    outFile << "Nama:" << a.nama << endl;
    outFile << "Alamat:" << a.alamat << endl;
    outFile << "Tanggal:" << a.ttl.hari << "-" << a.ttl.bulan << "-" << a.ttl.tahun << endl;
    outFile << "Email:" << a.email << endl;
    outFile << "Status:" << a.status << endl;
    outFile << "----" << endl;
    outFile.close();

    cout << "\nAnggota berhasil ditambahkan dengan Kode Anggota: " << a.kode_anggota << endl;
    cout << "------------------------------------------------------------------------------\n";
}

// Function fitur tambah peminjaman
void tambahpeminjaman()
{
    peminjaman p;
    string line;
    bool anggotaAda = false, bukuAda = false;
    int stok_buku = 0;
    string nama_anggota, judul_buku;

    cout << "\n=== TAMBAH PEMINJAMAN ===\n";
    cout << "Masukkan Nama Anggota : ";
    cin.ignore();
    getline(cin, nama_anggota);
    cout << "Masukkan Judul Buku   : ";
    getline(cin, judul_buku);

    // Cek nama anggota
    ifstream fileAnggota("anggota.txt");
    string id, nama;
    while (getline(fileAnggota, line))
    {
        if (line.find("Kode:") == 0)
            id = line.substr(5);
        else if (line.find("Nama:") == 0)
        {
            nama = line.substr(5);
            if (nama == nama_anggota)
            {
                cout << "Anggota ditemukan: " << nama << " (ID:" << id << ")" << endl;
                cout << "\n--------------------------------------------------\n";
                anggotaAda = true;
                break;
            }
        }
    }
    fileAnggota.close();

    if (!anggotaAda)
    {
        cout << "Anggota \"" << nama_anggota << "\" tidak ditemukan!\n";
        return;
    }

    // Cek buku berdasarkan judul
    ifstream fileBuku("buku.txt");
    string judulB, pengarang, penerbit;
    int stokTmp, tahun;
    while (getline(fileBuku, line))
    {
        if (line.find("Judul:") == 0)
            judulB = line.substr(6);
        else if (line.find("Pengarang:") == 0)
            pengarang = line.substr(10);
        else if (line.find("Penerbit:") == 0)
            penerbit = line.substr(9);
        else if (line.find("Tahun:") == 0)
            tahun = stoi(line.substr(6));
        else if (line.find("Stok:") == 0)
            stokTmp = stoi(line.substr(5));
        else if (line == "----")
        {
            if (judulB == judul_buku)
            {
                if (stokTmp > 0)
                {
                    bukuAda = true;
                    stok_buku = stokTmp;
                    break;
                }
                else
                {
                    cout << "Buku \"" << judul_buku << "\" sedang kehabisan stok.\n";
                    return;
                }
            }
        }
    }
    fileBuku.close();

    if (!bukuAda)
    {
        cout << "Buku \"" << judul_buku << "\" tidak ditemukan!\n";
        return;
    }

    // Ambil tanggal sekarang
    time_t now = time(0);
    tm *ltm = localtime(&now);
    p.tgl_pjm.hari = ltm->tm_mday;
    p.tgl_pjm.bulan = 1 + ltm->tm_mon;
    p.tgl_pjm.tahun = 1900 + ltm->tm_year;

    p.status = 1;

    // Simpan data peminjaman
    int urutanPeminjaman = 0;
    ifstream hitungFile("peminjaman.txt");
    while (getline(hitungFile, line))
    {
        if (line.find("ID_Peminjaman:") == 0)
            urutanPeminjaman++;
    }

    p.id_peminjaman = 300000 + urutanPeminjaman + 1;
    ofstream outFile("peminjaman.txt", ios::app);

    outFile << "ID_Peminjaman:" << p.id_peminjaman << endl;
    outFile << "Nama_Anggota:" << nama_anggota << endl;
    outFile << "Judul_Buku:" << judul_buku << endl;
    outFile << "Tgl_Pinjam:" << p.tgl_pjm.hari << "-" << p.tgl_pjm.bulan << "-" << p.tgl_pjm.tahun << endl;
    outFile << "Status:" << p.status << endl;
    outFile << "----" << endl;
    outFile.close();

    // Kurangi stok buku
    ifstream fileIn("buku.txt");
    ofstream fileTemp("temp.txt");
    string baris_setelah;
    string judulSekarang;
    while (getline(fileIn, baris_setelah))
    {
        if (baris_setelah.find("Judul:") == 0)
            judulSekarang = baris_setelah.substr(6);

        if (judulSekarang == judul_buku && baris_setelah.find("Stok:") == 0)
        {
            int stokBaru = stok_buku - 1;
            fileTemp << "Stok:" << stokBaru << endl;
        }
        else
        {
            fileTemp << baris_setelah << endl;
        }
    }
    fileIn.close();
    fileTemp.close();
    remove("buku.txt");
    rename("temp.txt", "buku.txt");

    cout << "\nPeminjaman berhasil dicatat!\n";
    cout << "Nama Anggota   : " << nama_anggota << endl;
    cout << "Judul Buku     : " << judul_buku << endl;
    cout << "Tanggal Pinjam : " << p.tgl_pjm.hari << "-" << p.tgl_pjm.bulan << "-" << p.tgl_pjm.tahun << endl;
    cout << "\n---------------------------------------------------\n";
}

// Function fitur tampil data buku
void tampil_data_buku()
{
    ifstream fileBuku("buku.txt");
    string line;
    while (getline(fileBuku, line))
    {
        cout << line << endl;
    }
    fileBuku.close();
}

// Function fitur tampil data anggota
void tampil_data_anggota()
{
    ifstream file("anggota.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file anggota.txt\n";
        return;
    }

    string line;
    string id, nama;
    int status;

    cout << "\n=== DAFTAR ANGGOTA ===\n";

    // Cari kata per baris
    while (getline(file, line))
    {
        if (line.find("Kode:") == 0)
        {
            id = line.substr(5);
        }
        else if (line.find("Nama:") == 0)
        {
            nama = line.substr(5);
        }
        else if (line.find("Status:") == 0)
        {
            status = stoi(line.substr(7));
        }
        else if (line == "----")
        {
            cout << "Kode Anggota : " << id << endl;
            cout << "Nama         : " << nama << endl;
            cout << "Status       : " << (status == 1 ? "Aktif" : "Nonaktif") << endl;
            cout << "-----------------------------\n";
        }
    }

    file.close();
}

// Function fitur tampilkan peringkat buku yang paling banyak dipinjam
void tampilkanPeringkatBuku()
{
    BukuPinjam data[100];
    int totalBuku = 0;

    ifstream inFile("peminjaman.txt");
    string line, judul, username;

    while (getline(inFile, line))
    {
        if (line.find("Judul_Buku:") == 0)
            judul = line.substr(11);
        else if (line.find("Nama_Anggota:") == 0)
            username = line.substr(13);
        else if (line == "----")
        {
            // Cek buku di array
            int i;
            for (i = 0; i < totalBuku; i++)
            {
                if (data[i].judul == judul)
                    break;
            }
            if (i == totalBuku)
            {
                // Buku baru
                data[i].judul = judul;
                data[i].peminjam[0] = username;
                data[i].count = 1;
                data[i].totalPinjam = 1;
                totalBuku++;
            }
            else
            {
                // Buku sudah ada
                bool sudahAda = false;
                for (int j = 0; j < data[i].count; j++)
                {
                    if (data[i].peminjam[j] == username)
                    {
                        sudahAda = true;
                        break;
                    }
                }
                if (!sudahAda)
                {
                    data[i].peminjam[data[i].count] = username;
                    data[i].count++;
                }
                data[i].totalPinjam++;
            }
        }
    }
    inFile.close();

    // Urutkan buku berdasarkan total pinjam
    for (int i = 0; i < totalBuku - 1; i++)
    {
        for (int j = 0; j < totalBuku - i - 1; j++)
        {
            if (data[j].totalPinjam < data[j + 1].totalPinjam)
            {
                BukuPinjam temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }

    // Tampilkan di terminal
    for (int i = 0; i < totalBuku; i++)
    {
        cout << "Judul Buku      : " << data[i].judul << endl;
        cout << "Banyak Dipinjam : " << data[i].totalPinjam << "x" << endl;
        cout << "Daftar Peminjam : " << endl;
        for (int j = 0; j < data[i].count; j++)
            cout << j + 1 << ". " << data[i].peminjam[j] << endl;
        cout << "------------------------\n";
    }
}

void peminjaman_blm_dikembalikan()
{
    BukuPinjam data[100];
    int totalBuku = 0;

    ifstream inFile("peminjaman.txt");
    string line, judul, username, status;

    while (getline(inFile, line))
    {
        if (line.find("Judul_Buku:") == 0)
            judul = line.substr(11);
        else if (line.find("Nama_Anggota:") == 0)
            username = line.substr(13);
        else if (line.find("Status:") == 0)
            status = line.substr(7);
        else if (line == "----")
        {
            // Cek buku di array

            int i;
            if (status != "1")
                continue; // Hanya proses jika status peminjaman adalah 1 (belum dikembalikan)

            for (i = 0; i < totalBuku; i++)
            {
                if (data[i].judul == judul)
                    break;
            }
            if (i == totalBuku)
            {
                // Buku baru
                data[i].judul = judul;
                data[i].peminjam[0] = username;
                data[i].peminjam_per_orang[0] = 1;
                data[i].count = 1;
                data[i].totalPinjam = 1;
                totalBuku++;
            }
            else
            {
                // Buku sudah ada
                bool sudahAda = false;
                for (int j = 0; j < data[i].count; j++)
                {
                    if (data[i].peminjam[j] == username)
                    {
                        data[i].peminjam_per_orang[j]++;
                        sudahAda = true;
                        break;
                    }
                }
                if (!sudahAda)
                {
                    data[i].peminjam[data[i].count] = username;
                    data[i].peminjam_per_orang[data[i].count] = 1;
                    data[i].count++;
                }
                data[i].totalPinjam++;
            }
        }
    }
    inFile.close();

    if (totalBuku == 0)
    {
        cout << "Tidak ada anggota yang sedang meminjam buku.\n";
        cout << "----------------------------------------------\n";
        return;
    }

    // Urutkan buku berdasarkan total pinjam
    for (int i = 0; i < totalBuku - 1; i++)
    {
        for (int j = 0; j < totalBuku - i - 1; j++)
        {
            if (data[j].totalPinjam < data[j + 1].totalPinjam)
            {
                BukuPinjam temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }

    // Tampilkan di terminal
    for (int i = 0; i < totalBuku; i++)
    {
        cout << "Judul Buku      : " << data[i].judul << endl;
        cout << "Banyak buku yang belum dikembalikan: " << data[i].totalPinjam << "x" << endl;
        cout << "Daftar Peminjam : " << endl;
        for (int j = 0; j < data[i].count; j++)
            cout << j + 1 << ". " << data[i].peminjam[j] << " (" << data[i].peminjam_per_orang[j] << "x)" << endl;
        cout << "------------------------\n";
    }
}

void tampilkan_data_petugas()
{
    // Menampilkan data petugas
    ifstream filePetugas("petugas.txt");
    string line;
    while (getline(filePetugas, line))
    {
        cout << line << endl;
    }
    filePetugas.close();
}

// Function fitur pengembalian buku
void pengembalian_buku()
{
    cout << "\n=== PENGEMBALIAN BUKU ===\n";
    string nama_anggota, judul_buku;
    bool ditemukan = false;

    cout << "Masukkan Nama Anggota : ";
    cin.ignore();
    getline(cin, nama_anggota);
    cout << "Masukkan Judul Buku   : ";
    getline(cin, judul_buku);

    ifstream inFile("peminjaman.txt");
    ofstream tempFile("temp.txt");
    string line;

    string id_pjm, nama, judul, tgl, status;
    int statusNum = 0;

    while (getline(inFile, line))
    {
        if (line.find("ID_Peminjaman:") == 0)
            id_pjm = line.substr(14);
        else if (line.find("Nama_Anggota:") == 0)
            nama = line.substr(13);
        else if (line.find("Judul_Buku:") == 0)
            judul = line.substr(11);
        else if (line.find("Tgl_Pinjam:") == 0)
            tgl = line.substr(11);
        else if (line.find("Status:") == 0)
            status = line.substr(7);
        else if (line == "----")
        {

            // Cek jika nama & judul cocok dan belum dikembalikan
            if (nama == nama_anggota && judul == judul_buku && status == "1")
            {
                ditemukan = true;
                statusNum = 0; // Ubah status jadi dikembalikan

                // Catat tanggal pengembalian
                time_t now = time(0);
                tm *ltm = localtime(&now);
                string tgl_kembali = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);

                int hari_injam, bulan_pinjam, tahun_injam;
                sscanf(tgl.c_str(), "%d-%d-%d", &hari_injam, &bulan_pinjam, &tahun_injam);

                // Tanggal pinjam
                tm tmPinjam = {};
                tmPinjam.tm_mday = hari_injam;
                tmPinjam.tm_mon = bulan_pinjam - 1;
                tmPinjam.tm_year = tahun_injam - 1900;
                time_t timePinjam = mktime(&tmPinjam);

                // Tanggal kembali
                tm tmKembali = {};
                tmKembali.tm_mday = ltm->tm_mday;
                tmKembali.tm_mon = ltm->tm_mon;
                tmKembali.tm_year = ltm->tm_year;
                time_t timeKembali = mktime(&tmKembali);

                // Hitung selisih hari
                double selisihDetik = difftime(timeKembali, timePinjam);
                int selisihHari = selisihDetik / (60 * 60 * 24);

                // Hitung denda
                int denda = 0;
                if (selisihHari > 7)
                {
                    denda = (selisihHari - 7) * 1000;
                }

                cout << "\nBuku \"" << judul << "\" telah dikembalikan oleh " << nama << endl;
                cout << "------------------------------------------------\n";

                if (denda > 0)
                {
                    cout << "7 hari terlambat mengembalikan buku jadi " << selisihHari - 7 << " hari menunda. Denda: Rp." << denda << endl;
                    cout << "------------------------------------------------\n";
                }
                else
                {
                    cout << "Tidak terlambat. Tidak ada denda." << endl;
                    cout << "------------------------------------------------\n";
                }

                tempFile << "ID_Peminjaman:" << id_pjm << endl;
                tempFile << "Nama_Anggota:" << nama << endl;
                tempFile << "Judul_Buku:" << judul << endl;
                tempFile << "Tgl_Pinjam:" << tgl << endl;
                tempFile << "Tgl_Kembali:" << tgl_kembali << endl;
                tempFile << "Status:" << statusNum << endl;
                tempFile << "----" << endl;

                // Tambah stok buku kembali
                ifstream bukuIn("buku.txt");
                ofstream bukuTemp("buku_temp.txt");
                string bLine, jdl;
                int stok = 0;
                bool foundBuku = false;

                while (getline(bukuIn, bLine))
                {
                    if (bLine.find("Judul:") == 0)
                        jdl = bLine.substr(6);

                    if (jdl == judul && bLine.find("Stok:") == 0)
                    {
                        stok = stoi(bLine.substr(5));
                        stok++;
                        bukuTemp << "Stok:" << stok << endl;
                        foundBuku = true;
                    }
                    else
                    {
                        bukuTemp << bLine << endl;
                    }
                }
                bukuIn.close();
                bukuTemp.close();

                remove("buku.txt");
                rename("buku_temp.txt", "buku.txt");
            }
            else
            {
                // Salin data lama jika tidak cocok
                tempFile << "ID_Peminjaman:" << id_pjm << endl;
                tempFile << "Nama_Anggota:" << nama << endl;
                tempFile << "Judul_Buku:" << judul << endl;
                tempFile << "Tgl_Pinjam:" << tgl << endl;
                if (status == "0")
                    tempFile << "Tgl_Kembali:" << tgl << endl;
                tempFile << "Status:" << status << endl;
                tempFile << "----" << endl;
            }
        }
    }
    inFile.close();
    tempFile.close();

    remove("peminjaman.txt");
    rename("temp.txt", "peminjaman.txt");

    if (!ditemukan)
    {
        cout << "\nData peminjaman tidak ditemukan atau buku sudah dikembalikan.\n";
        cout << "------------------------------------------------\n";
    }
    else
    {
        cout << "Pengembalian berhasil dicatat.\n";
        cout << "------------------------------------------------\n";
    }
}

// Function fitur cari buku
void cari_buku()
{
    cout << "\n=== CARI BUKU ===\n";
    string keyword;
    cout << "Masukkan kata kunci (judul / pengarang / ISBN): ";
    cin.ignore();
    getline(cin, keyword);

    ifstream file("buku.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file buku.txt\n";
        return;
    }

    string line;
    bool ditemukan = false;
    string id, isbn, judul, pengarang, penerbit;
    int tahun = 0, stok = 0;

    while (getline(file, line))
    {
        if (line.find("ID:") == 0)
            id = line.substr(3);
        else if (line.find("ISBN:") == 0)
            isbn = line.substr(5);
        else if (line.find("Judul:") == 0)
            judul = line.substr(6);
        else if (line.find("Pengarang:") == 0)
            pengarang = line.substr(10);
        else if (line.find("Penerbit:") == 0)
            penerbit = line.substr(9);
        else if (line.find("Tahun:") == 0)
            tahun = stoi(line.substr(6));
        else if (line.find("Stok:") == 0)
            stok = stoi(line.substr(5));
        else if (line == "----")
        {
            // Cek apakah keyword cocok (judul/pengarang/isbn)
            if (judul.find(keyword) != string::npos ||
                pengarang.find(keyword) != string::npos ||
                isbn.find(keyword) != string::npos)
            {

                cout << "\n=== Buku Ditemukan ===\n";
                cout << "ID Buku     : " << id << endl;
                cout << "ISBN        : " << isbn << endl;
                cout << "Judul       : " << judul << endl;
                cout << "Pengarang   : " << pengarang << endl;
                cout << "Penerbit    : " << penerbit << endl;
                cout << "Tahun Terbit: " << tahun << endl;
                cout << "Stok        : " << stok << endl;
                cout << "------------------------------\n";
                ditemukan = true;
            }
        }
    }

    file.close();

    if (!ditemukan)
        cout << "\nBuku dengan kata kunci \"" << keyword << "\" tidak ditemukan.\n";
}

// Function fitur cari anggota
void cari_anggota()
{
    cout << "\n=== CARI ANGGOTA ===\n";
    string keyword;
    cout << "Masukkan kata kunci (nama / kode anggota / email): ";
    cin.ignore();
    getline(cin, keyword);

    ifstream file("anggota.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file anggota.txt\n";
        return;
    }

    string line;
    bool ditemukan = false;
    string kode, id, nama, alamat, tanggal, email;
    int status = 0;

    while (getline(file, line))
    {
        if (line.find("Kode:") == 0)
            kode = line.substr(5);
        else if (line.find("ID:") == 0)
            id = line.substr(3);
        else if (line.find("Nama:") == 0)
            nama = line.substr(5);
        else if (line.find("Alamat:") == 0)
            alamat = line.substr(7);
        else if (line.find("Tanggal:") == 0)
            tanggal = line.substr(8);
        else if (line.find("Email:") == 0)
            email = line.substr(6);
        else if (line.find("Status:") == 0)
            status = stoi(line.substr(7));
        else if (line == "----")
        {
            // Cek apakah keyword cocok
            if (kode.find(keyword) != string::npos ||
                id.find(keyword) != string::npos ||
                nama.find(keyword) != string::npos ||
                email.find(keyword) != string::npos)
            {

                cout << "\n=== Anggota Ditemukan ===\n";
                cout << "Kode Anggota : " << kode << endl;
                cout << "ID Anggota   : " << id << endl;
                cout << "Nama         : " << nama << endl;
                cout << "Alamat       : " << alamat << endl;
                cout << "Tanggal Lahir: " << tanggal << endl;
                cout << "Email        : " << email << endl;
                cout << "Status       : " << (status == 1 ? "Aktif" : "Nonaktif") << endl;
                cout << "------------------------------\n";
                ditemukan = true;
            }
        }
    }

    file.close();

    if (!ditemukan)
        cout << "\nAnggota dengan kata kunci \"" << keyword << "\" tidak ditemukan.\n";
}

// Function fitur cari peminjam
void cari_peminjam()
{
    cout << "\n=== CARI PEMINJAM ===\n";
    string keyword;
    cout << "Masukkan kata kunci (nama anggota / judul buku): ";
    cin.ignore();
    getline(cin, keyword);

    ifstream file("peminjaman.txt");
    if (!file.is_open())
    {
        cout << "Gagal membuka file peminjaman.txt\n";
        return;
    }

    string line;
    bool ditemukan = false;
    string id_pjm, nama, judul, tgl_pinjam, tgl_kembali, status;

    while (getline(file, line))
    {
        if (line.find("ID_Peminjaman:") == 0)
            id_pjm = line.substr(14);
        else if (line.find("Nama_Anggota:") == 0)
            nama = line.substr(13);
        else if (line.find("Judul_Buku:") == 0)
            judul = line.substr(11);
        else if (line.find("Tgl_Pinjam:") == 0)
            tgl_pinjam = line.substr(11);
        else if (line.find("Tgl_Kembali:") == 0)
            tgl_kembali = line.substr(12);
        else if (line.find("Status:") == 0)
            status = line.substr(7);
        else if (line == "----")
        {
            // Cek apakah keyword cocok
            if (nama.find(keyword) != string::npos ||
                judul.find(keyword) != string::npos)
            {

                cout << "\n=== Data Peminjaman Ditemukan ===\n";
                cout << "ID Peminjaman : " << id_pjm << endl;
                cout << "Nama Anggota  : " << nama << endl;
                cout << "Judul Buku    : " << judul << endl;
                cout << "Tanggal Pinjam: " << tgl_pinjam << endl;
                if (!tgl_kembali.empty())
                    cout << "Tgl Kembali   : " << tgl_kembali << endl;
                cout << "Status        : " << (status == "1" ? "Belum Dikembalikan" : "Sudah Dikembalikan") << endl;
                cout << "------------------------------\n";
                ditemukan = true;
            }
        }
    }

    file.close();

    if (!ditemukan)
        cout << "\nPeminjaman dengan kata kunci \"" << keyword << "\" tidak ditemukan.\n";
}

// Function fitur hapus buku
void hapus_buku()
{
    cout << "\n=== HAPUS DATA BUKU ===\n";
    string idCari;
    cout << "Masukkan ID Buku yang ingin dihapus: ";
    cin >> idCari;

    ifstream file("buku.txt");
    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open())
    {
        cout << "Gagal membuka file.\n";
        return;
    }

    string line;
    bool ditemukan = false;
    bool skip = false;
    string idSekarang = "";

    while (getline(file, line))
    {
        if (line.find("ID:") == 0)
        {
            idSekarang = line.substr(3);
            if (idSekarang == idCari)
            {
                skip = true;
                ditemukan = true;
                continue;
            }
        }

        if (!skip)
            temp << line << "\n";

        if (line == "----")
            skip = false;
    }

    file.close();
    temp.close();

    remove("buku.txt");
    rename("temp.txt", "buku.txt");

    if (ditemukan)
        cout << "Data buku dengan ID " << idCari << " berhasil dihapus.\n";
    else
        cout << "Buku dengan ID " << idCari << " tidak ditemukan.\n";
}

// Function fitur hapus anggota
void hapus_anggota()
{
    cout << "\n=== HAPUS DATA ANGGOTA ===\n";
    string kodeCari;
    cout << "Masukkan Kode Anggota yang ingin dihapus: ";
    cin >> kodeCari;

    ifstream file("anggota.txt");
    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open())
    {
        cout << "Gagal membuka file.\n";
        return;
    }

    string line;
    bool ditemukan = false;
    bool skip = false;
    string kodeSekarang = "";

    while (getline(file, line))
    {
        if (line.find("Kode:") == 0)
        {
            kodeSekarang = line.substr(5);
            if (kodeSekarang == kodeCari)
            {
                skip = true;
                ditemukan = true;
                continue;
            }
        }

        if (!skip)
            temp << line << "\n";

        if (line == "----")
            skip = false;
    }

    file.close();
    temp.close();

    remove("anggota.txt");
    rename("temp.txt", "anggota.txt");

    if (ditemukan)
        cout << "Data anggota dengan kode " << kodeCari << " berhasil dihapus.\n";
    else
        cout << "Anggota dengan kode " << kodeCari << " tidak ditemukan.\n";
}

// Function fitur tambah stok buku
void Tambah_stok_buku()
{
    cout << "\n=== TAMBAH STOK BUKU ===\n";
    string idCari;
    int tambahanStok;
    cout << "Masukkan ID Buku yang ingin ditambah stoknya: ";
    cin >> idCari;
    cout << "Masukkan jumlah stok yang ingin ditambahkan: ";
    cin >> tambahanStok;

    ifstream file("buku.txt");
    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open())
    {
        cout << "Gagal membuka file.\n";
        return;
    }

    string line;
    bool ditemukan = false;
    string idSekarang = "";
    int stokSekarang = 0;

    while (getline(file, line))
    {
        if (line.find("ID:") == 0)
        {
            idSekarang = line.substr(3);
        }

        if (idSekarang == idCari && line.find("Stok:") == 0)
        {
            stokSekarang = stoi(line.substr(5));
            stokSekarang += tambahanStok;
            temp << "Stok:" << stokSekarang << "\n";
            ditemukan = true;
        }
        else
        {
            temp << line << "\n";
        }
    }

    file.close();
    temp.close();

    remove("buku.txt");
    rename("temp.txt", "buku.txt");

    if (ditemukan)
        cout << "Stok buku dengan ID " << idCari << " berhasil ditambahkan. Stok baru: " << stokSekarang << "\n";
    else
        cout << "Buku dengan ID " << idCari << " tidak ditemukan.\n";
}

// Function fitur hapus peminjaman
void hapus_peminjaman()
{
    cout << "\n=== HAPUS DATA PEMINJAMAN ===\n";
    string idCari;
    cout << "Masukkan ID Peminjaman yang ingin dihapus: ";
    cin >> idCari;

    ifstream file("peminjaman.txt");
    ofstream temp("temp.txt");

    if (!file.is_open() || !temp.is_open())
    {
        cout << "Gagal membuka file.\n";
        return;
    }

    string line;
    bool ditemukan = false;
    bool skip = false;
    string idSekarang = "";

    while (getline(file, line))
    {
        if (line.find("ID_Peminjaman:") == 0)
        {
            idSekarang = line.substr(14);
            if (idSekarang == idCari)
            {
                skip = true;
                ditemukan = true;
                continue;
            }
        }

        if (!skip)
            temp << line << "\n";

        if (line == "----")
            skip = false;
    }

    file.close();
    temp.close();

    remove("peminjaman.txt");
    rename("temp.txt", "peminjaman.txt");

    if (ditemukan)
        cout << "Data peminjaman dengan ID " << idCari << " berhasil dihapus.\n";
    else
        cout << "Data peminjaman dengan ID " << idCari << " tidak ditemukan.\n";
}

int main()
{
    bool login = true;

    while (login)
    {
        int nomor = daftarlogin();

        // Menu switch case login dan daftar petugas
        switch (nomor)
        {
        case 1:
        {
            bool sukses = loginPetugas();

            if (sukses)
            {
                bool menuJalan = true;
                while (menuJalan)
                {
                    int pilih = menu_petugas();

                    // Menu switch case fitur petugas jika sudah login
                    switch (pilih)
                    {
                    case 1:
                        tambahBuku();
                        break;
                    case 2:
                        tambahAnggota();
                        break;
                    case 3:
                        tambahpeminjaman();
                        break;
                    case 4:
                        tampil_data_buku();
                        break;
                    case 5:
                        tampil_data_anggota();
                        break;
                    case 6:
                        tampilkanPeringkatBuku();
                        break;
                    case 7:
                        peminjaman_blm_dikembalikan();
                        break;
                    case 8:
                        tampilkan_data_petugas();
                        break;
                    case 9:
                        pengembalian_buku();
                        break;
                    case 10:
                        cari_buku();
                        break;
                    case 11:
                        cari_anggota();
                        break;
                    case 12:
                        cari_peminjam();
                        break;
                    case 13:
                        hapus_buku();
                        break;
                    case 14:
                        Tambah_stok_buku();
                        break;
                    case 15:
                        hapus_peminjaman();
                        break;
                    case 16:
                        cout << "Logout berhasil.\n";
                        menuJalan = false;
                        break;
                    default:
                        cout << "Fungsi lain belum diimplementasikan.\n";
                        break;
                    }
                }
            }

            break;
        }

        case 2:
            daftarPetugas();
            break;
        default:
            cout << "Pilih lah yang benar.\n";
            break;
        }
    }
}