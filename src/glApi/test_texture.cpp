#include "test.h"

// #include "msbsp.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLsizei texWidth = 0, texHeight = 0;
GLuint tex = 0;



// GLuint textures[TMAX];

static int load_rgb_alpha_image(const std::string &path, bool alpha)
{
    int width, height, imgComp;
    // stbi_set_flip_vertically_on_load(1);

    // req_comp has to be 3, since 'GL_RGB' is used as parameter 
    // for generating texture now, despite the original image comp.
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_rgb_alpha : STBI_rgb);
    if (!data) {
        printf("load_rgb_alpha_image(): failed to read file %s!\n\n", path.c_str());
        return 0;
    }
    if (width <= 0 || height <= 0) {
        printf("load_rgb_alpha_image(): width or height is 0!\n\n");
        return 0;
    }
    // if (imgComp != 3) {
    //     printf("load_rgb_alpha_image(): comp is: %d\n", imgComp);
    // }

    // Should have bind with the target textureId before the following operation
    glTexImage2D(GL_TEXTURE_2D, 0, 
        (alpha) ? GL_RGBA : GL_RGB, 
        width, height, 0, 
        (alpha) ? GL_RGBA : GL_RGB, 
        GL_UNSIGNED_BYTE, data);
        // unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
        // unsigned int size = ((imgWidth + 3) / 4) * ((imgHeight + 3) / 4) * blockSize; 
        // glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data); 

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    printf("  gl rgb-%s texture2d ready, w-h: %d-%d\n", (alpha) ? ("alpha") : ("non_alpha"), width, height);
    printf("  path: %s\n\n", path.c_str());
    return 1;
}

static int load_grayscale_alpha_image(const std::string &path, bool alpha)
{
    int width, height, imgComp;
    // stbi_set_flip_vertically_on_load(1);

    // How to load 1/2 channels image properly, instead of using RGBA format ???
    // unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_grey_alpha : STBI_grey);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, (alpha) ? STBI_rgb_alpha : STBI_rgb);
    
    if (!data) {
        printf("load_grayscale_alpha_image(): failed to read file %s!\n\n", path.c_str());
        return 0;
    }
    if (width <= 0 || height <= 0) {
        printf("load_grayscale_alpha_image(): width or height is 0!\n\n");
        return 0;
    }

    // How to load 1/2 channels image properly, instead of using RGBA format ???
    //
    // Should have bind with the target textureId before the following operation
    // glTexImage2D(GL_TEXTURE_2D, 0, 
    //     (alpha) ? GL_RG16 : GL_RED, 
    //     width, height, 0, 
    //     (alpha) ? GL_RG16 : GL_RED, 
    //     GL_UNSIGNED_BYTE, data);
    // if (alpha) {
    //     GLint swizzleMask[] = {GL_GREEN, GL_GREEN, GL_GREEN, GL_RED};
    //     glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    // }
    // else {
    //     GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
    //     glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    // }
        glTexImage2D(GL_TEXTURE_2D, 0, 
            (alpha) ? GL_RGBA : GL_RGB, 
            width, height, 0, 
            (alpha) ? GL_RGBA : GL_RGB, 
            GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_image_free(data);
    printf("  gl grayscale-%s texture2d ready, w-h: %d-%d\n", (alpha) ? ("alpha") : ("non_alpha"), width, height);
    return 1;
}

int createTex(unsigned int num, unsigned int *texIds, std::string *imgPaths) {

    glGenTextures(num, texIds);

    for (unsigned int i = 0; i < num; i++) {
        if (!texIds[i]) {
            printf("  createTex(): texture[%d] is 0 !\n", i);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, texIds[i]);

        int x, y, comp = 0;
        if ( !stbi_info(imgPaths[i].c_str(), &x, &y, &comp) ) {
            printf("  createTex(): failed to read tex file: %s\n", imgPaths[i].c_str());
            return 0;
        }

        if (comp == 3) {
            if (!load_rgb_alpha_image(imgPaths[i], 0)) {
                return 0;
            }
        }
        else if (comp == 4) {
            // if (!load_rgb_alpha_image(imgPaths[i], 1)) {
            if (!load_rgb_alpha_image(imgPaths[i], 0)) {
                return 0;
            }
        }
        else if (comp == 2) {
            // grayscale-alpha character map for texts
            if (!load_grayscale_alpha_image(imgPaths[i], 1)) {
                return 0;
            }
        }
        else {
            printf("  createTex(): tex file:%s not rgb/rgba/text-char-map format, comp is:%d\n", imgPaths[i].c_str(), comp);
            return 0;
        }
    }

    return 1;
}

void destroyTex(unsigned int num, unsigned int *texIds) {
    glDeleteTextures(num, texIds);
}

void bindTex(GLenum textureUnit, unsigned int texId) {
    // if (textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31 && tex < TMAX) {
    if (textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31) {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, texId);
    }
    else {
        printf("bindTex() failed.\n");
    }
}

// void bindTex(GLenum textureUnit, texEnum tex) {
//     if ( textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31 && tex != TMAX ) {
//         glActiveTexture(textureUnit);
//         glBindTexture(GL_TEXTURE_2D, textures[tex]);
//     }
//     else {
//         printf("bindTex() failed.\n");
//     }
// }

// int load_resources() {
//     std::string paths[TMAX] = {
//         "./data/tex/konami.png",
//         "./data/tex/00.png",
//         "./data/tex/01.png",
//         "./data/tex/02.png",
//         "./data/tex/03.png"
//     };

//     glGenTextures(TMAX, textures);

//     for (unsigned int i = T00; i < TMAX; i++) {
//         glBindTexture(GL_TEXTURE_2D, textures[i]);
//         if (!load_rgb_alpha_image(paths[i])) {
//             return 0;
//         }
//     }
//     return 1;
// }

// void unload_resources() {
//     glDeleteTextures(TMAX, textures);
// }



int initTexture(const std::string &path)
{
    int width, height, imgComp;
    // stbi_set_flip_vertically_on_load(1);

    // req_comp has to be 3, since 'GL_RGB' is used as parameter 
    // for generating texture now, despite the original image comp.
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &imgComp, 3);
    if (!data) {
        printf("loadTexture(): failed to read file %s!\n\n", path.c_str());
        return 0;
    }

    if (imgComp != 3) {
        // printf("loadTexture(): comp is not 3, it has to be 3 !");
        // return 0;

        printf("loadTexture(): comp is: %d\n", imgComp);
    }

    if (width <= 0 || height <= 0) {
        printf("loadTexture(): width or height not correct !\n\n");
        return 0;
    }

    texWidth = width;
    texHeight = height;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
        // unsigned int size = ((imgWidth + 3) / 4) * ((imgHeight + 3) / 4) * blockSize; 
        // glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    printf("  gl texture2d generated, id, w-h: \'%d, %d-%d\'\n", tex, width, height);
    return 1;

    /**
     * They're actually mirrored too! The co-ordinate systems are COMPLETELY different,
     * OpenGL is bottom left, bmp is (more pracitically), top left. We should probably pass in
     * some kind of shader attribute (via a boolean) so that we can swap the <s, t> co-ords.
     */
     //std::reverse(data.begin(), data.end()); // Reverse the data because .bmp files are actually upside down in RAM

     // Do the actual texture upload
    // glGenTextures(1, &tex);
    // glBindTexture(GL_TEXTURE_2D, tex);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for the texture
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glBindTexture(GL_TEXTURE_2D, 0); // Un-bind this texture.
}

void deinitTexture() {
    glDeleteTextures(1, &tex);
    printf("  gl texture2d: %d deleted.\n", tex);

    tex = 0; texWidth = 0; texHeight = 0;
}

void bindTex(GLenum textureUnit)
{
    if ( textureUnit >= GL_TEXTURE0 && textureUnit <= GL_TEXTURE31 ) {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
}

void unbindTex()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void textureTest(const std::string &texName) {
    std::string texPath = "./data/tex/" + texName;
    if (!initTexture(texPath)) {
        return;
    }

    bindTex(GL_TEXTURE0);
    unbindTex();

    deinitTexture();
}

int texTest_Init(const std::string &texName) {
    std::string texPath = "./data/tex/" + texName;
    if (!initTexture(texPath)) {
        return 0;
    }

    return 1;
}

void texTest_Deinit() {
    deinitTexture();
}

    /*
    using namespace sh3::graphics::bmp;

    std::ifstream   file;
    file_header     fheader;    // File information header
    info_header     iheader;
    std::uint16_t   bpp;        // Bitmap bit depth

    std::vector<palette_color> palette;     // Vector containing colour data for this image
    std::vector<std::uint8_t>  data;        // Vector containing image data or palette offsets

    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        printf("loadTexture(): Unable to open file %s!", path.c_str());
        return 0;
    }
    file.read(reinterpret_cast<char*>(&fheader), sizeof(fheader)); // Read in the file header
    if (fheader.tag[0] != 'B' || fheader.tag[1] != 'M') {
        printf("loadTexture(): file does not appear to be a BMP! Reason: BAD_HEADER!");
        return 0;
    }

    file.read(reinterpret_cast<char*>(&iheader), sizeof(iheader)); // Read in the information header
    unsigned short width, height;
    // ASSERT(iheader.width >= 0);
    // width = static_cast<GLsizei>(iheader.width);
    // ASSERT(iheader.height >= 0);
    // height = static_cast<GLsizei>(iheader.height);
    if (iheader.width >= 0 && iheader.height >= 0) {
        width = static_cast<GLsizei>(iheader.width);
        height = static_cast<GLsizei>(iheader.height);
    }
    else {
        printf("loadTexture(): width or height from header is not correct! < 0!");
    }
    bpp = iheader.bpp;

    data.resize(static_cast<std::size_t>(width * height) * 3u); // Some programs misformat this, so calculate it ourselves. This is usually 24-bit even for 8-bit paletted images!

    // We can now differentiate if our bitmap is 8bpp or 24bpp
    if (bpp == 8) // Paletted images are for fuckwits, I'm looking at you, Konami....
    {
        // palette.resize(iheader.palette_size);
        // file.seekg(sizeof(fheader) + sizeof(iheader), std::ios_base::beg); // Seek to the palette
        // file.read(reinterpret_cast<char*>(palette.data()), iheader.palette_size);
        // file.seekg(fheader.pix_offset, std::ios_base::beg); // Ensure we are actually in the data section

        // // Slightly expensive, but we now copy each pixel into the data buffer by getting it's index in the palette.
        // // Seeing as this is done once, I doubt it matters too much, especially if we do this during load time
        // // and keep the image alive for the whole of the scene duration (there are 3 bmps total in SH3, not including
        // // our default 'error' texture)
        // for(pixel& p: data)
        // {
        //     palette_color color;    // 4-byte colour, stored in BGRA (where A is unused)
        //     std::uint8_t index;     // Image byte, which is the index into the colour palette
        //     file.read(reinterpret_cast<char*>(&index), sizeof(index));

        //     color = palette[index];
        //     p.r = color.r; // Extract Red
        //     p.g = color.g; // Extract Green
        //     p.b = color.b; // Extract Blue
        // }
        Log(LogLevel::WARN, "CTexture::Load() does not support 8-bit MS bitmaps! Consider revising!");
        return;
    }
    if (bpp == 24)
    {
        file.seekg(fheader.pix_offset, std::ios_base::beg); // Seek to the image data
        ASSERT(data.size() <= std::numeric_limits<std::streamsize>::max());
        //file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size())); // Read in the image data
        file.read(reinterpret_cast<char*>(data.data()), data.size());
    }
    else
    {
        Log(LogLevel::WARN, "msbmp::Load( ): Bad pixel depth %d!", bpp);
        return;
    }
    //*/
