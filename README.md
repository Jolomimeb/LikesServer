Developed a LikesServer project using C programming language in the virtual machine which stimulates a simplified version of a YouTube-like system where likes on videos by users are distributed across multiple servers.
Established the PrimaryLikesServer to act as a central server, receiving and logging likes data from distributed LikesServers in a temp file which helped in robust process management.
Utilized bidirectional communication using sockets for interaction between LikesServers and PrimaryLikesServer. 
Implemented Eventual Consistency by enabling periodic updates from LikesServers to the central PrimaryLikesServer, preventing overloading and ensured secure programming by validating data integrity during interprocess communication, with a focus on error handling and logging for debugging.
