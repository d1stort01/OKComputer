// FileManager.h - 文件管理类声明
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Graph.h"
#include "City.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// 文件管理类
class FileManager {
private:
    std::string cityDataFile;   // 城市数据文件路径
    std::string routeDataFile;  // 路线数据文件路径
    
public:
    // 构造函数
    FileManager(const std::string& cityFile = "cities.txt", 
                const std::string& routeFile = "routes.txt");
    
    // 设置文件路径
    void setCityDataFile(const std::string& filename);
    void setRouteDataFile(const std::string& filename);
    
    // 保存城市数据到文件
    bool saveCities(const std::vector<City>& cities) const;
    
    // 从文件加载城市数据
    bool loadCities(std::vector<City>& cities) const;
    
    // 保存路线数据到文件
    bool saveRoutes(const Graph& graph) const;
    
    // 从文件加载路线数据
    bool loadRoutes(Graph& graph) const;
    
    // 备份数据文件
    bool backupFiles(const std::string& backupDir) const;
    
    // 恢复数据文件
    bool restoreFiles(const std::string& backupDir);
    
    // 检查文件是否存在
    bool fileExists(const std::string& filename) const;
    
    // 创建默认数据文件（用于首次运行）
    bool createDefaultFiles() const;
    
    // 清空数据文件
    bool clearDataFiles() const;
};

#endif // FILEMANAGER_H