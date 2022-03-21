#include "utils.h"

std::string get_directory() {
    std::string result = "";

    const std::filesystem::path src{"./src/"};
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{src})
        if (dir_entry.is_directory()) {
            const char* path = (const char*)dir_entry.path().c_str();
            result += std::string(path) + " ";
        }

    return result;
}

void execute(std::string cmd) {
    system(cmd.c_str());
}

void make(std::string configure, std::string subdirectories, std::string flags="") {
    execute("make build configure=" + configure + " SUB_DIR=\"" + subdirectories + "\" " + (flags.empty() ? "" : ("compile_flags=" + flags)));
}

void execute_new(std::string cmd) {
    execute("mintty " + cmd);
}

int main() {
    std::cout << "GoodMake Start" << std::endl;
    
    std::cout << "The following subdirectories were found in src:" << std::endl;
    const std::string subdirectories = get_directory();
    std::cout << (subdirectories.empty() ? "None" : subdirectories) << std::endl;

    make("Debug", subdirectories, "-g"); 
    
    execute_new("./bin/Test/main");
    Sleep(1000);
    execute("ps aux | grep /cygdrive/c/Users/acer/Desktop/Cpp/GoodMake/bin/Test/main | grep -v grep > pid");

    std::ifstream file("./pid");
    
    std::string pid = "";
    std::getline(file, pid);
    pid = pid.substr(5, 4);

    std::cout << "PID:" << pid << "\n";

    file.close();   

    execute_new("gdb \"./bin/Test/main\" " + pid);

    std::cout << "Please enter any key for exit\n";
    system("read -N 1");

    return EXIT_SUCCESS;
}