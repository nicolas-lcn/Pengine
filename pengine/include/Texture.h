//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_TEXTURE_H
#define TP1_CODE_TEXTURE_H

#include "image_ppm.h"
#include <iostream>


class Texture {

private:


public:
    int width, height, nrChannels;
    unsigned char *data;

    void readTexture(char * path){
        data = stbi_load(path, &width , &height , &nrChannels , 0);

        if (data) {} else { std::cout << "Failed to load texture" << std::endl; }
        stbi_image_free(data);
    }

    void readPGMTexture(char imagepath[]) {
        lire_nb_lignes_colonnes_image_pgm(imagepath, &width, &height);
        allocation_tableau(data, OCTET, width*height);
        lire_image_pgm(imagepath, data, width*height);

        /*for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++) {
                std::cout << (int) data[i*width + j] << std::endl;
            }
        }*/
    }

};


#endif //TP1_CODE_TEXTURE_H
