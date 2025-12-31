// FileManager.cpp - 文件管理类实现
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <sys/stat.h>

// 构造函数
FileManager::FileManager(const std::string& cityFile, const std::string& routeFile) 
    : cityDataFile(cityFile), routeDataFile(routeFile) {}

// 设置城市数据文件路径
void FileManager::setCityDataFile(const std::string& filename) {
    cityDataFile = filename;
}

// 设置路线数据文件路径
void FileManager::setRouteDataFile(const std::string& filename) {
    routeDataFile = filename;
}

// 保存城市数据到文件
bool FileManager::saveCities(const std::vector<City>& cities) const {
    std::ofstream file(cityDataFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& city : cities) {
        file << city.getId() << " " << city.getName() << std::endl;
    }
    
    file.close();
    return true;
}

// 从文件加载城市数据
bool FileManager::loadCities(std::vector<City>& cities) const {
    std::ifstream file(cityDataFile);
    if (!file.is_open()) {
        return false;
    }
    
    cities.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int cityId;
        std::string cityName;
        
        if (iss >> cityId >> cityName) {
            cities.emplace_back(cityId, cityName);
        }
    }
    
    file.close();
    return true;
}

// 保存路线数据到文件
bool FileManager::saveRoutes(const Graph& graph) const {
    std::ofstream file(routeDataFile);
    if (!file.is_open()) {
        return false;
    }
    
    auto allIds = graph.getAllVertexIds();
    
    // 遍历所有城市对，保存存在的路线
    for (size_t i = 0; i < allIds.size(); ++i) {
        for (size_t j = i + 1; j < allIds.size(); ++j) {
            int fromId = allIds[i];
            int toId = allIds[j];
            
            if (graph.hasEdge(fromId, toId)) {
                int weight = graph.getEdgeWeight(fromId, toId);
                if (weight > 0) {
                    file << fromId << " " << toId << " " << weight << std::endl;
                }
            }
        }
    }
    
    file.close();
    return true;
}

// 从文件加载路线数据
bool FileManager::loadRoutes(Graph& graph) const {
    std::ifstream file(routeDataFile);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int fromId, toId, weight;
        
        if (iss >> fromId >> toId >> weight) {
            graph.addEdge(fromId, toId, weight);
        }
    }
    
    file.close();
    return true;
}

// 备份数据文件
bool FileManager::backupFiles(const std::string& backupDir) const {
    // 创建备份目录
    #ifdef _WIN32
    _mkdir(backupDir.c_str());
    #else
    mkdir(backupDir.c_str(), 0755);
    #endif
    
    // 备份城市数据
    std::ifstream cityFileSrc(cityDataFile);
    std::ofstream cityFileDst(backupDir + "/" + cityDataFile);
    
    if (cityFileSrc.is_open() && cityFileDst.is_open()) {
        cityFileDst << cityFileSrc.rdbuf();
        cityFileSrc.close();
        cityFileDst.close();
    }
    
    // 备份路线数据
    std::ifstream routeFileSrc(routeDataFile);
    std::ofstream routeFileDst(backupDir + "/" + routeDataFile);
    
    if (routeFileSrc.is_open() && routeFileDst.is_open()) {
        routeFileDst << routeFileSrc.rdbuf();
        routeFileSrc.close();
        routeFileDst.close();
    }
    
    return true;
}

// 恢复数据文件
bool FileManager::restoreFiles(const std::string& backupDir) {
    // 恢复城市数据
    std::ifstream cityFileSrc(backupDir + "/" + cityDataFile);
    std::ofstream cityFileDst(cityDataFile);
    
    if (cityFileSrc.is_open() && cityFileDst.is_open()) {
        cityFileDst << cityFileSrc.rdbuf();
        cityFileSrc.close();
        cityFileDst.close();
    }
    
    // 恢复路线数据
    std::ifstream routeFileSrc(backupDir + "/" + routeDataFile);
    std::ofstream routeFileDst(routeDataFile);
    
    if (routeFileSrc.is_open() && routeFileDst.is_open()) {
        routeFileDst << routeFileSrc.rdbuf();
        routeFileSrc.close();
        routeFileDst.close();
    }
    
    return true;
}

// 检查文件是否存在
bool FileManager::fileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.is_open();
}

// 创建默认数据文件
bool FileManager::createDefaultFiles() const {
    // 创建默认城市数据
    std::ofstream cityFile(cityDataFile);
    if (cityFile.is_open()) {
        cityFile << "1 北京" << std::endl;
        cityFile << "2 上海" << std::endl;
        cityFile << "3 广州" << std::endl;
        cityFile << "4 深圳" << std::endl;
        cityFile << "5 杭州" << std::endl;
        cityFile << "6 南京" << std::endl;
        cityFile << "7 武汉" << std::endl;
        cityFile << "8 成都" << std::endl;
        cityFile.close();
    }
    
    // 创建默认路线数据
    std::ofstream routeFile(routeDataFile);
    if (routeFile.is_open()) {
        routeFile << "1 2 1200" << std::endl;  // 北京-上海
        routeFile << "1 6 1000" << std::endl;  // 北京-南京
        routeFile << "2 5 200" << std::endl;   // 上海-杭州
        routeFile << "3 4 150" << std::endl;   // 广州-深圳
        routeFile << "2 7 800" << std::endl;   // 上海-武汉
        routeFile << "5 3 1200" << std::endl;  // 杭州-广州
        routeFile << "7 8 1100" << std::endl;  // 武汉-成都
        routeFile << "6 7 500" << std::endl;   // 南京-武汉
        routeFile.close();
    }
    
    return true;
}

// 清空数据文件
bool FileManager::clearDataFiles() const {
    // 清空城市数据文件
    std::ofstream cityFile(cityDataFile, std::ios::trunc);
    if (cityFile.is_open()) {
        cityFile.close();
    }
    
    // 清空路线数据文件
    std::ofstream routeFile(routeDataFile, std::ios::trunc);
    if (routeFile.is_open()) {
        routeFile.close();
    }
    
    return true;
}