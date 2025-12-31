/*
 * IP包解析测试程序
 * 功能：模拟解析预定义的IP包数据，用于演示和测试
 * 作者：IP包分析器
 * 日期：2025-12-24
 */

#include <iostream>
#include <iomanip>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring>
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
};

// 函数声明
void parse_ip_packet(const u_char* packet_data, int packet_len);
void print_packet_info(const IPPacketInfo& packet_info, int packet_count);
string get_protocol_name(uint8_t protocol);
void print_flags_info(uint8_t flags);
void print_ip_header(const IPPacketInfo& packet_info);
void print_hex_dump(const u_char* data, int length);

// 测试数据：预定义的IP包
const vector<vector<uint8_t>> test_packets = {
    // 测试包1: TCP SYN包 (192.168.1.100 -> 192.168.1.1)
    {
        0x45, 0x00, 0x00, 0x3c,  // Version/IHL, DSCP/ECN, Total Length
        0x4a, 0x2c, 0x40, 0x00,  // Identification, Flags/Fragment Offset
        0x40, 0x06, 0x12, 0x34,  // TTL, Protocol, Header Checksum
        0xc0, 0xa8, 0x01, 0x64,  // Source IP: 192.168.1.100
        0xc0, 0xa8, 0x01, 0x01,  // Destination IP: 192.168.1.1
        // TCP数据...
        0xab, 0xcd, 0xef, 0x01, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x50, 0x02, 0x71, 0x10,
        0x12, 0x34, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4,
        0x04, 0x02, 0x08, 0x0a, 0x00, 0x0f, 0x7a, 0x8b,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07
    },
    // 测试包2: UDP包 (8.8.8.8 -> 192.168.1.100)
    {
        0x45, 0x00, 0x05, 0xdc,  // Version/IHL, DSCP/ECN, Total Length
        0x3f, 0x1a, 0x20, 0x00,  // Identification, Flags/Fragment Offset
        0x3f, 0x11, 0x56, 0x78,  // TTL, Protocol, Header Checksum
        0x08, 0x08, 0x08, 0x08,  // Source IP: 8.8.8.8
        0xc0, 0xa8, 0x01, 0x64,  // Destination IP: 192.168.1.100
        // UDP数据...
        0x00, 0x35, 0x00, 0x35, 0x05, 0xc8, 0x12, 0x34,
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77,
        0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03,
        0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01
    },
    // 测试包3: ICMP Echo Request (192.168.1.100 -> 8.8.8.8)
    {
        0x45, 0x00, 0x00, 0x54,  // Version/IHL, DSCP/ECN, Total Length
        0x5c, 0x8f, 0x40, 0x00,  // Identification, Flags/Fragment Offset
        0x40, 0x01, 0x9a, 0xbc,  // TTL, Protocol, Header Checksum
        0xc0, 0xa8, 0x01, 0x64,  // Source IP: 192.168.1.100
        0x08, 0x08, 0x08, 0x08,  // Destination IP: 8.8.8.8
        // ICMP数据...
        0x08, 0x00, 0x12, 0x34, 0x00, 0x01, 0x00, 0x0a,
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
    }
};

int main() {
    cout << "========================================" << endl;
    cout << "     IP包解析测试程序" << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "本程序模拟解析预定义的IP包数据" << endl;
    cout << "用于演示IP包解析功能" << endl;
    cout << endl;
    
    // 解析每个测试包
    for (size_t i = 0; i < test_packets.size(); ++i) {
        cout << "\n========================================" << endl;
        cout << "测试包 #" << (i + 1) << endl;
        cout << "========================================" << endl;
        
        // 显示十六进制数据
        cout << "\n原始数据（十六进制）:" << endl;
        print_hex_dump(test_packets[i].data(), test_packets[i].size());
        
        // 解析IP包
        parse_ip_packet(test_packets[i].data(), test_packets[i].size());
        
        cout << "\n按Enter键继续...";
        cin.get();
    }
    
    cout << "\n========================================" << endl;
    cout << "测试完成！" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

// 解析IP包
void parse_ip_packet(const u_char* packet_data, int packet_len) {
    if (packet_len < 20) {
        cerr << "错误：IP包长度不足20字节" << endl;
        return;
    }
    
    // 解析IP头部
    struct ip ip_header;
    memcpy(&ip_header, packet_data, sizeof(struct ip));
    
    // 创建解析结果结构体
    IPPacketInfo packet_info;
    packet_info.version = ip_header.ip_v;
    packet_info.header_length = ip_header.ip_hl * 4;
    packet_info.total_length = ntohs(ip_header.ip_len);
    packet_info.identification = ntohs(ip_header.ip_id);
    packet_info.protocol = ip_header.ip_p;
    packet_info.checksum = ntohs(ip_header.ip_sum);
    
    // 解析标志位和片偏移
    uint16_t flags_fragoff = ntohs(ip_header.ip_off);
    packet_info.flags = (flags_fragoff & 0xE000) >> 13;
    packet_info.fragment_offset = flags_fragoff & 0x1FFF;
    
    // 转换IP地址
    inet_ntop(AF_INET, &(ip_header.ip_src), packet_info.source_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header.ip_dst), packet_info.dest_ip, INET_ADDRSTRLEN);
    
    // 打印解析结果
    print_packet_info(packet_info, 1);
    print_ip_header(packet_info);
}

// 打印包信息
void print_packet_info(const IPPacketInfo& packet_info, int packet_count) {
    cout << "\n[解析结果]" << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(20) << "字段名" << setw(25) << "值" << "说明" << endl;
    cout << "----------------------------------------" << endl;
}

// 打印IP头部
void print_ip_header(const IPPacketInfo& packet_info) {
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

// 打印十六进制数据
void print_hex_dump(const u_char* data, int length) {
    const int bytes_per_line = 16;
    
    for (int i = 0; i < length; i += bytes_per_line) {
        // 打印偏移
        cout << setw(4) << setfill('0') << hex << i << setfill(' ') << dec << "  ";
        
        // 打印十六进制
        for (int j = 0; j < bytes_per_line; ++j) {
            if (i + j < length) {
                cout << setw(2) << setfill('0') << hex << static_cast<int>(data[i + j]) << " ";
            } else {
                cout << "   ";
            }
        }
        
        cout << " ";
        
        // 打印ASCII
        for (int j = 0; j < bytes_per_line; ++j) {
            if (i + j < length) {
                char c = data[i + j];
                if (c >= 32 && c <= 126) {
                    cout << c;
                } else {
                    cout << ".";
                }
            }
        }
        
        cout << endl;
    }
}
