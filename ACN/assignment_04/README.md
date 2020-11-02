#### ACN Assignment 04 - Socket Programming

##### Build Instructions

```
mkdir build && cd build  
cmake ..  
make
```
![Build](./screenshot/build.png?raw=true "Build")

##### Run Instructions - SAWFTP Server
`./SAWFTPServer -i <bind_ip_address> -p <bind_port>`  
Parameters: 
- -i bind ip address
- -p bind port

##### Run Instructions - SAWFTP Client
`./SAWFTPClient -s <server_ip_address> -p <server_port> -f <filename>`
Parameters: 
- -s server ip address
- -p server port
- -f filename

![SAW FTP Client](./screenshot/saw_ftp_client.png?raw=true "SAW FTP Client")

##### Run Instructions - GBNFTP Server
`./GBNFTPServer -i <bind_ip_address> -p <bind_port>`  
Parameters: 
- -i bind ip address
- -p bind port

##### Run Instructions - GBNFTP Client
`./GBNFTPClient -s <server_ip_address> -p <server_port> -f <filename>`
Parameters: 
- -s server ip address
- -p server port
- -f filename

![GBN FTP Client](./screenshot/gbn_ftp_client.png?raw=true "GBN FTP Client")

#### Analysis
** File Size = 2mb file **

###### SAW FTP Client

    - 0 ms Delay = 16

    - 10 ms Delay = 4979

    - 20 ms Delay = 9878

    - 30 ms Delay = 14767

    - 40 ms Delay = 19655

###### GBN FTP Client

0 ms

    - 0% - 32
    
    - 2% - 4082
    
    - 4% - 9628
    
    - 6% - 13197
    
    - 8% - 19719
    
    - 10% - 22370

20 ms

    - 0% - 2796
    
    - 2% - 4610
    
    - 4% - 7608
    
    - 6% - 12666
    
    - 8% - 25398
    
    - 10% - 26390

40 ms

    - 0% - 5260
    
    - 2% - 6083
    
    - 4% - 8661
    
    - 6% - 16228
    
    - 8% - 20806
    
    - 10% - 29466


####PLAGIARISM STATEMENT

I certify that this assignment/report is my own work, based on my personal study and/or research and that I have acknowledged all material and sources used in its preparation, whether they be books, articles, reports, lecture notes, and any other kind of document, electronic or personal communication. I also certify that this assignment/report has not previously been submitted for assessment in any other course, except where specific permission has been granted from all course instructors involved, or at any other time in this course, and that I have not copied in part or whole or otherwise plagiarised the work of other students and/or persons. I pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honour violations by other students if I become aware of it. 

**Name:** Bhavishya Sharma  
**Roll No:** CS20MTECH12006

