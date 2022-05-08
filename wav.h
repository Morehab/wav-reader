

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
    ushort num_channels;
    uint sample_rate;
    uint byte_rate;
    ushort block_align;
    ushort bits_per_sample;
    char data_chunk_id[4];
    uint data_chunk_size;
    int num_frame;
    int start_pos;
};

void getHead(char *fname, struct WaveHeader *wh);
void getData(char *fname, struct WaveHeader *wh);