// Created by: Jialan Dong
// Cursor AI assistant used
/*
Method referenced: 
    https://github.com/bobowitz/15666-game1/blob/master/
    https://github.com/xinyis991105/15-466-f20-base1/blob/master/
    https://github.com/Chipxiang/Jump-Guy/blob/master/

*/ 

#include "Load.hpp"
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "PPU466.hpp"
#include "data_path.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

void load_assets() {
    std::vector<PPU466::Tile> tiles;
    std::vector<PPU466::Palette> palettes;

    std::ifstream fileList;
    // std::cout << "exe_path = " << data_path("./") << "\n";
    fileList.open(data_path("../assets/asset_list.txt")); // open asset list file
    if (!fileList.is_open()) { // fail to open asset list file
        std::cerr << "Error: Failed to open asset list file" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(fileList, line)) {

        std::string filePath = data_path("../assets/" + line + ".png");

        std::cout << "Processing PNG file: " << filePath << std::endl;
        glm::uvec2 size;
        std::vector<glm::u8vec4> data;
        load_png(filePath, &size, &data, LowerLeftOrigin);

        //Referenced https://github.com/xinyis991105/15-466-f20-base1/blob/master/
        int w = size.x / 8;
        int h = size.y / 8;

        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) { // for each tile
                
                PPU466::Tile tile = { {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} };
                PPU466::Palette palette;
                int colorCount = 0;

                for(int a = 0; a < 8; a++) { //creating palette
                    for(int b = 0; b < 8; b++) { // for each pixel in the tile
                        int index = (j * 8 + b) * w * 8 + i * 8 + a;
                        glm::u8vec4 color = data[index];
                        
                        int colorIndex = -1;
                        for(int c = 0; c< colorCount; c++) {
                            if(color == palette[c]) { //color exist in palette
                                colorIndex = c;
                                break;
                            }
                        }

                        if(colorIndex == -1) { //color doesn't exist in palette, add new
                            palette[colorCount] = color;
                            colorIndex = colorCount;
                            colorCount++;
                        }

                        tile.bit0[b] = tile.bit0[b] | ((colorIndex & 1) << a);
                        tile.bit1[b] = tile.bit1[b] | ((colorIndex>>1 & 1) << a);
                    }
                }

                tiles.push_back(tile);
                palettes.push_back(palette);
            }
        }
    }

    fileList.close();
    std::ofstream palettes_stream;
	std::ofstream tiles_stream;
	palettes_stream.open(data_path("../assets/palettes.asset"));
	tiles_stream.open(data_path("../assets/tiles.asset"));
	write_chunk(std::string("pale"), palettes, &palettes_stream);
	write_chunk(std::string("tile"), tiles, &tiles_stream);
	palettes_stream.close();
	tiles_stream.close();
    
    std::cout << "Assets Loading Done. " << tiles.size() << " Tiles And " 
              << palettes.size() << " Palettes Loaded" << std::endl;
}

Load<void> asset_loader(LoadTagEarly, load_assets);