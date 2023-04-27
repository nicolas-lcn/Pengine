//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>


class Texture {

private:


public:
    int width, height, nrChannels;
    unsigned char *data;

    void readTexture(char * path);

    void readPGMTexture(char imagepath[]);

};


#endif //TEXTURE_H
