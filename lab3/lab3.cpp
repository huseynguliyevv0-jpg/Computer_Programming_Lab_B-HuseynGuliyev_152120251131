#include <iostream>
#include <string>
#include <algorithm> // transform() için
#include <iomanip>   // setfill ve setw için

using namespace std;

// 1. KURAL: İstenen Struct Yapısı
struct Page {
    string url;
    string title;
    int accessTime[3]; // {Saat, Dakika, Saniye}
    Page *next;
    Page *prev;
};

class BrowserHistory {
private:
    Page* head;    // Listenin başı
    Page* current; // Şu an bulunduğumuz sayfa

public:
    BrowserHistory() : head(nullptr), current(nullptr) {}

    // VISIT: Yeni sayfa ekle (Append)
    void visit(const string& url, const string& title, int h, int m, int s) {
        Page* newPage = new Page();
        newPage->url = url;
        newPage->title = title;
        newPage->accessTime[0] = h;
        newPage->accessTime[1] = m;
        newPage->accessTime[2] = s;
        newPage->next = nullptr;
        newPage->prev = nullptr;

        if (head == nullptr) {
            head = newPage;
            current = newPage;
        } else {
            // Eğer "Forward" (İleri) geçmişi varsa ve yeni sayfa açılırsa, ileridekiler silinir
            Page* temp = current->next;
            while (temp != nullptr) {
                Page* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            
            current->next = newPage;
            newPage->prev = current;
            current = newPage;
        }
    }

    // BACK: Bir önceki sayfaya git
    bool back() {
        if (current != nullptr && current->prev != nullptr) {
            current = current->prev;
            return true;
        }
        return false;
    }

    // FORWARD: Bir sonraki sayfaya git
    bool forward() {
        if (current != nullptr && current->next != nullptr) {
            current = current->next;
            return true;
        }
        return false;
    }

    // DELETE ENTRY: Mevcut sayfayı sil ve bağlantıları kur
    bool deleteEntry() {
        if (current == nullptr) return false;

        Page* toDelete = current;

        // Bağlantıları güncelle
        if (toDelete->prev != nullptr) {
            toDelete->prev->next = toDelete->next;
        } else {
            head = toDelete->next; // İlk eleman siliniyorsa head değişir
        }

        if (toDelete->next != nullptr) {
            toDelete->next->prev = toDelete->prev;
        }

        // Yeni "current" belirle
        if (toDelete->prev != nullptr) {
            current = toDelete->prev;
        } else {
            current = toDelete->next;
        }

        delete toDelete;
        return true;
    }

    // DISPLAY HISTORY: Tüm geçmişi yazdır
    bool displayHistory() {
        if (head == nullptr) return false;

        cout << "\n---------- TARAYICI GECMISI ----------" << endl;
        for (Page* temp = head; temp != nullptr; temp = temp->next) {
            // Mevcut sayfayı vurgula
            if (temp == current) {
                cout << " [CURRENT] -> ";
            } else {
                cout << "              ";
            }

            cout << temp->title << " (" << temp->url << ") | Zaman: ";

            // KURAL: accessTime dizisini for döngüsü ile formatlı yazdır
            for (int i = 0; i < 3; i++) {
                cout << setfill('0') << setw(2) << temp->accessTime[i];
                if (i < 2) cout << ":";
            }
            cout << endl;
        }
        cout << "--------------------------------------\n" << endl;
        return true;
    }

    // MEMORY MANAGEMENT: Programdan çıkarken tüm düğümleri sil
    void clearMemory() {
        Page* temp = head;
        while (temp != nullptr) {
            Page* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        head = nullptr;
        current = nullptr;
        cout << "[SISTEM] Bellek tamamen temizlendi." << endl;
    }
};

int main() {
    string command;
    BrowserHistory browser;

    cout << "--- Tarayici Gecmis Yoneticisi Baslatildi ---" << endl;
    cout << "Komutlar: VISIT, BACK, FORWARD, DELETE, DISPLAY, EXIT" << endl;

    while (true) {
        cout << "\nKomut Girin: ";
        cin >> command;

        // Komutu büyük harfe çevir (Küçük harf girilse de çalışır)
        transform(command.begin(), command.end(), command.begin(), ::toupper);

        if (command == "VISIT") {
            string url, title;
            int h, m, s;
            cout << "URL: "; cin >> url;
            cout << "Baslik: "; cin >> title;
            cout << "Zaman (Saat Dakika Saniye - Bosluklu): "; cin >> h >> m >> s;

            if (h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59) {
                cout << "[HATA] Gecersiz zaman formati!" << endl;
            } else {
                browser.visit(url, title, h, m, s);
                cout << "[OK] Sayfa eklendi." << endl;
            }
        } 
        else if (command == "BACK") {
            if (browser.back()) cout << "[OK] Geri gidildi." << endl;
            else cout << "[HATA] Geriye gidecek sayfa yok!" << endl;
        } 
        else if (command == "FORWARD") {
            if (browser.forward()) cout << "[OK] Ileri gidildi." << endl;
            else cout << "[HATA] Ileride sayfa yok!" << endl;
        } 
        else if (command == "DELETE") {
            if (browser.deleteEntry()) cout << "[OK] Mevcut sayfa gecmisten silindi." << endl;
            else cout << "[HATA] Gecmis zaten bos!" << endl;
        } 
        else if (command == "DISPLAY") {
            if (!browser.displayHistory()) cout << "[BILGI] Gecmis su an bos." << endl;
        } 
        else if (command == "EXIT") {
            browser.clearMemory();
            cout << "[SISTEM] Program kapatiliyor..." << endl;
            break;
        } 
        else {
            cout << "[HATA] Gecersiz komut! Tekrar deneyin." << endl;
        }
    }

    return 0;
}