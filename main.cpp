#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <SDL.h>
#include "fltk.h"

#define VERSION_MAJOR       0
#define VERSION_MINOR       9
#define VERSION_SUBMINOR    0
#define VERSION_STRING      "0.9.0"

#define MAX_BANKS   8

struct bank_info {
    int size;
    uint32_t crc32;
    const char *const name;
} bank_info[] = {
    { 2048, 0xf77e2e94, "LM-1 Bass Drum Bank 0" },
    { 2048, 0x6779b7e8, "LM-1 Bass Drum Bank 1" },
    { 2048, 0xfc6ffddb, "LM-1 Cabasa" },
    { 2048, 0x60fba06f, "LM-1 Conga Bank 0" },
    { 2048, 0x5c0fc62b, "LM-1 Conga Bank 1" },
    { 2048, 0x2efcec08, "LM-1 Clap Bank 0" },
    { 2048, 0x01a4c645, "LM-1 Clap Bank 1" },
    { 2048, 0x6160a181, "LM-1 Cowbell" },
    { 2048, 0xae3c416b, "LM-1 Hi-Hat Bank 0" },
    { 2048, 0x2916f43a, "LM-1 Hi-Hat Bank 1" },
    { 2048, 0x0b99cdf1, "LM-1 Rim" },
    { 2048, 0x0ad2479e, "LM-1 Snare Bank 0" },
    { 2048, 0x6d611f5b, "LM-1 Snare Bank 1" },
    { 2048, 0xd634ceaf, "LM-1 Tambourine Bank 0" },
    { 2048, 0xa24fb9cd, "LM-1 Tambourine Bank 1" },
    { 2048, 0x1a5c2a0f, "LM-1 Tom Bank 0" },
    { 2048, 0x430cb76f, "LM-1 Tom Bank 1" },
    { 4096, 0x57eb8886, "LinnDrum Bass 1" },
    { 4096, 0x7dbacb9b, "LinnDrum Bass 6" },
    { 4096, 0x9535274a, "LinnDrum Cabasa" },
    { 4096, 0x1a579539, "LinnDrum Conga Bank 0" },
    { 4096, 0x0b13c231, "LinnDrum Conga Bank 1" },
    { 4096, 0xede0860d, "LinnDrum Claps" },
    { 4096, 0x819d4a2c, "LinnDrum Cowbell" },
    { 4096, 0x85ce8dc5, "LinnDrum Crash Bank 0" },
    { 4096, 0x3681d6f0, "LinnDrum Crash Bank 1" },
    { 4096, 0xff1a4d87, "LinnDrum Crash Bank 2" },
    { 4096, 0x7f623944, "LinnDrum Crash Bank 3" },
    { 4096, 0x851c306a, "LinnDrum Crash Bank 4" },
    { 4096, 0x2fee35fe, "LinnDrum Crash Bank 5" },
    { 4096, 0x53f939bb, "LinnDrum Crash Bank 6" },
    { 4096, 0x7b9f55c2, "LinnDrum Crash Bank 7" },
    { 4096, 0x20b35416, "LinnDrum Hi Hat Bank 0" },
    { 4096, 0xfb4b3bac, "LinnDrum Hi Hat Bank 1" },
    { 4096, 0x62d1e667, "LinnDrum Hi Hat Bank 2" },
    { 4096, 0x01819ab1, "LinnDrum Hi Hat Bank 3" },
    { 4096, 0x3d0a852f, "LinnDrum Ride Bank 0" },
    { 4096, 0x5bb0e082, "LinnDrum Ride Bank 1" },
    { 4096, 0xfa48f0e3, "LinnDrum Ride Bank 2" },
    { 4096, 0x3a8b4133, "LinnDrum Ride Bank 3" },
    { 4096, 0xe30dc9a2, "LinnDrum Ride Bank 4" },
    { 4096, 0xba63cc66, "LinnDrum Ride Bank 5" },
    { 4096, 0xd5e24b3e, "LinnDrum Ride Bank 6" },
    { 4096, 0x4ac922bb, "LinnDrum Ride Bank 7" },
    { 4096, 0xc51a5407, "LinnDrum Shaker" },
    { 4096, 0x846681b2, "LinnDrum Snap Bank 0" },
    { 4096, 0xc71c0011, "LinnDrum Snap Bank 1" },
    { 4096, 0xdc06b36a, "LinnDrum Snare 23" },
    { 4096, 0x6f19e08c, "LinnDrum Snare 1" },
    { 4096, 0xfda3a90f, "LinnDrum Sidestick" },
    { 4096, 0x0309eba3, "LinnDrum Tambourine" },
    { 4096, 0x67968e2e, "LinnDrum Tom 1 Bank 0" },
    { 4096, 0x00525d95, "LinnDrum Tom 1 Bank 1" },
    { 4096, 0x75f83e43, "LinnDrum Tom 6 Bank 0" },
    { 4096, 0xc7633ca4, "LinnDrum Tom 6 Bank 1" },
    { 4096, 0x3b9bbfc1, "LinnDrum Tom 7 Bank 0" },
    { 4096, 0x086cc497, "LinnDrum Tom 7 Bank 1" },
    { 8192, 0x45151647, "Linn 9000 Bass Drum" },
    { 8192, 0xabaf7243, "Linn 9000 Bell Bank 0" },
    { 8192, 0x39c058bd, "Linn 9000 Bell Bank 1" },
    { 8192, 0x4a16643e, "Linn 9000 Bell Bank 2" },
    { 8192, 0x3de51074, "Linn 9000 Bell Bank 3" },
    { 8192, 0x3214fa50, "Linn 9000 Cabasa" },
    { 8192, 0x4e73b5f9, "Linn 9000 Claps" },
    { 8192, 0x3145f40d, "Linn 9000 Conga" },
    { 8192, 0x1d3c641f, "Linn 9000 Cowbell" },
    { 8192, 0x0a1e0e72, "Linn 9000 Crash Bank 0" },
    { 8192, 0xd5a16983, "Linn 9000 Crash Bank 1" },
    { 8192, 0xb5fc7526, "Linn 9000 Crash Bank 2" },
    { 8192, 0xa89b0d1c, "Linn 9000 Crash Bank 3" },
    { 8192, 0x8db2c264, "Linn 9000 Crash Bank 4" },
    { 8192, 0xb5175fc9, "Linn 9000 Crash Bank 5" },
    { 8192, 0x76910fe3, "Linn 9000 Hi Hat Bank 0" },
    { 8192, 0x0fa21007, "Linn 9000 Hi Hat Bank 1" },
    { 8192, 0x9e6a8536, "Linn 9000 Ride Bank 0" },
    { 8192, 0x73437d48, "Linn 9000 Ride Bank 1" },
    { 8192, 0x5f077df1, "Linn 9000 Ride Bank 2" },
    { 8192, 0x216d3a94, "Linn 9000 Ride Bank 3" },
    { 8192, 0xe910b9bc, "Linn 9000 Snare" },
    { 8192, 0x285a8f60, "Linn 9000 Sidestick" },
    { 8192, 0xdccfffc3, "Linn 9000 Tambourine" },
    { 8192, 0x219594f8, "Linn 9000 Tom" },
    { 8192, 0x75b90386, "Oberheim DX Stock Conga" },
    { 8192, 0x7726c645, "Oberheim DX Stock Cowbell" },
    { 8192, 0x9c51a0b4, "Oberheim DX Stock Hi-Hat" },
    { 8192, 0x45d4618c, "Oberheim DX Stock Shaker" },
    { 8192, 0x1c8481e3, "Oberheim DX Stock Tom" },
    { 8192, 0x4fd0c6ac, "Oberheim DX Stock Fat Snare" },
    { 8192, 0xb80749fc, "Oberheim DX Stock Bass" },
    { 8192, 0x867effea, "Oberheim DX Stock Regular Snare" },
    { 8192, 0x83a604ce, "Oberheim DX Stock Rim / Tambourine" },
    { 8192, 0xae4e945f, "Oberheim DX Stock Timbale" },
    { 4096, 0x3ab63275, "Oberheim DMX Stock Bass" },
    { 4096, 0x36e934f9, "Oberheim DMX Stock Hi-Hat" },
    { 4096, 0xf5b48db0, "Oberheim DMX Stock Floor Tom" },
    { 4096, 0x00549f07, "Oberheim DMX Stock Clap / Shaker" },
    { 4096, 0xeca57ad7, "Oberheim DMX Stock Snare" },
    { 4096, 0x61af39e3, "Oberheim DMX Stock Rim / Tambourine" },
    { 4096, 0xe8022d72, "Oberheim DMX Stock Tom" },
    { 4096, 0xe27ed36a, "Drumulator, Stock, Clap" },
    { 4096, 0xe5c53765, "Drumulator, Stock, Clave" },
    { 4096, 0x1dfe36bc, "Drumulator, Stock, Closed Hi-Hat 1" },
    { 4096, 0xca8f157b, "Drumulator, Stock, Closed Hi-Hat 2" },
    { 4096, 0x943dbe15, "Drumulator, Stock, Cowbell" },
    { 16384, 0x0abd3f8a, "Drumulator, Stock, Crash" },
    { 4096, 0xa43e91f3, "Drumulator, Stock, Kick 1" },
    { 4096, 0xdc058347, "Drumulator, Stock, Kick 2" },
    { 4096, 0xa0aeeb7e, "Drumulator, Stock, Metronome" },
    { 16384, 0x535eb1c1, "Drumulator, Stock, Open Hi-Hat" },
    { 32768, 0xf598eca0, "Drumulator, Stock, Ride 1" },
    { 65536, 0x458a6bd1, "Drumulator, Stock, Ride 2" },
    { 4096, 0xe379668e, "Drumulator, Stock, Rim" },
    { 4096, 0x88cfd284, "Drumulator, Stock, Snare 1" },
    { 4096, 0x427c30ef, "Drumulator, Stock, Snare 2" },
    { 8192, 0x601137d3, "Drumulator, Stock, Snare 3" },
    { 8192, 0x05963a89, "Drumulator, Stock, Tom 1" },
    { 8192, 0x5500b985, "Drumulator, Stock, Tom 2" },
    { 16384, 0x191bfba1, "Drumulator, Stock, Tom 3" },
    { 4096, 0x4226d7d4, "Drumulator, Electric 1, Clap" },
    { 4096, 0xec3d4599, "Drumulator, Electric 1, Closed Hi-Hat" },
    { 4096, 0x6bba4962, "Drumulator, Electric 1, Cowbell" },
    { 8192, 0xa110e309, "Drumulator, Electric 1, Kick" },
    { 8192, 0x32b293b2, "Drumulator, Electric 1, Open Hi-Hat" },
    { 8192, 0xb597646a, "Drumulator, Electric 1, Rim" },
    { 8192, 0x6c65007f, "Drumulator, Electric 1, Slap" },
    { 8192, 0x1ad79089, "Drumulator, Electric 1, Snare" },
    { 8192, 0xc82905df, "Drumulator, Electric 1, Tamb" },
    { 16384, 0xd59dd78f, "Drumulator, Electric 1, Tom 1" },
    { 16384, 0x46c285b6, "Drumulator, Electric 1, Tom 2" },
    { 16384, 0xd6811f01, "Drumulator, Electric 1, Tom 3" },
    { 8192, 0xedd49162, "Drumulator, Electric 2, China" },
    { 4096, 0x7af60f96, "Drumulator, Electric 2, Closed Hi-Hat" },
    { 4096, 0x466ef0a8, "Drumulator, Electric 2, Kick 1" },
    { 4096, 0xe34468db, "Drumulator, Electric 2, Kick 2" },
    { 4096, 0x6d8d32c1, "Drumulator, Electric 2, Kick 3" },
    { 32768, 0x2d641a65, "Drumulator, Electric 2, Open Hi-Hat" },
    { 16384, 0x697e08bb, "Drumulator, Electric 2, Ride" },
    { 8192, 0x6b9b534a, "Drumulator, Electric 2, Slap" },
    { 8192, 0x03b9231e, "Drumulator, Electric 2, Snare 1" },
    { 8192, 0x5fedabb5, "Drumulator, Electric 2, Snare 2" },
    { 8192, 0x969aae7d, "Drumulator, Electric 2, Snare 3" },
    { 4096, 0xd16b8a0b, "Drumulator, Electric 2, Squelch" },
    { 16384, 0x94e81089, "Drumulator, Electric 2, Sync Cymbal" },
    { 8192, 0x6861852e, "Drumulator, Electric 2, Tambourine" },
    { 8192, 0xcfbc05c3, "Drumulator, Electric 2, Tape Snap" },
    { 8192, 0xf7239d98, "Drumulator, Electric 2, Tom 1" },
    { 8192, 0xe7cbaedb, "Drumulator, Electric 2, Tom 2" },
    { 8192, 0x33bb43f6, "Drumulator, Electric 2, Tom 3" },
    { 4096, 0x8af9df41, "Drumulator, African Percussion, Agogo" },
    { 4096, 0x20baa036, "Drumulator, African Percussion, Agogo Low" },
    { 8192, 0x73899527, "Drumulator, African Percussion, Bell Shake" },
    { 16384, 0x3a72af7e, "Drumulator, African Percussion, Big Tom" },
    { 4096, 0x79f8abfd, "Drumulator, African Percussion, Bongo" },
    { 4096, 0xb748c686, "Drumulator, African Percussion, Closed Triangle" },
    { 4096, 0x181adce9, "Drumulator, African Percussion, Conga Hi" },
    { 4096, 0x62fae16a, "Drumulator, African Percussion, Metal Block" },
    { 16384, 0xb2a6c34a, "Drumulator, African Percussion, Open Triangle" },
    { 8192, 0xddbb4703, "Drumulator, African Percussion, Tom 1" },
    { 8192, 0xf9b55fd5, "Drumulator, African Percussion, Tom 2" },
    { 4096, 0xe67ad337, "Drumulator, Latin Percussion, Agogo High" },
    { 4096, 0xb9bc443a, "Drumulator, Latin Percussion, Agogo Low" },
    { 4096, 0x96f2bb25, "Drumulator, Latin Percussion, Block" },
    { 4096, 0xe32c1cc0, "Drumulator, Latin Percussion, Cabasa" },
    { 4096, 0x602e4acc, "Drumulator, Latin Percussion, Conga 1" },
    { 4096, 0xed3e7102, "Drumulator, Latin Percussion, Conga 2" },
    { 4096, 0x691e6474, "Drumulator, Latin Percussion, Cowbell" },
    { 4096, 0xe9c700bb, "Drumulator, Latin Percussion, Tambourine" },
    { 4096, 0x43ed1ffa, "Drumulator, Latin Percussion, Timbale High" },
    { 8192, 0x6a3dacda, "Drumulator, Latin Percussion, Timbale Low" },
    { 8192, 0x4cdaf4fd, "Drumulator, Latin Drums, Agogo High" },
    { 8192, 0x40d4e153, "Drumulator, Latin Drums, Agogo Low" },
    { 4096, 0x16708238, "Drumulator, Latin Drums, Cabasa" },
    { 4096, 0x91cbb82e, "Drumulator, Latin Drums, Clave" },
    { 4096, 0x634228a7, "Drumulator, Latin Drums, Kick" },
    { 8192, 0x86093665, "Drumulator, Latin Drums, Slap" },
    { 8192, 0xa27bf858, "Drumulator, Latin Drums, Snare" },
    { 8192, 0x7531c857, "Drumulator, Latin Drums, Timbale High 2" },
    { 8192, 0xeefe3c66, "Drumulator, Latin Drums, Timbale High" },
    { 8192, 0x2f4aa74a, "Drumulator, Latin Drums, Timbale Low" },
    { 8192, 0x7faa3b74, "Drumulator, Alternative Drums, Clap" },
    { 4096, 0x139e96a1, "Drumulator, Alternative Drums, Closed Hi-Hat" },
    { 16384, 0xb49f602d, "Drumulator, Alternative Drums, Crash" },
    { 8192, 0xf8087138, "Drumulator, Alternative Drums, Electric Kick" },
    { 4096, 0x0cf42f21, "Drumulator, Alternative Drums, Kick" },
    { 4096, 0xe6f45fa9, "Drumulator, Alternative Drums, Open Hi-Hat" },
    { 32768, 0xf9894ca9, "Drumulator, Alternative Drums, Ride" },
    { 4096, 0xf0aa2e7b, "Drumulator, Alternative Drums, Rim" },
    { 16384, 0x84722e74, "Drumulator, Alternative Drums, Shot" },
    { 16384, 0x338c0cac, "Drumulator, Alternative Drums, Snare" },
    { 8192, 0x76f604b4, "Drumulator, Alternative Drums, Tom 1" },
    { 8192, 0x155acdf3, "Drumulator, Alternative Drums, Tom 2" },
    { 32768, 0xea573559, "Drumulator, Jazz Drums, China" },
    { 4096, 0x17540fcb, "Drumulator, Jazz Drums, Closed Hi-Hat" },
    { 16384, 0x12715c95, "Drumulator, Jazz Drums, Crash" },
    { 8192, 0x4c04e616, "Drumulator, Jazz Drums, Kick" },
    { 16384, 0xbb8c5398, "Drumulator, Jazz Drums, Open Hi-Hat" },
    { 16384, 0xc9c52f83, "Drumulator, Jazz Drums, Ride 2" },
    { 65536, 0x8fda8b90, "Drumulator, Jazz Drums, Ride" },
    { 4096, 0xecc4b664, "Drumulator, Jazz Drums, Rim" },
    { 8192, 0xd10efddf, "Drumulator, Jazz Drums, Snare" },
    { 8192, 0x53d52715, "Drumulator, Jazz Drums, Tom 1" },
    { 8192, 0x33c304d8, "Drumulator, Jazz Drums, Tom 2" },
    { 16384, 0x81c7a770, "Drumulator, Jazz Drums, Tom 3" },
    { 4096, 0x1e29f846, "Drumulator, Rock Drums, Clap" },
    { 4096, 0x042c5697, "Drumulator, Rock Drums, Closed Hi-Hat" },
    { 16384, 0x92c41227, "Drumulator, Rock Drums, Crash" },
    { 8192, 0x8392e562, "Drumulator, Rock Drums, Kick 1" },
    { 8192, 0x7f41b6f9, "Drumulator, Rock Drums, Kick 2" },
    { 8192, 0x45961a27, "Drumulator, Rock Drums, Kick 3" },
    { 16384, 0x8c36cb77, "Drumulator, Rock Drums, Open Hi-Hat" },
    { 32768, 0x4eafa5e8, "Drumulator, Rock Drums, Ride" },
    { 8192, 0xbfb3cf44, "Drumulator, Rock Drums, Snare 1" },
    { 8192, 0x1aaa7116, "Drumulator, Rock Drums, Snare 2" },
    { 16384, 0xabd52ad2, "Drumulator, Rock Drums, Snare 3" },
    { 8192, 0x374d04c3, "Drumulator, Rock Drums, Snare 4" },
    { 8192, 0x2b6960db, "Drumulator, Rock Drums, Tom 1" },
    { 16384, 0x9c588dea, "Drumulator, Rock Drums, Tom 2" },
    { 8192, 0xc3b7b53c, "Drumulator, SFX, Bug Hit" },
    { 4096, 0x583fb85f, "Drumulator, SFX, Cabasa" },
    { 8192, 0x6e1cdcfd, "Drumulator, SFX, Car Beep" },
    { 8192, 0xd6e2377d, "Drumulator, SFX, Crack" },
    { 4096, 0x89148e85, "Drumulator, SFX, Dog" },
    { 16384, 0x12602b3c, "Drumulator, SFX, Gunfire" },
    { 16384, 0x4b03116b, "Drumulator, SFX, Horn" },
    { 16384, 0x7f567e35, "Drumulator, SFX, Reverse Kick" },
    { 16384, 0xdea73022, "Drumulator, SFX, Snap" },
    { 4096, 0x5e46d93e, "Drumulator, TR-808, Cabasa" },
    { 4096, 0xd3621835, "Drumulator, TR-808, Clap" },
    { 4096, 0x107ccf02, "Drumulator, TR-808, Closed Hi-Hat 2" },
    { 4096, 0x5beaaf4c, "Drumulator, TR-808, Closed Hi-Hat" },
    { 16384, 0x8b6ebfc0, "Drumulator, TR-808, Cowbell" },
    { 8192, 0x8ca34c7a, "Drumulator, TR-808, Open Hi-Hat" },
    { 4096, 0x305eb429, "Drumulator, TR-808, Rim" },
    { 4096, 0x6de014c8, "Drumulator, TR-808, Rim Click" },
    { 4096, 0xc05e8569, "Drumulator, TR-808, Snare 1" },
    { 4096, 0x0c121ea2, "Drumulator, TR-808, Snare 2" },
    { 4096, 0x2f2238a9, "Drumulator, TR-808, Tom 1" },
    { 4096, 0x27ea457a, "Drumulator, TR-808, Tom 2" },
    { 16384, 0x81ba8882, "Drumulator, TR-808, Tom 3" },
    { 8192, 0xd9a944bb, "Drumulator, Various Hits, Snare" },
    { 8192, 0xf787a1cc, "Drumulator, Various Hits, Kick" },
    { 32768, 0x385dd873, "Drumulator, Various Hits, Crash" },
    { 4096, 0xa0267b9d, "Drumulator, Various Hits, Closed Hi-Hat" },
    { 8192, 0xe2c8eb41, "Drumulator, Various Hits, Clap" },
    { 4096, 0xd0ab5931, "Drumulator, Various Hits, Kick 1" },
    { 4096, 0x7a57dfce, "Drumulator, Various Hits, Kick 2" },
    { 16384, 0x49eb2072, "Drumulator, Various Hits, Kick 3" },
    { 16384, 0x81b5a933, "Drumulator, Various Hits, Kick 4" },
    { 8192, 0x1ea7f038, "Drumulator, Various Hits, Kick 5" },
    { 16384, 0xd8f525fd, "Drumulator, Various Hits, Kick 6" },
    { 4096, 0xd6ecc5c8, "Drumulator, Various Hits, Kick 7" },
    { 4096, 0xde8e7a3e, "Drumulator, Various Hits, Snare 0" },
    { 16384, 0x28c14361, "Drumulator, Various Hits, Snare 1" },
    { 4096, 0x3690386e, "Drumulator, Various Hits, Snare 2" },
    { 16384, 0x95326b8a, "Drumulator, Various Hits, Snare 3" },
    { 4096, 0xdf72f2a1, "Drumulator, Various Hits, Snare 4" },
    { 16384, 0xa3f79715, "Drumulator, Various Hits, Tom 0" },
    { 16384, 0x5a1739f4, "Drumulator, Various Hits, Tom 1" },
    { 16384, 0x1cf088a2, "Drumulator, Various Hits, Tom 2" },
    { 16384, 0x9a1e1c9d, "Drumulator, Various Hits, Tom 3" },
    { 16384, 0xd4adee2a, "Drumulator, Various Hits, Tom 4" },
    { 16384, 0x8e45def8, "Drumulator, Various Hits, Tom 5" },
    { 16384, 0x53058322, "Drumulator, Various Hits, Tom 6" },
    { 16384, 0xcc4af090, "Drumulator, Various Hits, Tom 7" },
    {}
};

enum convert_e {
    CONVERT_BANK07,
    CONVERT_BANK0_FULL,
    CONVERT_BANK0_FIRST_HALF,
    CONVERT_BANK0_SECOND_HALF,
    CONVERT_BANK0_FIRST_QUARTER,
    CONVERT_BANK0_SECOND_QUARTER,
    CONVERT_BANK0_THIRD_QUARTER,
    CONVERT_BANK0_FOURTH_QUARTER,
    CONVERT_BANK0_FIRST_75P,
    CONVERT_BANK0_LAST_75P
};

static const struct convert_s {
    const enum convert_e type;
    const char *const descr;
} convlist[] = {
    { CONVERT_BANK07, "Convert Bank 0-7" },
    { CONVERT_BANK0_FULL, "Convert Bank 0 Only" },
    { CONVERT_BANK0_FIRST_HALF, "Convert Bank 0, first 50%" },
    { CONVERT_BANK0_SECOND_HALF, "Convert Bank 0, second 50%" },
    { CONVERT_BANK0_FIRST_QUARTER, "Convert Bank 0, first 25%" },
    { CONVERT_BANK0_SECOND_QUARTER, "Convert Bank 0, second 25%" },
    { CONVERT_BANK0_THIRD_QUARTER, "Convert Bank 0, third 25%" },
    { CONVERT_BANK0_FOURTH_QUARTER, "Convert Bank 0, fourth 25%" },
    { CONVERT_BANK0_FIRST_75P, "Convert Bank 0, first 75%" },
    { CONVERT_BANK0_LAST_75P, "Convert Bank 0, last 75%" },
    {}
};

struct bank_data {
    long size;
    uint8_t *data;
};

static long converted_size;
static int16_t *converted_audio;
static struct bank_data bank_data[MAX_BANKS] = {};

// ****************************************************************************
// MAIN WINDOW CLASS
//
class MainWindow : public Fl_Double_Window {
public:
    MainWindow(const char *l = nullptr);

private:
    Fl_Button *clear_buttons[MAX_BANKS];
    Fl_Input *input_fields[MAX_BANKS];
    Fl_Button *browse_buttons[MAX_BANKS];
    Fl_Box *detected_as[MAX_BANKS];
    Fl_Slider *sample_rate_slider;
    Fl_Spinner *sample_rate_spin;
    Fl_Input *save_filename;
    Fl_Choice *convtype_choice;
    Fl_Check_Button *decay_check;
    Fl_Spinner *decay_spin;
    Fl_Slider *decay_slider;

    void load_bank_data(int bank, const char *filename);

    static void HandlePlayButton_redirect(Fl_Widget *w, void *data);
    void HandlePlayButton(Fl_Widget *w, void *data);
    static void HandleClearButton_redirect(Fl_Widget *w, void *data);
    void HandleClearButton(Fl_Widget *w, void *data);
    static void HandleSaveButton_redirect(Fl_Widget *w, void *data);
    void HandleSaveButton(Fl_Widget *w, void *data);

    static void HandleBrowse_redirect(Fl_Widget *w, void *data);
    void HandleBrowse(Fl_Widget *w, void *data);
    static void HandleSaveBrowse_redirect(Fl_Widget *w, void *data);
    void HandleSaveBrowse(Fl_Widget *w, void *data);

    static void HandleInputField_redirect(Fl_Widget *w, void *data);
    void HandleInputField(Fl_Input *w, void *data);

    static void HandleDecay_redirect(Fl_Widget *w, void *data);
    void HandleDecay(Fl_Slider *w, void *data);
    static void HandleDecaySpin_redirect(Fl_Widget *w, void *data);
    void HandleDecaySpin(Fl_Spinner *w, void *data);

    static void HandleSampleRate_redirect(Fl_Widget *w, void *data);
    void HandleSampleRate(Fl_Slider *w, void *data);
    static void HandleSampleRateSpin_redirect(Fl_Widget *w, void *data);
    void HandleSampleRateSpin(Fl_Spinner *w, void *data);
};

// ****************************************************************************
// AM6070 DECODER
//
static int am6070_decode_table[128];

static void am6070_init_decode_table(void) {
    for (int i = 0; i<128; i++) {
        int x = i & 0x0f, y = i >> 4;
        am6070_decode_table[i] = (((1<<y) * ((x<<1) + 33) - 33))<<2;
    }
}

static int am6070_decode(int i) {
    int v = am6070_decode_table[i & 0x7f];
    return i >= 0x80 ? -v : v;
}

// ****************************************************************************
// CALCULATE CRC32
//
static uint32_t crc32(uint8_t *p, long size) {
    uint32_t crc=0xffffffff;

    for (long i=0; i<size; i++) {
        uint8_t c = p[i];
        for (int j=0; j<8; j++) {
            uint32_t b = (crc^c) & 1;
            crc >>= 1;
            if (b) crc ^= 0xedb88320;
            c >>= 1;
        }
    }

    return ~crc;
}

static const char *look_up_crc(uint32_t crc) {
    struct bank_info *p = bank_info;
    while (p->size) if (p->crc32 == crc) return p->name; else p++;
    return "unknown";
}

// ****************************************************************************
// LOAD NEW BANK DATA
//
static const char *load_bank(int bank, const char *filename) {
    if (bank_data[bank].data) {
        free(bank_data[bank].data);
        bank_data[bank] = {};
    }

    FILE *f = fopen(filename, "rb");
    if (!f && (!filename || (filename && !strlen(filename)))) return "";
    else if (!f) return "file not found";

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *p = (uint8_t *) malloc(size);
    if (!p) {
        fclose(f);
        return "out of memory!";
    }
    bank_data[bank].data = p;
    bank_data[bank].size = size;
    int ret = fread(p, size, 1, f);
    fclose(f);

    if (ret != 1) return "read error";

    uint32_t crc = crc32(p, size);
    const char *name = look_up_crc(crc);

    static char s[256];
    snprintf(s, 256, "Size: %ld, CRC32: %08x, %s", size, crc, name);
    return s;
}

void MainWindow::load_bank_data(int bank, const char *filename) {
    detected_as[bank]->label("");
    const char *retmsg = load_bank(bank, filename);
    detected_as[bank]->label(retmsg);
}

// ****************************************************************************
// CONVERT AUDIO / BANKS
//
static void convert_audio(uint8_t *src, int16_t *dst, int nsamples) {
    for (int i=0; i<nsamples; i++, src++, dst++) {
        *dst = am6070_decode(*src);
    }
}

static bool convert_banks(int type) {
    if (converted_audio) {
        free(converted_audio);
        converted_audio = nullptr;
        converted_size = 0;
    }

    if (!bank_data[0].data) return false;

    uint8_t *src = bank_data[0].data;
    int16_t *dst = nullptr;
    int dst_size = 0;

    switch (type) {
    case CONVERT_BANK07:
        for (int i=0; i<MAX_BANKS; i++)
            dst_size += bank_data[i].size;
        break;
    case CONVERT_BANK0_FULL:
        dst_size = bank_data[0].size;
        break;
    case CONVERT_BANK0_FIRST_HALF:
        dst_size = bank_data[0].size / 2;
        break;
    case CONVERT_BANK0_SECOND_HALF:
        dst_size = bank_data[0].size / 2;
        src += dst_size;
        break;
    case CONVERT_BANK0_FIRST_QUARTER:
        dst_size = bank_data[0].size / 4;
        break;
    case CONVERT_BANK0_SECOND_QUARTER:
        dst_size = bank_data[0].size / 4;
        src += dst_size;
        break;
    case CONVERT_BANK0_THIRD_QUARTER:
        dst_size = bank_data[0].size / 4;
        src += dst_size * 2;
        break;
    case CONVERT_BANK0_FOURTH_QUARTER:
        dst_size = bank_data[0].size / 4;
        src += dst_size * 3;
        break;
    case CONVERT_BANK0_FIRST_75P:
        dst_size = bank_data[0].size * 3 / 4;
        break;
    case CONVERT_BANK0_LAST_75P:
        dst_size = bank_data[0].size * 3 / 4;
        src += bank_data[0].size / 4;
        break;
    }

    dst = (int16_t *) malloc(dst_size * sizeof(int16_t));
    if (!dst) return false;

    converted_size = dst_size;
    converted_audio = dst;

    if (type == CONVERT_BANK07) {
        for (int i=0; i<MAX_BANKS; i++) {
            src = bank_data[i].data;
            int ssiz = bank_data[i].size;
            if (ssiz) {
                convert_audio(src, dst, ssiz);
                dst += ssiz;
            }
        }
    } else {
        convert_audio(src, dst, dst_size);
    }

    return true;
}

static void apply_decay(int ms, int rate) {
    int nsamples = (float) rate / 1000.0 * ms;
    for (int i=0; i<converted_size; i++) {
        float f = i < nsamples ? ((float) nsamples - i) / (float) nsamples : 0;
        converted_audio[i] *= f;
    }
}

// ****************************************************************************
// HANDLE PLAY BUTTON
//
void MainWindow::HandlePlayButton_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandlePlayButton(w, data);
}

static int16_t *playptr, *playend;
static bool reached_end;

static void fill_audio(void *udata, Uint8 *stream, int len) {
    if (playptr >= playend) {
        memset(stream, 0, len);
        reached_end = true;
        return;
    }
    if (len > playend - playptr) {
        memcpy(stream, playptr, playend-playptr);
        memset(stream+(playend-playptr), 0, len - (playend-playptr));
        playptr = playend;
    } else {
        memcpy(stream, playptr, len);
        playptr += len / sizeof(int16_t);
    }
}

void MainWindow::HandlePlayButton(Fl_Widget *w, void *data) {
    if (!convert_banks(convtype_choice->value())) return;

    if (decay_check->value())
        apply_decay(decay_spin->value(), sample_rate_slider->value());

    reached_end = false;
    playptr = converted_audio;
    playend = converted_audio + converted_size;

    SDL_AudioSpec wanted;
    wanted.freq = sample_rate_slider->value();
    wanted.format = AUDIO_S16;
    wanted.channels = 1;
    wanted.samples = 882;
    wanted.callback = fill_audio;
    wanted.userdata = NULL;

    if (SDL_OpenAudio(&wanted, nullptr) < 0) {
        fprintf(stderr, "error opening audio device\n");
        return;
    }

    SDL_PauseAudio(0);

    while (!reached_end) SDL_Delay(1);
    SDL_CloseAudio();
}

// ****************************************************************************
// HANDLE SAVE BUTTON
//
void MainWindow::HandleSaveButton_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleSaveButton(w, data);
}

static void setLE16(uint8_t *dst, int v) {
    *dst++ = v & 0xff;
    *dst = (v >> 8) & 0xff;
}

static void setLE32(uint8_t *dst, int v) {
    setLE16(dst, v);
    setLE16(dst+2, v>>16);
}

static bool save_wav(const char *filename, int sample_rate) {
    uint8_t header[44] = {};
    int datasize = converted_size * sizeof(int16_t);
    int bits_per_sample = 16;
    int nchannels = 1;

    FILE *f = fopen(filename, "wb");
    if (!f) goto err_out;

    memcpy(&header[0], "RIFF", 4);
    setLE32(&header[4], datasize+44-8);
    memcpy(&header[8], "WAVEfmt ", 8);
    setLE32(&header[16], 16);
    setLE16(&header[20], 1);
    setLE16(&header[22], nchannels);
    setLE32(&header[24], sample_rate);
    setLE32(&header[28], sample_rate * bits_per_sample * nchannels / 8);
    setLE16(&header[32], bits_per_sample * nchannels);
    setLE16(&header[34], bits_per_sample);
    memcpy(&header[36], "data", 4);
    setLE32(&header[40], datasize);

    if (!fwrite(header, 44, 1, f)) goto err_out;
    if (!fwrite(converted_audio, datasize, 1, f)) goto err_out;

    fclose(f);

    return true;

err_out:
    if (f) fclose(f);
    return false;
}

void MainWindow::HandleSaveButton(Fl_Widget *w, void *data) {
    if (!convert_banks(convtype_choice->value()) || !converted_size) {
        fl_choice("There is no data to save!\n", "OK", nullptr, nullptr);
        return;
    }

    if (decay_check->value())
        apply_decay(decay_spin->value(), sample_rate_slider->value());

    if (save_wav(save_filename->value(), sample_rate_spin->value()))
        fl_choice("Sample saved successfully!\n", "OK", nullptr, nullptr);
    else
        fl_choice("Error saving sample to file!\n", "OK", nullptr, nullptr);
}

// ****************************************************************************
// HANDLE SPINNERS
//
void MainWindow::HandleDecaySpin_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleDecaySpin((Fl_Spinner *)w, data);
}

void MainWindow::HandleDecaySpin(Fl_Spinner *w, void *data) {
    decay_slider->value(decay_spin->value());
}

void MainWindow::HandleSampleRateSpin_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleSampleRateSpin((Fl_Spinner *)w, data);
}

void MainWindow::HandleSampleRateSpin(Fl_Spinner *w, void *data) {
    sample_rate_slider->value(sample_rate_spin->value());
}

// ****************************************************************************
// HANDLE SLIDERS
//
void MainWindow::HandleDecay_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleDecay((Fl_Slider *)w, data);
}

void MainWindow::HandleDecay(Fl_Slider *w, void *data) {
    decay_spin->value((int)decay_slider->value());
}

void MainWindow::HandleSampleRate_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleSampleRate((Fl_Slider *)w, data);
}

void MainWindow::HandleSampleRate(Fl_Slider *w, void *data) {
    sample_rate_spin->value((int)sample_rate_slider->value());
}

// ****************************************************************************
// HANDLE NEW FILE NAME
//
void MainWindow::HandleInputField_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleInputField((Fl_Input *)w, data);
}

void MainWindow::HandleInputField(Fl_Input *w, void *data) {
    int bank;
    for (bank=0; bank<MAX_BANKS; bank++) if (w == input_fields[bank]) break;
    if (bank >= MAX_BANKS) return;      // should be impossible

    load_bank_data(bank, w->value());
}

// ****************************************************************************
// HANDLE CLEAR BUTTON
//
void MainWindow::HandleClearButton_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleClearButton(w, data);
}

void MainWindow::HandleClearButton(Fl_Widget *w, void *data) {
    int bank;
    for (bank=0; bank<MAX_BANKS; bank++) if (w == clear_buttons[bank]) break;
    if (bank >= MAX_BANKS) return;      // should be impossible

    input_fields[bank]->value("");
    input_fields[bank]->do_callback();
}

// ****************************************************************************
// HANDLE BROWSE BUTTONS
//
void MainWindow::HandleSaveBrowse_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleSaveBrowse(w, data);
}

void MainWindow::HandleSaveBrowse(Fl_Widget *w, void *data) {
    auto path = save_filename->value();
    auto newpath = fl_file_chooser("Save WAV file to...", "*.*", path);
    if (newpath) save_filename->value(newpath);
}

void MainWindow::HandleBrowse_redirect(Fl_Widget *w, void *data) {
    ((MainWindow *)data)->HandleBrowse(w, data);
}

void MainWindow::HandleBrowse(Fl_Widget *w, void *data) {
    int bank;
    for (bank=0; bank<MAX_BANKS; bank++) if (w == browse_buttons[bank]) break;
    if (bank >= MAX_BANKS) return;      // should be impossible

    auto path = input_fields[bank]->value();
    auto newpath = fl_file_chooser("Select ROM Bank File...", "*.*", path);
    if (newpath) {
        input_fields[bank]->value(newpath);
        input_fields[bank]->do_callback();
    }
}

// ****************************************************************************
// MAIN WINDOW
//
static void MainWindowCallback(Fl_Widget *w, void *data) {
    exit(0);
}

MainWindow::MainWindow(const char *l) : Fl_Double_Window(850, 500, l) {
    int cury = 16;

    auto title = new Fl_Box(w()/2-128, cury, 256, 48, "AM6070 TO WAV");
    title->labelsize(48);
    title->labelfont(FL_ITALIC | FL_BOLD);
    title->labelcolor(fl_rgb_color(0x20,0x40,0x80));
    cury += 48;

    auto copyright = new Fl_Box(w()/2-128, cury, 256, 16,
        "version " VERSION_STRING " / Copyright © 2025 by Ivo van Poorten");
    copyright->labelsize(12);
    copyright->labelfont(FL_ITALIC);
    cury += 32;

    for (int y=0; y<MAX_BANKS; y++) {
        int curx = 16;

        auto clr = new Fl_Button(curx, cury + y*24, 48, 24, "CLEAR");
        clr->labelsize(10);
        clr->callback(HandleClearButton_redirect, this);
        clear_buttons[y] = clr;
        curx += 4+48;

        char lblstr[64];
        snprintf(lblstr, 64, "Bank %d:", y);
        auto lbl = new Fl_Box(curx, cury + y*24, 80-16, 24, strdup(lblstr));
        lbl->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        lbl->labelsize(14);
        curx += 80-16;

        auto input = new Fl_Input(curx, cury + y*24, 256, 24);
        input->callback(HandleInputField_redirect, this);
        input_fields[y] = input;
        curx += 256;

        auto browse = new Fl_Button(curx+4, cury + y*24, 24, 24, "...");
        browse->callback(HandleBrowse_redirect, this);
        browse_buttons[y] = browse;
        curx += 4+24;

        auto detected = new Fl_Box(curx, cury + y*24, 600, 24);
        detected->labelsize(10);
        detected->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        detected_as[y] = detected;
    }

    cury += MAX_BANKS * 24 + 24;

    {
        int curx = 16;

        auto osrt = new Fl_Box(curx, cury, 160, 24, "Ouput Sample Rate:");
        osrt->labelsize(14);
        osrt->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        curx += 160;

        auto sr = new Fl_Slider(FL_HOR_NICE_SLIDER, curx,cury, 256,24, nullptr);
        sr->minimum(8000);
        sr->maximum(40000);
        sr->value(20833);
        sr->callback(HandleSampleRate_redirect, this);
        sample_rate_slider = sr;
        curx += 256 + 16;

        auto sp = new Fl_Spinner(curx, cury, 96, 24);
        sp->minimum(8000);
        sp->maximum(40000);
        sp->value(20833);
        sp->callback(HandleSampleRateSpin_redirect, this);
        sample_rate_spin = sp;
        curx += 96 + 16;

        auto but = new Fl_Button(curx, cury, 24, 24, "@>");
        but->callback(HandlePlayButton_redirect, this);
    }
    cury += 48;

    {
        int curx = 16;
        auto cb = new Fl_Check_Button(curx, cury, 160, 24, "Decay To Zero:");
        decay_check = cb;
        curx += 160;

        auto dl = new Fl_Slider(FL_HOR_NICE_SLIDER, curx,cury, 256,24, nullptr);
        dl->minimum(0);
        dl->maximum(500);
        dl->value(100);
        dl->callback(HandleDecay_redirect, this);
        decay_slider = dl;
        curx += 256 + 16;

        auto ds = new Fl_Spinner(curx, cury, 64, 24, "(ms)");
        ds->minimum(0);
        ds->maximum(500);
        ds->value(100);
        ds->align(FL_ALIGN_RIGHT);
        ds->callback(HandleDecaySpin_redirect, this);
        decay_spin = ds;
    }
    cury += 48;

    {
        int curx = 16;
        auto ctl = new Fl_Choice(curx, cury, 256, 24);
        for (int i=0; convlist[i].descr; i++) ctl->add(convlist[i].descr);
        ctl->value(0);
        convtype_choice = ctl;
        curx += 256 + 16;
    }
    cury += 48;

    {
        int curx = 16;
        auto txt = new Fl_Box(curx, cury, 136, 24, "Output Filename:");
        txt->labelsize(14);
        txt->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        curx += 136;

        auto sf = new Fl_Input(curx, cury, 368, 24);
        sf->value("output.wav");
        save_filename = sf;
        curx += 368+4;

        auto browse = new Fl_Button(curx, cury, 24, 24, "...");
        browse->callback(HandleSaveBrowse_redirect, this);
        curx += 4+24;

        auto save = new Fl_Button(curx, cury, 64, 24, "Save");
        save->callback(HandleSaveButton_redirect, this);
    }
    cury += 48;

    callback(MainWindowCallback, this);
}

// ****************************************************************************

static void usage(void) {
    puts("usage: am6070towav [options] input.rom [input.rom]*\n"
"\n"
"   Run without arguments to start the GUI version.\n"
"\n"
"   When using the command line version, you can specify up to eight ROMs.\n"
"\n"
"   -h              print this help message\n"
"   -o filename     output filename [default: output.wav]\n"
"   -r rate         output sample rate [default: 20833]\n"
"   -d time         decay to zero in [0-500] ms [default: off]\n"
"   -t type         type of conversion [default: all banks]"
    );
    for (int i=0; convlist[i].descr; i++) {
        printf("                       %d - %s\n", convlist[i].type,
                                                   convlist[i].descr);
    }
    puts("");
}

static const char *output_filename = "output.wav";
static int sample_rate = 20833, conversion_type, decay_ms = -1;

int main(int argc, char **argv) {
    am6070_init_decode_table();

    if (argc == 1) {
        SDL_Init(SDL_INIT_AUDIO);
        MainWindow *main_window = new MainWindow("AM6070 TO WAV");
        main_window->show();
        return Fl::run();
    }

    int option;

    while ((option = getopt(argc, argv, "ho:r:t:d:")) != -1) {
        switch (option) {
        case 'o':
            output_filename = strdup(optarg);
            break;
        case 'r':
            sample_rate = atoi(optarg);
            if (sample_rate < 8000 || sample_rate > 40000) {
                puts("error: specified sample rate out of range [8000-40000]");
                return 1;
            }
            break;
        case 't':
            conversion_type = atoi(optarg);
            if (conversion_type < CONVERT_BANK07 ||
                conversion_type > CONVERT_BANK0_LAST_75P) {
                puts("error: unknown convertsion type specified");
                return 1;
            }
            break;
        case 'd':
            decay_ms = atoi(optarg);
            if (decay_ms < 0 || decay_ms > 500) {
                puts("error: decay out of range [0-500]");
                return 1;
            }
            break;
        case ':':
        case '?':
            return 1;
        case 'h':
        default:
            usage();
            return 0;
        }
    }

    if (optind == argc) {
        puts("error: no ROM files specified");
        return 1;
    }

    for (int bank=0; bank<8 && optind<argc; bank++, optind++) {
        printf("Bank %d: ", bank);
        puts(load_bank(bank, argv[optind]));
    }

    if (!convert_banks(conversion_type)) {
        puts("error: conversion failed");
        return 1;
    }

    if (decay_ms >= 0) apply_decay(decay_ms, sample_rate);

    return save_wav(output_filename, sample_rate);
}
