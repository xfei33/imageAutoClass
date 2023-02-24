#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "unzip.h"
#include "zip.h"

std::string getString(const std::string str, const char ch, const int pos) {
        int front_pos = 0;
        int rear_pos = 0;
        int cnt = 0;
        while(cnt <= pos && (rear_pos = str.find(ch, rear_pos)) != std::string::npos) {
                if(cnt == pos - 1 && pos > 0) {
                        front_pos = rear_pos + 1;
                }
                ++cnt;
                ++rear_pos;
        }
        // std::cout << front_pos << rear_pos << std::endl;
        std::string result(str, front_pos, rear_pos - front_pos - 1);
        return result;
}

int main(int argc, char* argv[]) {
        const char* zipfile = argv[1];                        // 压缩文件路径

        unzFile uf = unzOpen(zipfile);
        if(uf == NULL) {
                std::cerr << "Error opening " << zipfile << std::endl;
                return 1;
        }
        /* 读取压缩包内文件 */
        int err = unzGoToFirstFile(uf);
        while(err == UNZ_OK) {
                unz_file_info file_info;        // 文件名信息
                char filename[100];
                err = unzGetCurrentFileInfo(uf, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);
                if(err != UNZ_OK) {
                        std::cerr << "Error getting file info" << std::endl;
                        unzClose(uf);
                        return 1;
                }
                        
                std::string buliding = getString(static_cast<std::string>(filename), '_', 0);
                std::string number = getString(static_cast<std::string>(filename), '_', 1);
                err = unzOpenCurrentFile(uf);
                if(err != UNZ_OK) {
                        std::cerr << "Error opening" << filename << std::endl;
                        unzClose(uf);
                        return 1;
                }
                std::vector<char> buf(file_info.uncompressed_size);
                int len = unzReadCurrentFile(uf, &buf[0], buf.size());
                if(len < 0) {
                        std::cerr << "Error reading" << filename << std::endl;
                        unzCloseCurrentFile(uf);
                        unzClose(uf);
                        return 1;
                }
                unzCloseCurrentFile(uf);
                std::cout << "Decompressing...  -> " << filename << std::endl;
                
                time_t t = time(0);
                tm *now = localtime(&t);
                char datestr[40];
                strftime(datestr, 80, "%Y-%m-%d_%H_%M_%S", now);
                std::string date(datestr);
                std::string save_path =".\\照片" + date + "\\" + buliding +
                        "\\" + number + "\\";

                std::ofstream ofs;
                // createDir(date, buliding, number);
                std::string md = "md" + save_path;
                system(md.c_str());
                ofs.open(save_path + static_cast<std::string>(filename), 
                        std::ios::out | std::ios::binary);
                ofs.write(&buf[0], len);
                ofs.close();
                err = unzGoToNextFile(uf);
                std::cout << save_path << std::endl;
                std::cerr << argv[0] << std::endl;
        }
        unzClose(uf);
        system("pause");
        return 0;
}
