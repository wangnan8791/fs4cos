/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <Auth.h>
#include <Cosapi.h>
#include <cstring>

using namespace std;
using namespace Qcloud_cos;

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static Cosapi * cosapiPtr = NULL;
static string bucketName = "bucket4fs";

static int hello_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    /*
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else if (strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    } else
        res = -ENOENT;
    */

    ///*
    cosapiPtr->stat(
            bucketName, path);
    cosapiPtr->dump_res();
    if (cosapiPtr->retCode == 0) {
        if (strcmp(path, "/") == 0) {
            stbuf->st_mode = S_IFDIR | 0755;
            stbuf->st_nlink = 2;
        } else {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = 0;
        }
    } else if (cosapiPtr->retCode == -166) {
        string folderPath = "/";
        folderPath = path + folderPath;
        cosapiPtr->stat(
                bucketName, folderPath);
        cosapiPtr->dump_res();

        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        res = -ENOENT;
    }
    //*/

    return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
             off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    /*
    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);
    */

    ///*
    cosapiPtr->listFolder(
            bucketName, path);
    cosapiPtr->dump_res();
    
    if (true) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        //cout << "test:" << cosapiPtr->retJson["data"]["infos"][0]["name"].asString() << endl;
        //filler(buf, cosapiPtr->retJson["data"]["infos"][0]["name"].asString().c_str(), NULL, 0);
        const Json::Value &resArray = cosapiPtr->retJson["data"]["infos"];
        for (int i=0; i<resArray.size(); i++) {
            filler(buf, resArray[i]["name"].asString().c_str(), NULL, 0);
        }
    }
    //*/

    return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
    /*
    if (strcmp(path, hello_path) != 0)
        return -ENOENT;

    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;
    */

    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    /*
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;

    len = strlen(hello_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
    } else
        size = 0;
    */

    return size;
}

/*
static struct fuse_operations hello_oper = {
    .getattr    = hello_getattr,
    .readdir    = hello_readdir,
    .open        = hello_open,
    .read        = hello_read,
};
*/

static struct fuse_operations hello_oper;

int main(int argc, char *argv[])
{
    Cosapi::global_init();
    cosapiPtr = new Cosapi(
            10001380,
            "AKIDcZzRgNsMtiFWczqZB8SsF1xrT8aYqYPB",
            "9Ha9jULA208wJmB13hHlyTPMcKi3FawG");

#ifdef __cplusplus
    cout << "cpp is defined" << endl;
#endif
    cout << "test:" << hello_oper.getattr << endl;
    hello_oper.getattr    = hello_getattr;
    hello_oper.readdir    = hello_readdir;
    hello_oper.open       = hello_open;
    hello_oper.read       = hello_read;
    cout << "test:" << hello_oper.getattr << endl;
    return fuse_main(argc, argv, &hello_oper, NULL);
}
