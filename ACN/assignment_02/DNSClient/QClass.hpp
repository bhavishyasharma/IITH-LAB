//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_QCLASS_H
#define IITH_LAB_QCLASS_H

#include <map>
#include <string>

enum QClass {
    QC_IN = 1,
    QC_CS,
    QC_CH,
    QC_HS,
    QC_ANY = 255
};

static std::map<u_int16_t, std::string> qClassMap = {{1, "IN"}, {2, "CS"}, {3, "CH"}, {4, "HS"},{255, "ANY"}};

static std::map<std::string, u_int16_t> qClassReverseMap = {{"IN", 1}, {"CS", 2}, {"CH", 3}, {"HS", 4}, {"ANY", 255}};

#endif //IITH_LAB_QCLASS_H
