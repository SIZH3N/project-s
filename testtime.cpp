#include <iostream>
#include <chrono>
#include <conio.h>

int main()
{
    // ثبت زمان شروع برنامه
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Program started. Press 'a' to see elapsed time." << std::endl;
    char x;
    char y;
    while (true)
    {
        //     // بررسی اینکه آیا دکمه 'a' زده شده است

        if (y = kbhit())
        {
            (x = getch());
            std::cout << x << std ::endl;
            // ثبت زمان فعلی
            auto now = std::chrono::high_resolution_clock::now();
            // محاسبه زمان گذشته از شروع
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            std::cout << "Elapsed time: " << elapsed << " seconds" << "    " << int(y) << "   " << y << std::endl;
        }
        else
        {
            // std::cout << x << std::endl;
        }
        // std::cout << "dd" << std::endl;
    } //

    return 0;
}
