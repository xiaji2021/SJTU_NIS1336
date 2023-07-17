//
// Created by 夏季 on 2023/7/6.
//

#include "scp.h"

//txt文件名存取的字符数组
char txtstr[50][50];

//连接远程服务器
ssh_session connect2server(){

    // 创建新的ssh连接进程
    ssh_session sshSession = ssh_new();
    // 处理错误
    if (sshSession == nullptr) {
        std::cout<<"连接进程创建失败"<<std::endl;
        return NULL;

    }

    // 设置连接选项
    ssh_options_set(sshSession, SSH_OPTIONS_HOST, "1.15.238.222");
    ssh_options_set(sshSession, SSH_OPTIONS_USER, "nis1336");

    // 连接到服务器
    int connectResult = ssh_connect(sshSession);
    // 处理连接错误
    if (connectResult != SSH_OK) {
        std::cout<<"ssh连接失败"<<std::endl;
        return NULL;
    }

    // 认证
    int authResult = ssh_userauth_password(sshSession, nullptr, "nis1336");
    // 处理认证错误
    if (authResult != SSH_AUTH_SUCCESS) {
        std::cout<<"ssh连接失败"<<std::endl;
        return NULL;
    }

    return sshSession;
}


size_t file_bytes(char*  filename,char* buffer){
    //获取字节数，同时获取文件内容

    FILE *file;
    long numBytes;

    // 打开文件
    file = fopen(filename , "rb");
    //错误处理
    if (file == NULL) {
        printf("无法打开文件.\n");
        return 1;
    }

    // 获取文件指针的位置，即文件字节数
    fseek(file, 0, SEEK_END);
    numBytes = ftell(file);

    //将指针重新指回开头
    rewind(file);

    //读取文件内容
    fread(buffer,sizeof(char),numBytes,file);
    fclose(file);

    return numBytes;
}


void scp_upload(ssh_session sshSession){

    //利用glob库获取当前目录下所有txt文件
    glob_t globResult;
    int globStatus = glob("*.txt", GLOB_TILDE, nullptr, &globResult);
    if(globStatus !=0 ){
        std::cout<<"没有需要上传的txt文件"<<std::endl;
        return;

    }

    //创建新的scp连接
    ssh_scp scp = ssh_scp_new(sshSession, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
    // 处理错误
    if (scp == NULL) {
        std::cout<<"scp创建失败"<<std::endl;
        return;
    }

    //创建用于存储所有txt文件名的index文件
    std::ofstream file("index.txt", std::ios_base::trunc);
    //错误处理
    if(file.fail())  {
        std::cout<<"警告：未能创建txt目录文件"<<std::endl;
        return;
    }

    //上传所有的除index以外的txt文件
    for (size_t i = 0; i < globResult.gl_pathc; i++) {

        //除开index
        if(strcmp("index.txt",globResult.gl_pathv[i]) == 0) continue;

        //文件路径字符
        char targetFilePath[64];
        //存储二进制文件内容的字符数组
        char buffer[4096];
        //连接
        sprintf(targetFilePath, "%s%s","/home/nis1336/", globResult.gl_pathv[i]);

        //创建新的scp会话
	    ssh_scp scp = ssh_scp_new(sshSession, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
        if(scp ==NULL) {
            std::cout<<"scp会话对象创建失败"<<std::endl;
            return;
        }

        //初始化会话
	    int init_result = ssh_scp_init(scp);
        if(init_result != SSH_OK) {
            std::cout<<"scp对象初始化失败"<<std::endl;
            return;
        }
        //获取文件大小
        size_t byte_size = file_bytes(globResult.gl_pathv[i],buffer);
        //推送文件（仅创建）
        int scpPushFileResult = ssh_scp_push_file(scp, targetFilePath, byte_size, 0666);
        if(scpPushFileResult != SSH_OK) std::cout<<"在远程创建"<<globResult.gl_pathv[i]<<"失败"<<std::endl;

        //二进制流传输文件
        int rc_write = ssh_scp_write(scp, buffer, byte_size);
        if (rc_write != SSH_OK)
        {
            printf("Can't write to remote file: %s\n",
                    ssh_get_error(sshSession));
            return;
        }

        //index文件中加入该文件名
        file<<globResult.gl_pathv[i]<<"\n";

        //关闭和释放会话
        ssh_scp_close(scp);
        ssh_scp_free(scp);


        }

    file.close();

    //对index进行相同操作

    char buffer[4096];

    ssh_scp scp_index = ssh_scp_new(sshSession, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
    if(scp_index ==NULL) {
        std::cout<<"scp会话对象创建失败"<<std::endl;
        return;
    }

    int init_result = ssh_scp_init(scp_index);
    if(init_result != SSH_OK) {
        std::cout<<"scp对象初始化失败"<<std::endl;
        return;
    }

    size_t byte_size = file_bytes("index.txt",buffer);
    int scp_push_index = ssh_scp_push_file(scp_index, "/home/nis1336/index.txt", byte_size, 0666);
    if(scp_push_index != SSH_OK) std::cout<<"在远程创建"<<"index.txt"<<"失败"<<std::endl;

    int rc_writei = ssh_scp_write(scp_index, buffer, byte_size);
    if (rc_writei != SSH_OK)
    {
        printf("Can't write to remote file: %s\n",
               ssh_get_error(sshSession));
        return;
    }

    ssh_scp_close(scp_index);
    ssh_scp_free(scp_index);

    return;
}

int load_index(ssh_session sshSession){

    //创建scp会话
    ssh_scp scp = ssh_scp_new(sshSession, SSH_SCP_READ, "index.txt");
    if(scp ==NULL) {
        std::cout<<"scp会话对象创建失败"<<std::endl;
        return -1;
    }

    //初始化scp
    int init_result = ssh_scp_init(scp);
    if(init_result != SSH_OK) {
        std::cout<<"scp对象初始化失败"<<std::endl;
        return -1;
    }

    //获取文件信息
    int rc;
    int size, mode;
    char *filename;
    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE){
        fprintf(stderr, "Error receiving information about file: %s\n",
                ssh_get_error(sshSession));
        return SSH_ERROR;
    }
    size = ssh_scp_request_get_size(scp);
    filename = strdup(ssh_scp_request_get_filename(scp));
    mode = ssh_scp_request_get_permissions(scp);
    printf("Receiving file %s, size %d, permisssions 0%o\n",
           filename, size, mode);
    free(filename);

    //接受scp请求
    ssh_scp_accept_request(scp);

    FILE* file = fopen("index.txt","wb");
    char buffer[4096];
    size_t bytes_read;
    int i =0;
    if((bytes_read = ssh_scp_read(scp,buffer,sizeof(buffer))) == SSH_ERROR){
        printf("远程读取数据错误");
        return -1;
    }
    fwrite(buffer, 1, bytes_read, file);
    fclose(file);

    //获取index中存储的文件名
    FILE * readFile = fopen("index.txt","r+");
    char line[64];
    while(fgets(line,64,readFile)){

        line[strcspn(line, "\n")] = '\0';
        strcpy(txtstr[i],line);
        printf("%s",txtstr[i]);
        i++;
    }
    fclose(readFile);

    ssh_scp_close(scp);
    ssh_scp_free(scp);

    return i;
}


//下载文件函数，代码逻辑同上
void scp_download(ssh_session sshSession) {

    int num = load_index(sshSession);

    for (int i = 0; i < num; i++) {
        char targetFilePath[64];
        sprintf(targetFilePath, "%s%s", "/home/nis1336/", txtstr[i]);
        std::cout<<targetFilePath<<std::endl;
        std::cout<<(const char*)txtstr[i];

        ssh_scp scp = ssh_scp_new(sshSession, SSH_SCP_READ,txtstr[i]);
        if (scp == NULL) {
            std::cout << "scp会话对象创建失败" << std::endl;
            return;
        }

        int init_result = ssh_scp_init(scp);
        if (init_result != SSH_OK) {
            std::cout << "scp对象初始化失败" << std::endl;
            return;
        }

        int rc;
        int size, mode;
        char *filename;
        rc = ssh_scp_pull_request(scp);
        if (rc != SSH_SCP_REQUEST_NEWFILE)
        {
            fprintf(stderr, "Error receiving information about file: %s\n",
                    ssh_get_error(sshSession));
            return ;
        }

        size = ssh_scp_request_get_size(scp);
        filename = strdup(ssh_scp_request_get_filename(scp));
        mode = ssh_scp_request_get_permissions(scp);
        printf("Receiving file %s, size %d, permisssions 0%o\n",
               filename, size, mode);
        free(filename);

        ssh_scp_accept_request(scp);//接受scp请求

        FILE *file = fopen(txtstr[i], "wb");
        char buffer[4096];
        size_t bytes_read;
        if((bytes_read = ssh_scp_read(scp,buffer,sizeof(buffer))) == SSH_ERROR){
            printf("远程读取数据错误");
            return ;
        }
        fwrite(buffer, 1, bytes_read, file);
        fclose(file);

        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }
    return;
}

void free_ssh_session(ssh_session sshSession){

    ssh_disconnect(sshSession);
    ssh_free(sshSession);

    return;
}
