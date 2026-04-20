#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <sstream>
#include <thread>

enum LogLevel{
       DEBUG = 0,
       INFO,
       WARNING,
       ERROR
};

class Logger
{
public:
       static Logger* GetInstance()
       {
              static Logger Instance;
              return &Instance;
       }

       void Log(LogLevel level, const std::string& file, int line, const std::string& message) 
       {
        std::lock_guard<std::mutex> lock(mtx);
        
        std::cout << "[" << GetCurrentTime() << "] "
                  << "[" << LevelToString(level) << "] "
                  << "[" << std::this_thread::get_id() << "] "
                  << "[" << file << ":" << line << "] "
                  << message << std::endl;
    }

protected:
       std::string LevelToString(const LogLevel& level)
       {
              switch(level)
              {
                     case LogLevel::DEBUG: return "DEBUG" ;
                     case LogLevel::INFO: return "INFO" ;
                     case LogLevel::WARNING: return "WARNING" ;
                     case LogLevel::ERROR: return "ERROR" ;
                     default :  return "UNKNOWN";
              }
       }
       std::string GetCurrentTime()
       {
              auto now = std::chrono::system_clock::now();
              auto in_time_t = std::chrono::system_clock::to_time_t(now);
              std::stringstream ss;
              ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
              return ss.str();
       }
private:
       Logger() = default;
       ~Logger(){};

private:
       std::mutex mtx;
};

class LogMessage {
public:
    LogMessage(LogLevel level, const char* file, int line) 
        : Level(level), File(file), Line(line) {}

    ~LogMessage() {
        Logger::GetInstance()->Log(Level, File, Line, Oss.str());
    }

    template <typename T>
    LogMessage& operator<<(const T& msg) {
        Oss << msg;
        return *this;
    }

private:
    std::ostringstream Oss;
    LogLevel Level;
    std::string File;
    int Line;
};

#define LOG_DEBUG LogMessage(LogLevel::DEBUG, __FILE__, __LINE__)
#define LOG_INFO  LogMessage(LogLevel::INFO, __FILE__, __LINE__)
#define LOG_WARN  LogMessage(LogLevel::WARN, __FILE__, __LINE__)
#define LOG_ERROR LogMessage(LogLevel::ERROR, __FILE__, __LINE__)
