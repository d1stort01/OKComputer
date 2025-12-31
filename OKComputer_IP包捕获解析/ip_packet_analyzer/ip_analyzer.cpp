/*
 * IP包捕获与解析程序
 * 功能：捕获本机网卡的IP包并解析关键字段
 * 作者：IP包分析器
 * 日期：2025-12-24
 */

#include <iostream>
#include <iomanip>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

// 协议名称映射表
const map<uint8_t, string> PROTOCOL_NAMES = {
    {1, "ICMP"},
    {2, "IGMP"},
    {6, "TCP"},
    {17, "UDP"},
    {89, "OSPF"},
    {50, "ESP"},
    {51, "AH"},
    {58, "ICMPv6"}
};

// IP包解析结果结构体
struct IPPacketInfo {
    uint8_t version;          // 版本号
    uint8_t header_length;    // 首部长度
    uint16_t total_length;    // 总长度
    uint16_t identification;  // 标识
    uint8_t flags;           // 标志位
    uint16_t fragment_offset; // 片偏移
    uint8_t protocol;        // 协议
    uint16_t checksum;       // 首部校验和
    char source_ip[16];      // 源IP地址
    char dest_ip[16];        // 目的IP地址
    time_t timestamp;        // 捕获时间戳
};

// 函数声明
void packet_handler(u_char *user_data, const struct pcap_pkthdr* pkthdr, const u_char* packet);
void print_packet_info(const IPPacketInfo& packet_info, int packet_count);
string get_protocol_name(uint8_t protocol);
void print_flags_info(uint8_t flags);
void print_ip_header(const struct ip* ip_header, const IPPacketInfo& packet_info);
void list_all_devices();
string get_device_by_index(int index);

// 全局变量
vector<IPPacketInfo> captured_packets;
int packet_count = 0;

int main() {
    cout << "========================================" << endl;
    cout << "     IP包捕获与解析程序" << endl;
    cout << "========================================" << endl;
    cout << endl;

    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    const char *filter_exp = "ip";  // 只捕获IP包
    bpf_u_int32 net;

    // 列出所有可用网络设备
    cout << "正在检测网络设备..." << endl;
    list_all_devices();
    
    cout << "\n请选择要监听的网卡索引号(0-9): ";
    int device_index;
    cin >> device_index;
    
    string device = get_device_by_index(device_index);
    if (device.empty()) {
        cerr << "错误：无效的网卡索引号！" << endl;
        return 1;
    }
    
    cout << "\n正在打开网卡: " << device << endl;

    // 打开网络设备
    handle = pcap_open_live(device.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        cerr << "错误：无法打开网卡 - " << errbuf << endl;
        return 1;
    }

    cout << "网卡打开成功！" << endl;

    // 编译过滤器
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        cerr << "错误：无法编译过滤器 - " << pcap_geterr(handle) << endl;
        return 1;
    }

    // 应用过滤器
    if (pcap_setfilter(handle, &fp) == -1) {
        cerr << "错误：无法应用过滤器 - " << pcap_geterr(handle) << endl;
        return 1;
    }

    cout << "过滤器设置成功，只捕获IP包" << endl;
    cout << "\n开始捕获IP包... (按Ctrl+C停止)" << endl;
    cout << endl;

    // 开始捕获包
    pcap_loop(handle, -1, packet_handler, NULL);

    // 清理
    pcap_freecode(&fp);
    pcap_close(handle);

    return 0;
}

// 列出所有可用的网络设备
void list_all_devices() {
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
    int i = 0;

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        cerr << "错误：无法获取设备列表 - " << errbuf << endl;
        return;
    }

    cout << "\n可用的网络设备：" << endl;
    cout << "索引\t设备名称\t\t描述" << endl;
    cout << "------------------------------------------------" << endl;
    
    for (d = alldevs; d != NULL; d = d->next) {
        cout << i << "\t" << d->name;
        if (d->description) {
            cout << "\t" << d->description;
        } else {
            cout << "\t(无描述)";
        }
        cout << endl;
        i++;
        if (i >= 10) break;  // 最多显示10个设备
    }

    if (i == 0) {
        cout << "未找到网络设备！请确保以管理员权限运行程序。" << endl;
    }

    pcap_freealldevs(alldevs);
}

// 根据索引获取设备名称
string get_device_by_index(int index) {
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
    int i = 0;

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        return "";
    }

    for (d = alldevs; d != NULL; d = d->next) {
        if (i == index) {
            string result = d->name;
            pcap_freealldevs(alldevs);
            return result;
        }
        i++;
        if (i >= 10) break;
    }

    pcap_freealldevs(alldevs);
    return "";
}

// 包处理回调函数
void packet_handler(u_char *user_data, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    packet_count++;
    
    // 跳过以太网头部
    struct ether_header *eth_header = (struct ether_header *)packet;
    
    // 检查是否为IP包（以太网类型为0x0800）
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        return;
    }

    // 获取IP头部
    const u_char *ip_packet = packet + sizeof(struct ether_header);
    struct ip *ip_header = (struct ip *)ip_packet;

    // 解析IP包信息
    IPPacketInfo packet_info;
    packet_info.timestamp = pkthdr->ts.tv_sec;
    packet_info.version = ip_header->ip_v;
    packet_info.header_length = ip_header->ip_hl * 4;  // 转换为字节
    packet_info.total_length = ntohs(ip_header->ip_len);
    packet_info.identification = ntohs(ip_header->ip_id);
    packet_info.protocol = ip_header->ip_p;
    packet_info.checksum = ntohs(ip_header->ip_sum);
    
    // 解析标志位和片偏移
    uint16_t flags_fragoff = ntohs(ip_header->ip_off);
    packet_info.flags = (flags_fragoff & 0xE000) >> 13;  // 提取前3位作为标志位
    packet_info.fragment_offset = flags_fragoff & 0x1FFF;  // 提取后13位作为片偏移

    // 转换IP地址
    inet_ntop(AF_INET, &(ip_header->ip_src), packet_info.source_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->ip_dst), packet_info.dest_ip, INET_ADDRSTRLEN);

    // 保存捕获的包
    captured_packets.push_back(packet_info);

    // 打印包信息
    print_packet_info(packet_info, packet_count);
    print_ip_header(ip_header, packet_info);
    
    cout << "\n========================================" << endl;
}

// 打印包基本信息
void print_packet_info(const IPPacketInfo& packet_info, int packet_count) {
    cout << "\n[包 #" << packet_count << "]" << endl;
    cout << "捕获时间: " << ctime(&packet_info.timestamp);
    cout << "----------------------------------------" << endl;
    cout << left << setw(20) << "字段名" << setw(25) << "值" << "说明" << endl;
    cout << "----------------------------------------" << endl;
}

// 打印IP头部详细信息
void print_ip_header(const struct ip* ip_header, const IPPacketInfo& packet_info) {
    // 版本号
    cout << left << setw(20) << "版本号(Version)" << setw(25) << 
            static_cast<int>(packet_info.version) << "IPv" << static_cast<int>(packet_info.version) << endl;
    
    // 首部长度
    cout << left << setw(20) << "首部长度(IHL)" << setw(25) << 
            static_cast<int>(packet_info.header_length) << "字节" << endl;
    
    // 总长度
    cout << left << setw(20) << "总长度(Total Length)" << setw(25) << 
            packet_info.total_length << "字节" << endl;
    
    // 标识
    cout << left << setw(20) << "标识(Identification)" << setw(25) << 
            "0x" << hex << setfill('0') << setw(4) << packet_info.identification << 
            " (" << dec << packet_info.identification << ")" << setfill(' ') << endl;
    
    // 标志位
    cout << left << setw(20) << "标志位(Flags)" << setw(25) << 
            "0x" << hex << setfill('0') << setw(1) << static_cast<int>(packet_info.flags) << 
            setfill(' ') << dec << endl;
    print_flags_info(packet_info.flags);
    
    // 片偏移
    cout << left << setw(20) << "片偏移(Fragment Offset)" << setw(25) << 
            packet_info.fragment_offset << " * 8 = " << 
            (packet_info.fragment_offset * 8) << " 字节" << endl;
    
    // 协议
    string protocol_name = get_protocol_name(packet_info.protocol);
    cout << left << setw(20) << "协议(Protocol)" << setw(25) << 
            static_cast<int>(packet_info.protocol) << "(" << protocol_name << ")" << endl;
    
    // 首部校验和
    cout << left << setw(20) << "首部校验和" << setw(25) << 
            "0x" << hex << setfill('0') << setw(4) << packet_info.checksum << 
            setfill(' ') << dec << endl;
    
    // 源地址
    cout << left << setw(20) << "源IP地址(Source)" << setw(25) << 
            packet_info.source_ip << endl;
    
    // 目的地址
    cout << left << setw(20) << "目的IP地址(Destination)" << setw(25) << 
            packet_info.dest_ip << endl;
}

// 获取协议名称
string get_protocol_name(uint8_t protocol) {
    auto it = PROTOCOL_NAMES.find(protocol);
    if (it != PROTOCOL_NAMES.end()) {
        return it->second;
    }
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "协议%d", protocol);
    return buffer;
}

// 打印标志位详细信息
void print_flags_info(uint8_t flags) {
    bool reserved = (flags & 0x4) != 0;
    bool df = (flags & 0x2) != 0;  // Don't Fragment
    bool mf = (flags & 0x1) != 0;  // More Fragments
    
    cout << left << setw(20) << "" << setw(25) << "";
    cout << "[保留位: " << (reserved ? "1" : "0") << ", ";
    cout << "DF(不分片): " << (df ? "1" : "0") << ", ";
    cout << "MF(更多分片): " << (mf ? "1" : "0") << "]" << endl;
}
