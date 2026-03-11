#include <iostream>
#include <string>

using namespace std;

struct Node {
    string songTitle;
    Node* next;
};

Node* head = nullptr;

void addSong(string title) {
    Node* newNode = new Node();
    newNode->songTitle = title;
    newNode->next = head;
    head = newNode;
}

void displayPlaylist() {
    if (head == nullptr) {
        return;
    }
    Node* temp = head;
    int count = 1;
    while (temp != nullptr) {
        cout << count << ". " << temp->songTitle << endl;
        temp = temp->next;
        count++;
    }
}

void searchSong(string title) {
    Node* temp = head;
    bool found = false;
    while (temp != nullptr) {
        if (temp->songTitle == title) {
            found = true;
            break;
        }
        temp = temp->next;
    }
    if (found) cout << "Found: " << title << endl;
    else cout << "Not Found: " << title << endl;
}

int main() {
    addSong("The Weeknd - Blinding Lights");
    addSong("Queen - Bohemian Rhapsody");
    addSong("Arctic Monkeys - Do I Wanna Know?");

    int choice;
    string title;

    while (true) {
        cout << "\n1. Add\n2. Search\n3. List\n4. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            getline(cin, title);
            addSong(title);
        } else if (choice == 2) {
            getline(cin, title);
            searchSong(title);
        } else if (choice == 3) {
            displayPlaylist();
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}