#include "memory.h"
#include <iostream>
#include "Windows.h"
#include <thread>

#define SPACE_BAR 0x20

const uint32_t STANDING = 65665;
const uint32_t CROUCHING = 65667;

uint32_t PLUS_JUMP = 65537;
uint32_t MINUS_JUMP = 256;

int dwLocalPlayerPawn = 0x1868CC8;
int dwForceJump = 0x18619D0;
int m_fFlag = 0x3EC;

bool bhopEnabled = false;
uint32_t plusJumpDelay = 10;
uint32_t minusJumpDelay = 10;

void Bhop() {
    Memory mem("cs2.exe");

    std::uintptr_t client = mem.GetModuleAddress("client.dll");
    std::uintptr_t ForceJump = client + dwForceJump;

    while (true)
    {
        if (bhopEnabled) {
            std::uintptr_t LocalPlayer = mem.Read<std::uintptr_t>(client + dwLocalPlayerPawn);
            uintptr_t fFlag = mem.Read<std::uint32_t>(LocalPlayer + m_fFlag);

            if (GetAsyncKeyState(SPACE_BAR) & 0x8000)
            {
                if (fFlag == STANDING || fFlag == CROUCHING)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(plusJumpDelay));
                    mem.Write<uint32_t>(ForceJump, PLUS_JUMP);
                }
                else
                {
                    mem.Write<uint32_t>(ForceJump, MINUS_JUMP);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(minusJumpDelay));
    }
}

void DisplayMenu() {
    std::cout << "\033[2J\033[H";
    std::cout << "Made by FrolixCZE https://www.unknowncheats.me/forum/members/5952497.html\n\n";
    std::cout << "[1] Bunnyhop [" << (bhopEnabled ? "ON" : "OFF") << "]\n";
    std::cout << "[2] Options\n";
    std::cout << "Select an option: ";
}

void OptionsMenu() {
    while (true) {
        std::cout << "\n[OPTIONS]\n";
        std::cout << "1. Set PLUS_JUMP delay (current: " << plusJumpDelay << " ms)\n";
        std::cout << "2. Set MINUS_JUMP delay (current: " << minusJumpDelay << " ms)\n";
        std::cout << "3. Reset to default (Legit settings)\n";
        std::cout << "4. Back\n";
        std::cout << "Select an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter new PLUS_JUMP delay (ms): ";
            std::cin >> plusJumpDelay;
            break;
        case 2:
            std::cout << "Enter new MINUS_JUMP delay (ms): ";
            std::cin >> minusJumpDelay;
            break;
        case 3:
            plusJumpDelay = 10;
            minusJumpDelay = 10;
            std::cout << "Delays reset to default.\n";
            break;
        case 4:
            return;
        default:
            std::cout << "Invalid option. Try again.\n";
        }
    }
}

int main() {
    std::thread bhopThread(Bhop);
    bhopThread.detach();

    while (true) {
        DisplayMenu();

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            bhopEnabled = !bhopEnabled;
            DisplayMenu();
            std::cout << "Bunnyhop " << (bhopEnabled ? "enabled" : "disabled") << ".\n";
            break;
        case 2:
            OptionsMenu();
            break;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}
