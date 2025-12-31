// Graph.h - 图基类及其实现类声明
#ifndef GRAPH_H
#define GRAPH_H

#include "City.h"
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include <iomanip>

// 边结构体
struct Edge {
    int from;     // 起点城市ID
    int to;       // 终点城市ID
    int weight;   // 边的权重（距离）
    
    Edge(int f, int t, int w) : from(f), to(t), weight(w) {}
};

// 图基类（抽象类）
class Graph {
protected:
    std::vector<City> cities;  // 存储所有城市
    bool directed;             // 是否为有向图
    
public:
    // 构造函数
    Graph(bool isDirected = false);
    virtual ~Graph() {}
    
    // 纯虚函数 - 必须在派生类中实现
    virtual bool addVertex(const City& city) = 0;
    virtual bool removeVertex(int cityId) = 0;
    virtual bool addEdge(int fromId, int toId, int weight) = 0;
    virtual bool removeEdge(int fromId, int toId) = 0;
    virtual bool hasEdge(int fromId, int toId) const = 0;
    virtual int getEdgeWeight(int fromId, int toId) const = 0;
    virtual std::vector<int> getNeighbors(int cityId) const = 0;
    virtual void display() const = 0;
    virtual void clear() = 0;
    
    // 虚函数 - 可在派生类中重写
    virtual int getVertexCount() const;
    virtual bool hasVertex(int cityId) const;
    virtual bool hasVertex(const std::string& cityName) const;
    virtual City* getVertex(int cityId);
    virtual City* getVertex(const std::string& cityName);
    virtual std::vector<int> getAllVertexIds() const;
    
    // 图遍历算法
    virtual std::vector<int> dfs(int startId) const;  // 深度优先搜索
    virtual std::vector<int> bfs(int startId) const;  // 广度优先搜索
    
    // 最短路径算法
    virtual std::pair<std::vector<int>, int> dijkstra(int startId, int endId) const;
    
    // 获取城市名称
    std::string getCityName(int cityId) const;
};

// 邻接矩阵实现
class AdjacencyMatrix : public Graph {
private:
    std::vector<std::vector<int>> matrix;  // 邻接矩阵
    const int INF = INT_MAX;               // 表示无穷大（无边）
    
    // 扩展矩阵大小
    void resizeMatrix(int newSize);
    
public:
    // 构造函数
    AdjacencyMatrix(bool isDirected = false);
    
    // 重写基类虚函数
    bool addVertex(const City& city) override;
    bool removeVertex(int cityId) override;
    bool addEdge(int fromId, int toId, int weight) override;
    bool removeEdge(int fromId, int toId) override;
    bool hasEdge(int fromId, int toId) const override;
    int getEdgeWeight(int fromId, int toId) const override;
    std::vector<int> getNeighbors(int cityId) const override;
    void display() const override;
    void clear() override;
};

// 邻接表实现
class AdjacencyList : public Graph {
private:
    std::map<int, std::vector<std::pair<int, int>>> adjList;  // 邻接表：城市ID -> (邻居ID, 权重)
    
public:
    // 构造函数
    AdjacencyList(bool isDirected = false);
    
    // 重写基类虚函数
    bool addVertex(const City& city) override;
    bool removeVertex(int cityId) override;
    bool addEdge(int fromId, int toId, int weight) override;
    bool removeEdge(int fromId, int toId) override;
    bool hasEdge(int fromId, int toId) const override;
    int getEdgeWeight(int fromId, int toId) const override;
    std::vector<int> getNeighbors(int cityId) const override;
    void display() const override;
    void clear() override;
};

// 路径结果结构体
struct PathResult {
    std::vector<int> path;  // 路径上的城市ID序列
    int totalDistance;      // 总距离
    bool found;            // 是否找到路径
    
    PathResult() : totalDistance(0), found(false) {}
    PathResult(const std::vector<int>& p, int dist, bool f) 
        : path(p), totalDistance(dist), found(f) {}
};

#endif // GRAPH_H
