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
            { "-Q000000,1010FA,10FA10,10FAFA,FA1010,FA10FA,FAFA40,FAFAFA" }
        }
    },
    { "fastdither",
        {
            "Fast ordered dithering",
                "",
            { "--dm 8x8", "--dc 16", "-Dky", "--dr 1" },
        }
    },
    { "dither",
        {
            "Medium ordered dithering",
                "",
            { "--dm 8x8", "--dc 16", "-Dy2", "--dr 2" },
        }
    },
    { "lowdither",
        {
            "Low ordered dithering",
                "",
            { "--dm 32x32", "--dc 3", "-Dy2", "--dr 3,2,-2" },
        }
    },
    { "slowdither",
        {
            "Slow ordered dithering",
                "",
            { "--dm 8x8", "--dc 32", "-Dy2", "--dr 3,4,-2" },
        }
    }
};
