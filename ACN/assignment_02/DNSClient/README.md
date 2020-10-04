#### ACN Assignment 02 - Socket Programming

##### Build Instructions

```
mkdir build && cd build  
cmake ..  
make
```
![Build](./screenshot/build.png?raw=true "Build")

##### Run Instructions - DNS Client
`./ChatClient -s <server_ip_address> -p <server_port> -d <domainame> -t <qtype> -c <qclass>`

`./ChatClient -s 192.168.35.52 -p 53 -d dns1.iith.ac.in -t ANY -c IN`
Parameters: 
- -s server ip address (Default: 8.8.8.8)
- -p server port (Default: 53)
- -d domain name (Default: www.google.com)
- -t DNS QType (Default: A)
- -c DNS QClass (Default: IN)

![DNS Client](./screenshot/dns_client.png?raw=true "DNS Client")

####Supported QTypes
- A
- NS
- CNAME
- SOA
- MX
- TXT
- AAAA

For QClass, only IN is supported.

####Default Values
- Server 8.8.8.8
- Port 53
- Domain Name www.google.com
- QClass IN
- QType A

#####Sample Input
```
./DNSClient -s 192.168.35.52 -p 53 -d dns1.iith.ac.in -t ANY -c IN
```

#####Question/Query
```
+----------------------------------------------------------+
|                          Header                          |
+----------------------------------------------------------+
    Transaction Id : 41A7
    Flags :  100
        Response : No
        Opcode : 0000 (Standard Query)
        Authoritative Answer : No
        Truncation : No
        Recursion Desired : Yes
        Recursion Available : No
        Z - Reserved : 000
        Opcode : 0000 (No Error)
    Question Count : 1
    Answer Count : 0
    Authority Count : 0
    Additional Count : 0
+----------------------------------------------------------+
|                        Questions                         |
+----------------------------------------------------------+
    Domain Name : dns1.iith.ac.in
        QType : 255 (ANY)
        QClass : 1 (IN)
```

#####Reply/Response
```
+----------------------------------------------------------+
|                          Header                          |
+----------------------------------------------------------+
    Transaction Id : 41A7
    Flags : 8580
        Response : Yes
        Opcode : 0000 (Standard Query)
        Authoritative Answer : Yes
        Truncation : No
        Recursion Desired : Yes
        Recursion Available : Yes
        Z - Reserved : 000
        Opcode : 0000 (No Error)
    Question Count : 1
    Answer Count : 1
    Authority Count : 2
    Additional Count : 1
+----------------------------------------------------------+
|                        Questions                         |
+----------------------------------------------------------+
    Domain Name : dns1.iith.ac.in
        QType : 255 (ANY)
        QClass : 1 (IN)
+----------------------------------------------------------+
|                         Answers                          |
+----------------------------------------------------------+
    Domain Name : dns1.iith.ac.in
        Size : 30
        Type : 1 (A)
        Class : 1 (IN)
        TTL : 7200
        Data Length : 4
        Data : 
            Address : 52.35.168.192
+----------------------------------------------------------+
|                       Authorities                        |
+----------------------------------------------------------+
    Domain Name : iith.ac.in
        Size : 47
        Type : 2 (NS)
        Class : 2 (IN)
        TTL : 86400
        Data Length : 7
        Data : 
            NS : dns2.iith.ac.in
    Domain Name : iith.ac.in
        Size : 47
        Type : 2 (NS)
        Class : 2 (IN)
        TTL : 86400
        Data Length : 2
        Data : 
            NS : dns1.iith.ac.in
+----------------------------------------------------------+
|                        Additional                        |
+----------------------------------------------------------+
    Domain Name : dns2.iith.ac.in
        Size : 30
        Type : 1 (A)
        Class : 1 (IN)
        TTL : 7200
        Data Length : 4
        Data : 
            Address : 53.36.168.192
```

####PLAGIARISM STATEMENT

I certify that this assignment/report is my own work, based on my personal study and/or research and that I have acknowledged all material and sources used in its preparation, whether they be books, articles, reports, lecture notes, and any other kind of document, electronic or personal communication. I also certify that this assignment/report has not previously been submitted for assessment in any other course, except where specific permission has been granted from all course instructors involved, or at any other time in this course, and that I have not copied in part or whole or otherwise plagiarised the work of other students and/or persons. I pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honour violations by other students if I become aware of it. 

**Name:** Bhavishya Sharma  
**Roll No:** CS20MTECH12006