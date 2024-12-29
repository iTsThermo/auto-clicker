#include <iostream>
#include <windows.h>
#include <random>

int sleep_delay(int targetCPS) {
    // Validate input and ensure CPS is reasonable
    if (targetCPS <= 0) {
        std::cout << "Invalid CPS value: " << targetCPS << ". Using default of 10 CPS." << std::endl;
        targetCPS = 10;
    } else if (targetCPS > 30) {
        std::cout << "CPS capped at 30 for safety." << std::endl;
        targetCPS = 30;
    }

    const int delay = 875 / targetCPS;

    return delay;
}

int main() {
    std::cout << "Press and hold the left mouse button to activate the autoclicker." << std::endl;
    std::cout << "Release the left mouse button to deactivate it." << std::endl;
    std::cout << "Press ESC to exit the program." << std::endl;

    int cps_input = 0;

    std::cout << "Enter delay between clicks(Lower Delay=Higher CPS):";

    std::cin >> cps_input;

    int delay = sleep_delay(cps_input);

    bool wasPressed = false;
    int clickCount = 0;

    while (true) {
        // Check if the ESC key is pressed to exit the program
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // Get the current physical state of the left mouse button
        const bool isPressed = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;

        // Button just pressed
        if (isPressed && !wasPressed) {
            std::cout << "Autoclicker Activated" << std::endl;
            clickCount = 0;
        }
        // Button is being held down
        else if (isPressed) {
            // Simulate right click instead of left click to avoid interference
            INPUT inputs[2] = {};
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

            SendInput(2, inputs, sizeof(INPUT));
            clickCount++;

            if (clickCount % 100 == 0) {
                std::cout << "Clicks: " << clickCount << std::endl;
            }
        }
        // Button just released
        else if (wasPressed) {
            std::cout << "Autoclicker Deactivated - Total clicks: " << clickCount << std::endl;
        }

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator

        int delay_min_threshold = delay * .95;
        int delay_max_threshold = delay * 1.05;

        std::uniform_int_distribution<> distr(delay_min_threshold, delay_max_threshold); // define the range

        const int random_click_delay = distr(gen);

        std::cout << "Random Sleep Timer: " << random_click_delay << std::endl;

        wasPressed = isPressed;
        Sleep(random_click_delay); // Adjust this delay to control click speed
    }
    return 0;
}