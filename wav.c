

#include "wav.h"

#define HEAD_LENGTH 262144
// 256KB

void getHead(char *fname, struct WaveHeader *wh)
{
    int fd = open(fname, O_RDONLY);
    char buf[HEAD_LENGTH];
    int n;
    if (fd < 0)
    {
        fprintf(2, "getHead: cannot open %s\n", fname);
        exit(1);
    }
    if ((n = read(fd, buf, HEAD_LENGTH)) > 0)
    {
        int pos = 0;
        // 寻找“RIFF”标记
        while (pos < HEAD_LENGTH)
        {
            if (buf[pos] == 'R' && buf[pos + 1] == 'I' && buf[pos + 2] == 'F' && buf[pos + 3] == 'F')
            {
                wh->chunk_id[0] = 'R';
                wh->chunk_id[1] = 'I';
                wh->chunk_id[2] = 'F';
                wh->chunk_id[3] = 'F';
                pos += 4;
                break;
            }
            ++pos;
        }
        wh->chunk_size = *(int *)&buf[pos];
        pos += 4;
        wh->format[0] = buf[pos];
        wh->format[1] = buf[pos + 1];
        wh->format[2] = buf[pos + 2];
        wh->format[3] = buf[pos + 3];
        pos += 4;

        //寻找“fmt”标记
        while (pos < HEAD_LENGTH)
        {
            if (buf[pos] == 'f' && buf[pos + 1] == 'm' && buf[pos + 2] == 't')
            {
                wh->fmt_chunk_id[0] = 'f';
                wh->fmt_chunk_id[1] = 'm';
                wh->fmt_chunk_id[2] = 't';
                pos += 4;
                break;
            }
            ++pos;
        }

        //读取Format Chunk部分
        wh->fmt_chunk_size = *(int *)&buf[pos];
        pos += 4;
        wh->audio_fomat = *(short *)&buf[pos];
        pos += 2;
        wh->num_channels = *(short *)&buf[pos];
        pos += 2;
        wh->sample_rate = *(int *)&buf[pos];
        pos += 4;
        wh->byte_rate = *(int *)&buf[pos];
        pos += 4;
        wh->block_align = *(short *)&buf[pos];
        pos += 2;
        wh->bits_per_sample = *(short *)&buf[pos];
        pos += 2;

        //寻找“data”标记
        while (pos < HEAD_LENGTH)
        {
            if (buf[pos] == 'd' && buf[pos + 1] == 'a' && buf[pos + 2] == 't' && buf[pos + 3] == 'a')
            {
                wh->data_chunk_id[0] = 'd';
                wh->data_chunk_id[1] = 'a';
                wh->data_chunk_id[2] = 't';
                wh->data_chunk_id[3] = 'a';
                pos += 4;
                break;
            }
            ++pos;
        }

        //读取Data Chunk的非data部分
        wh->data_chunk_size = *(int *)&buf[pos];
        pos += 4;

        //记录真正音频数据的开始位置
        wh->start_pos = pos;

        //计算文件总帧数
        wh->num_frame = wh->data_chunk_size / (wh->num_channels * (wh->bits_per_sample / 8));
    }
    else
    {
        fprintf(2, "getHead: read error\n");
        exit(1);
    }
    close(fd);
}

void getData(char *fname, struct WaveHeader *wh)
{
    //记录文件读取位置
    int pos = wh->start_pos;

    //为加快处理速度，根据ChunkSize将文件一次读入内存
    int fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        fprintf(2, "getData: cannot open %s\n", fname);
        exit(1);
    }

    char file_data[wh->chunk_size + 8];
    int size = read(fd, file_data, wh->chunk_size + 8);
    close(fd);

    if (size != wh->chunk_size + 8)
    {
        fprintf(2, "getData: read error\n");
        exit(1);
    }

    fprintf(2, "getData: pos at %d\n", pos);
    fprintf(2, "getData: read %d bytes\n", size);

    /*
    //以每帧2字节为例
    short left_data;
    short right_data;

    while (pos < wh->start_pos + wh->data_chunk_size)
    {
        left_data = *(short *)&file_data[pos];
        // TODO: 处理左声道数据
        pos += 2;

        right_data = *(short *)&file_data[pos];
        // TODO: 处理右声道数据
        pos += 2;
    }
    */
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        exit(0);
    }

    else if (argc > 2)
    {
        fprintf(2, "Usage: %s [filename]\n", argv[0]);
        exit(1);
    }

    struct WaveHeader wh;
    getHead(argv[1], &wh);

    fprintf(2, "chunk_id: %c%c%c%c\n", wh.chunk_id[0], wh.chunk_id[1], wh.chunk_id[2], wh.chunk_id[3]);

    exit(0);
}
