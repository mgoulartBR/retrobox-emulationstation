/* 
 * File:   SystemInfo.cpp
 * Author: matthieu
 * 
 * Created on 29 novembre 2014, 03:15
 */

#include "SystemInfo.h"
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sstream>
#include "Settings.h"
#include <iostream>
#include <fstream>

SystemInfo::SystemInfo() {
}

SystemInfo * SystemInfo::instance = NULL;

SystemInfo * SystemInfo::getInstance() {
    if (SystemInfo::instance == NULL) {
        SystemInfo::instance = new SystemInfo();
    }
    return SystemInfo::instance;
}

unsigned long SystemInfo::getFreeSpaceGB(std::string mountpoint) {
    struct statvfs fiData;
    const char * fnPath = mountpoint.c_str();
    int free = 0;
    if ((statvfs(fnPath, &fiData)) >= 0) {
        free = (fiData.f_bfree * fiData.f_bsize) / (1024 * 1024 * 1024);
    }
    return free;
}

std::string SystemInfo::getFreeSpaceInfo() {
    struct statvfs fiData;
    std::string sharePart = Settings::getInstance()->getString("SharePartition");
    if(sharePart.size() > 0){
        const char * fnPath = sharePart.c_str();
        if ((statvfs(fnPath, &fiData)) < 0) {
            return "";
        } else {
            unsigned long total = (fiData.f_blocks * (fiData.f_bsize / 1024)) / (1024L * 1024L);
            unsigned long free = (fiData.f_bfree * (fiData.f_bsize / 1024)) / (1024L * 1024L);
            unsigned long used = total - free;
            unsigned long percent = used * 100 / total;

            std::ostringstream oss;
            oss << used << "GB/" << total << "GB (" << percent << "%)";
            return oss.str();
        }
    } else {
        return "ERROR";
    }
}

bool SystemInfo::isFreeSpaceLimit() {
    std::string sharePart = Settings::getInstance()->getString("SharePartition");
    if(sharePart.size() > 0){
        return getFreeSpaceGB(sharePart) < 2;
    } else {
        return "ERROR";
    }
    
}

std::string SystemInfo::getVersion() {
    std::string version = Settings::getInstance()->getString("VersionFile");
    if (version.size() > 0) {
        std::ifstream ifs(version);

        if (ifs.good()) {
            std::string contents;
            std::getline(ifs,contents);
            return contents;
        }
    }
    return "";
}
