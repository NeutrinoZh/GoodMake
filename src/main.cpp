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
    std::cout << "EXECUTE '" << cmd << "'\n";
    system(cmd.c_str());
}

void make(std::string configure, std::string subdirectories, std::string flags="") {
    std::cout << "make build configure=" + configure + " SUB_DIR=\"" + subdirectories + "\" " + (flags.empty() ? "" : ("compile_flags=" + flags)) << std::endl; 
    execute("make build configure=" + configure + " SUB_DIR=\"" + subdirectories + "\" " + (flags.empty() ? "" : ("compile_flags=" + flags)));
}

void execute_new(std::string cmd) {
    execute("mintty& " + cmd);
}

int main(int countArg, char* args[]) {
    std::cout << "Please enter any key for exit\n";
    system("read -N 1");

    std::cout << "GoodMake Start" << std::endl;

    std::cout << "Reading config file: " << (countArg >= 2 ? args[1] : "./config") << std::endl;

    std::ifstream config(countArg >= 2 ? args[1] : "./config");
    
    std::string title = ""; std::getline(config, title);
    std::string flags = ""; std::getline(config, flags);
    std::string gdb = ""; std::getline(config, gdb);

    title.pop_back();
    flags.pop_back();

    config.close();

    std::cout << "The following subdirectories were found in src:" << std::endl;
    const std::string subdirectories = get_directory();
    std::cout << (subdirectories.empty() ? "None" : subdirectories) << std::endl;

    make("Debug", subdirectories, flags); 

    std::string cmd = ("./bin/" + title + "/main");
    execute_new(cmd);
    Sleep(1000);
    execute("ps aux | grep ./bin/" + title + "/main | grep -v grep > pid");

    std::ifstream file("./pid");
    
    std::string pid = "";   
    std::getline(file, pid);
    pid = pid.substr(5, 4);

    std::cout << "PID:" << pid << "\n";

    file.close();   

    std::cout << "GDB:" << (gdb == "True") << "\n";
    if (gdb == "True")
        execute_new("gdb \"./bin/" + title + "/main\" " + pid);

    std::cout << "Please enter any key for exit\n";
    system("read -N 1");

    return EXIT_SUCCESS;
}