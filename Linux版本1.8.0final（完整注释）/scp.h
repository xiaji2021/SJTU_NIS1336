//
// Created by 夏季 on 2023/7/8.
//

#ifndef SCP_TXT_CPP_SCP_H
#define SCP_TXT_CPP_SCP_H

#include <libssh/libssh.h>
#include <glob.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>


extern char txtstr[50][50];
ssh_session connect2server();
size_t file_bytes(char*  filename,char* buffer);

void scp_upload(ssh_session sshSession);
int load_index(ssh_session sshSession);
void scp_download(ssh_session sshSession);
void free_ssh_session(ssh_session sshSession);
#endif //SCP_TXT_CPP_SCP_H
