#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <chrono>
#include <iomanip>
namespace cron = std::chrono;
using namespace std::literals::chrono_literals;
using namespace std;

char charInputTimeout(cron::milliseconds, const std::string&,
    cron::milliseconds = 1s, int = 0);

int main() {
    while (true) {
        char in = charInputTimeout(30s, "Quick, enter a letter: ", 3000ms, 1);
        if (!in) {
            cout << "You're too slow.\n";
        }
        else {
            cout << "Your response is " << in << '\n';
            if (in == 'q' || in == 'Q') break;
        }
    }
}

char charInputTimeout(cron::milliseconds duration, const std::string& label,
    cron::milliseconds displayInterval, int timerPrecision) {
    char ret = 0;
    auto now = cron::steady_clock::now();
    auto timerEnd = now + duration;
    cron::duration<double> timeLeft = timerEnd - now;
    auto lastIntervalDisplayTime = now;
    bool redraw = true;

    while (timeLeft > 0s) {
        // Check if we need to redraw time because of key hit
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\b' || ch == 0x7f) {
                ret = 0;
                std::cout << "\b \b"; // quick and dirty backspace
                redraw = true;
            }
            else if (ch == '\r' || ch == '\n') {
                if (ret) break;
                // else do nothing
            }
            else {
                ret = ch;
                redraw = true;
            }
        }
        // Check if we need to redraw time interval
        now = cron::steady_clock::now();
        timeLeft = timerEnd - now;
        if (timeLeft < 0s) timeLeft = 0s;
        if (now - lastIntervalDisplayTime >= displayInterval) {
            redraw = true;
            lastIntervalDisplayTime = now;
        }
        // Draw
        if (redraw) {
            std::cout << "\rYou have " << std::fixed
                << std::setprecision(timerPrecision) << timeLeft.count()
                << "s left. " << label;
            if (ret) std::cout << ret;
            redraw = false;
        }
        Sleep(16); // don't rape your CPU!
    }
    std::cout << '\n';
    return ret;
}