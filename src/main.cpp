#include <stdio.h>
#include "libyuv.h"

using namespace libyuv;

int write_file(const char* path, uint8_t* data, int size);

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("example\n   %s 640x480 yuvfile argbfile\n", argv[0]);
        return -1;
    }

    int ret;

    int width, height;
    sscanf(argv[1], "%dx%d", &width, &height);

    char *yuv_file_path = argv[2];
    char *argb_file_path = argv[3];
    char *rgba_file_path = argv[4];

    FILE *fp = fopen(yuv_file_path, "rb");
    if (!fp)
    {
        printf("failed to read file in path: %s\n", yuv_file_path);
        return -1;
    }

    int y_buf_size = width * height;
    uint8_t y_buf[y_buf_size];

    ret = fread(y_buf, 1, y_buf_size, fp);
    if (ret != y_buf_size)
    {
        printf("failed to read y data.\n");
        return -1;
    }

    int vu_buf_size = width * height / 2;
    uint8_t vu_buf[vu_buf_size];

    ret = fread(vu_buf, 1, vu_buf_size, fp);
    if (ret != vu_buf_size)
    {
        printf("failed to read vu data.\n");
        return -1;
    }

    fclose(fp);

    int argb_size = width * height * 4;
    uint8_t argb_buf[argb_size];

    ret = NV21ToARGB(y_buf, width, vu_buf, width, argb_buf, width * 4, width, height);
    if (ret != 0)
    {
        printf("failed to NV21ToARGB. %d\n", ret);
        return -1;
    }

    if (0 != write_file(argb_file_path, argb_buf, argb_size))
    {
        printf("failed to write argb file.\n");
        return -1;
    }

    int rgba_size = width * height * 4;
    uint8_t rgba_buf[rgba_size];

    ret = ARGBToRGBA(argb_buf, width * 4, rgba_buf, width * 4, width, height);
    if (ret != 0)
    {
        printf("failed to ARGBToRGBA. %d\n", ret);
        return -1;
    }

    if (0 != write_file(rgba_file_path, rgba_buf, rgba_size))
    {
        printf("failed to write argb file.\n");
        return -1;
    }

    printf("convert success.\n");

    return 0;
}

int write_file(const char* path, uint8_t* data, int size)
{
    FILE* f = fopen(path, "wb");
    if (!f)
    {
        printf("failed to open file: %s.\n", path);
        return -1;
    }

    int ret = fwrite(data, 1, size, f);
    if (ret < 0)
    {
        printf("failed to write file: %s. Ret: %d\n", path, ret);
        return -1;
    }

    fclose(f);

    if (ret != size)
    {
        printf("fail: wrote %d/%d bytes\n", ret, size);
        return -1;
    }

    return 0;
}
