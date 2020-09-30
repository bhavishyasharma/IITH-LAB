//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_QTYPE_H
#define IITH_LAB_QTYPE_H

#include <map>
#include <string>
#include <sys/types.h>

enum QType {
    QT_A = 1,
    QT_NS,
    QT_MD,
    QT_MF,
    QT_CNAME,
    QT_SOA,
    QT_MB,
    QT_MG,
    QT_MR,
    QT_NULL,
    QT_WKS,
    QT_PTR,
    QT_HINFO,
    QT_MINFO,
    QT_MX,
    QT_TXT,
    QT_AAAA = 28,
    QT_AXFR = 252,
    QT_MAILB,
    QT_MAILA,
    QT_ANY,
    QT_CAA = 257
};

static std::map<u_int16_t, std::string> qTypeMap = {{1, "A"}, {2, "NS"}, {3, "MD"}, {4, "MF"},
                                                    {5, "CNAME"}, {6, "SOA"}, {7, "MB"}, {8, "MG"},
                                                    {9, "MR"}, {10, "NULL"}, {11, "WKS"}, {12, "PTR"},
                                                    {13, "HINFO"}, {14, "MINFO"}, {15, "MX"}, {16, "TXT"},
                                                    {28, "AAAA"},
                                                    {252, "AXFR"}, {253, "MAILB"}, {254, "MAILA"}, {255, "ANY"},
                                                    {257, "CAA"}};

static std::map<std::string, u_int16_t> qTypeReverseMap = {{"A", 1}, {"NS", 2}, {"MD", 3}, {"MF", 4},
                                                           {"CNAME", 5}, {"SOA", 6}, {"MB", 7}, {"MG", 8},
                                                           {"MR", 9}, {"NULL", 10}, {"WKS", 11}, {"PTR", 12},
                                                           {"HINFO", 13}, {"MINFO", 14}, {"MX", 15}, {"TXT", 16},
                                                           {"AAAA", 28},
                                                           {"AXFR", 252}, {"MAILB", 253}, {"MAILA", 254}, {"ANY", 255},
                                                           {"CAA", 257}};

#endif //IITH_LAB_QTYPE_H
