#ifndef DISTRIBUTED_LOGGER_H_
#define DISTRIBUTED_LOGGER_H_

#include <string>
#include <log4cpp/Category.hh>

enum Priority
{
    ERROR,
    WARN,
    INFO,
    DEBUG
};
class Logger
{
public:
    bool init(const std::string &log_conf_file);

    static Logger *instance()
    {
        return &instance_;
    }
    log4cpp::Category *get_handle()
    {
        return category_;
    }
    void setPriority(Priority priority);

private:
    Logger();
    ~Logger();

protected:
    static Logger instance_;
    log4cpp::Category *category_;
};

#define LOG_INFO Logger::instance()->get_handle()->info
#define LOG_DEBUG Logger::instance()->get_handle()->debug
#define LOG_ERROR Logger::instance()->get_handle()->error
#define LOG_WARN Logger::instance()->get_handle()->warn
#endif
