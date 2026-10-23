#pragma once
#include <string>

// Stub buat #51 - nanti #52 baru link ke box64+Wine beneran
// Sekarang cuma simulasi launch biar build tetep HIJAU

inline bool WineNX_Launch(const std::string& exePath){
    // TODO #52: system("box64 wine sdmc:/switch/winlator-nx/games/xxx.exe")
    // Untuk #51 kita cuma return true biar UI bisa popup "Launching..."
    return true;
}

inline bool WineNX_IsWineInstalled(){
    // cek sdmc:/switch/winlator-nx/wine/bin/wine
    return false; // #51 belum install wine, #52 baru ada
}
