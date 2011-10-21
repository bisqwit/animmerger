#include "presets.hh"

const decltype(presets) presets =
{
    { "c64palette",
        {
            "VIC-II (Commodore 64) palette only",
                "Chooses the VIC-II 16-color palette.",
            { "-Q000000,FFFFFF,894036,7ABFC7,8A46AE,68A941,3E31A2,D0DC71"
               ",905F25,5C4700,BB776D,555555,808080,ACEA88,7C70DA,ABABAB"
            }
        }
    },
    { "monopalette",
        {
            "Choose 2-color monochrome palette",
                "Chooses a black-white monochrome palette.",
            { "-Q000000,FFFFFF" }
        }
    },
    { "egapalette",
        {
            "Choose 16-color EGA palette",
                "Chooses the 16-color standard CGA/EGA/VGA palette.",
            { "-Q000000,0000AA,00AA00,00AAAA,AA0000,AA00AA,AA5500,555555"
               ",AAAAAA,5555FF,55FF55,55FFFF,FF5555,FF55FF,FFFF55,FFFFFF" }
        }
    },
    { "bbcpalette",
        {
            "Choose 8-color BBC-Micro palette",
                "Chooses a 8-color palette comprised of slightly desaturated RGB extremes.",
            { "-Q121212,2323FA,23FA23,23FAFA,FA2323,FA23FA,FAFA23,FAFAFA" }
        }
    },
    { "fastdither",
        {
            "Fast ordered dithering",
                "",
            { "--dm=8x8", "--dc=16", "-Dky", "--dr=1" },
        }
    },
    { "dither",
        {
            "Medium ordered dithering",
                "",
            { "--dm=8x8", "--dc=16", "-Dy2", "--dr=2" },
        }
    },
    { "lowdither",
        {
            "Low ordered dithering",
                "",
            { "--dm=32x32", "--dc=3", "-Dy2", "--dr=3,2,-2" },
        }
    },
    { "slowdither",
        {
            "Slow ordered dithering",
                "",
            { "--dm=8x8", "--dc=32", "-Dy2", "--dr=3,4,-2" },
        }
    },

    { "msx",
/*
    // This palette taken from JSMSX by Marcus Granado,
    // multiplied by FF/E0 to saturate the entire RGB range
        0x000000, //YRBcolor(0.00, 0.47, 0.47), // black
        0x24DB24, //YRBcolor(0.53, 0.07, 0.20), // medium green
        0x6DFF6D, //YRBcolor(0.67, 0.17, 0.27), // light green
        0x2424FF, //YRBcolor(0.40, 0.40, 1.00), // dark blue
        0x496DFF, //YRBcolor(0.53, 0.43, 0.93), // light blue
        0xB62424, //YRBcolor(0.47, 0.83, 0.30), // dark red
        0x49DBFF, //YRBcolor(0.73, 0.00, 0.70), // cyan
        0xFF2424, //YRBcolor(0.53, 0.93, 0.27), // medium red
        0xFF6D6D, //YRBcolor(0.67, 0.93, 0.27), // light red
        0xDBDB24, //YRBcolor(0.73, 0.57, 0.07), // dark yellow
        0xDBDB92, //YRBcolor(0.80, 0.57, 0.17), // light yellow
        0x249224, //YRBcolor(0.47, 0.13, 0.23), // dark green
        0xDB49B6, //YRBcolor(0.53, 0.73, 0.67), // magenta
        0xB6B6B6, //YRBcolor(0.80, 0.47, 0.47), // gray
        0xFFFFFF);//YRBcolor(1.00, 0.47, 0.47));// white
*/
        {
            "TMS9918 (MSX, ColecoVision, TI-99/4A etc.) style rendering",
                "Fixed 15-color palette.\n"
                "For each 8x1 section, 2-color selection from the palette.\n"
                "Recommended resolution for authenticity: 256x160",
            { "--sections=8x1=2",
              "-Q000000,24DB24,6DFF6D,2424FF,496DFF,B62424,49DBFF,FF2424"
               ",FF6D6D,DBDB24,DBDB92,249224,DB49B6,B6B6B6,FFFFFF"
            }
        }
    },
    { "c64",
        {
            "VIC-II (Commodore 64) style rendering",
                "Fixed 16-color palette.\n"
                "For each 8x4 section, 3-color selection from the palette,\n"
                "plus background color that changes once per screen.\n"
                "A 72x32 border will be automatically added.\n"
                "Recommended resolution for authenticity: 160x200",
            { "--preset=c64palette",
              "--sections=WxH=1,8x4=3",
              "--padding=36,36,18,14" // top,bottom,left,right
            }
        }
    },
    { "bbc1",
        {
            "BBC-Micro 1-bpp (2-color) mode",
                "Fixed 8-color palette.\n"
                "For entire screen, 2-color selection from the palette.\n"
                "Recommended resolution for authenticity: 640x256",
            { "--preset=bbcpalette",
              "--sections=WxH=2" }
        }
    },
    { "bbc2",
        {
            "BBC-Micro 2-bpp (4-color) mode",
                "Fixed 8-color palette.\n"
                "For entire screen, 4-color selection from the palette.\n"
                "Recommended resolution for authenticity: 320x256",
            { "--preset=bbcpalette",
              "--sections=WxH=4" }
        }
    },
    { "bbc3",
        {
            "BBC-Micro 3-bpp (8-color) mode",
                "Fixed 8-color palette.\n"
                "Recommended resolution for authenticity: 160x256",
            { "--preset=bbcpalette"
            }
        }
    },
    { "nes",
        {
            "Nintendo Entertainment System mode",
                "Fixed 53-color palette.\n"
                "For each 16x16 section, arbitrary 3-color selection from palette.\n"
                "However, for every whole-screen, there's a maximum of four distinct\n"
                "3-color selections. Plus background color that changes once per screen.\n"
                "Recommended resolution for authenticity: 256x240 or 256x224",
            { "-Q000000",
              "--sections=WxH=1,16x16=3@WxH=4"
              // TODO: Color emphasis bits
            }
        }
    },
    { "cga16",
/*
    // composite cga (to be rendered on 640x200 monochrome mode)
    CurrentPalette.SetHardcoded(15,
        0x000000, // 0000
        0x007A00, // 0001
        0x1B33DF, // 0010
        0x01ADDE, // 0011
        0x990580, // 0100
        0x7F7F7F, // 0101, 1010
        0xB538FF, // 0110
        0x9AB2FF, // 0111
        0x644C00, // 1000
        0x49C600, // 1001
        0x65F97E, // 1011
        0xFD5120, // 1100
        0xE3CB1F, // 1101
        0xFF84FF, // 1110
        0xFFFFFF); // 1111
*/
        {
            "Special 16-color CGA mode",
                "Recommended resolution for authenticity: 160x200",
            { "--sections=cga",
              "-Q000000,007A00,1B33DF,01ADDE,990580,7F7F7F,B538FF,9AB2FF"
               ",644C00,49C600,65F97E,FD5120,E3CB1F,FF84FF,FFFFFF"
            }
        }
    }
};
