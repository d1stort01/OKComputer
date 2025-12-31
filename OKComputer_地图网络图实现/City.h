// City.h - 城市信息类声明
#ifndef CITY_H
#define CITY_H

#include <string>
#include <vector>

class City {
private:
    int id;           // 城市唯一标识
    std::string name; // 城市名称

public:
    // 构造函数
    City();
    City(int cityId): id(cityId), name("") {};
    City(int cityId, const std::string& cityName);
    
    // 拷贝构造函数
    City(const City& other);
    
    // 赋值运算符重载
    City& operator=(const City& other);
    
    // 比较运算符重载
    bool operator==(const City& other) const;
    bool operator!=(const City& other) const;
    
    //  Getter 和 Setter 方法
    int getId() const;
    void setId(int cityId);
    
    std::string getName() const;
    void setName(const std::string& cityName);
    
    // 显示城市信息
    void display() const;
    
    // 友元函数：用于输出流
    friend std::ostream& operator<<(std::ostream& os, const City& city);
};

#endif // CITY_H
