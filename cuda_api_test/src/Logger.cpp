#include "Logger.h"

#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>

Logger Logger::instance_;

bool Logger::init(const std::string& log_conf_file){
    try{
        log4cpp::PropertyConfigurator::configure(log_conf_file);

    }catch(log4cpp::ConfigureFailure& f){
        std::cerr << " load log config file " << log_conf_file.c_str() << " failed with result : " << f.what()<< std::endl;
        return false;
    }
    category_ = &log4cpp::Category::getRoot();
    return true;
}
Logger::Logger(): category_(&log4cpp::Category::getRoot())
{
    log4cpp::PatternLayout *pattern_one =
        new log4cpp::PatternLayout;
    pattern_one->setConversionPattern("%d: %p %c %x:%m%n");

    log4cpp::PatternLayout *pattern_two =
        new log4cpp::PatternLayout;
    pattern_two->setConversionPattern("%d: %p %c %x:%m%n");

    log4cpp::OstreamAppender *os_appender = 
        new log4cpp::OstreamAppender("osAppender",&std::cout);
    os_appender->setLayout(pattern_one);

    log4cpp::FileAppender *file_appender = 
        new log4cpp::FileAppender("fileAppender","bin/Test_Cuda.log");
    file_appender->setLayout(pattern_two);

    category_->setPriority(log4cpp::Priority::DEBUG);
    category_->addAppender(os_appender);
    category_->addAppender(file_appender);
}
Logger::~Logger(){

}

void Logger::setPriority(Priority priority) {
    switch (priority) {
        case (ERROR):
            category_->setPriority(log4cpp::Priority::ERROR);
            break;

        case (WARN):
            category_->setPriority(log4cpp::Priority::WARN);
            break;

        case (INFO):
            category_->setPriority(log4cpp::Priority::INFO);
            break;

        case (DEBUG):
            category_->setPriority(log4cpp::Priority::DEBUG);
            break;

        default:
            category_->setPriority(log4cpp::Priority::DEBUG);
            break;    
    }
}