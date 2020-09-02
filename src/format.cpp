#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string FormatTime(string time){
    /*
    private String twoDigitString(int number) {

    if (number == 0) {
        return "00";
    }

    if (number / 10 == 0) {
        return "0" + number;
    }

    return String.valueOf(number);
}
*/
    if (std::stoi(time) < 10) {
        return "0" + time;
    }
    return time;
}
string Format::ElapsedTime(long seconds) {
    /* 
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    */

    std::string hours = FormatTime(std::to_string(seconds / 3600));
    std::string minutes = FormatTime(std::to_string((seconds % 3600) / 60));
    std::string second = FormatTime(std::to_string(seconds % 60));

    return string(hours + ":" + minutes + ":" + second); 
    }