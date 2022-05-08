

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

struct WaveHeader
{
    char chunk_id[4];
    uint chunk_size;
    char format[4];
    char fmt_chunk_id[4];
    uint fmt_chunk_size;
    ushort audio_fomat;
    ushort num_channels; //对应数字1，表示声道数为1，是个单声道Wav
    uint sample_rate;
    uint byte_rate; //通道数×每秒样本数×每样本的数据位数／8（1*22050*16/8）。播放软件利用此值可以估计缓冲区的大小
    ushort block_align;
    ushort bits_per_sample;//每样本的数据位数，表示每个声道中各个样本的数据位数。如果有多个声道，对每个声道而言，样本大小都一样
    char data_chunk_id[4];
    uint data_chunk_size;
    int num_frame;
    int start_pos;
};


void getHead(char *fname, struct WaveHeader *wh);
void getData(char *fname, struct WaveHeader *wh); 
int print_binary(int dec);
