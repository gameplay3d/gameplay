#include "Base.h"
#include "TTFFontEncoder.h"
#include "GPBFile.h"
#include "StringUtil.h"

namespace gameplay
{

static void drawBitmap(unsigned char* dstBitmap, int x, int y, int dstWidth, unsigned char* srcBitmap, int srcWidth, int srcHeight)
{
    // offset dst bitmap by x,y.
    dstBitmap +=  (x + (y * dstWidth));

    for (int i = 0; i < srcHeight; ++i)
    {
        memcpy(dstBitmap, (const void*)srcBitmap, srcWidth);
        srcBitmap += srcWidth;
        dstBitmap += dstWidth;
    }
}

static void writeUint(FILE* fp, unsigned int i)
{
    fwrite(&i, sizeof(unsigned int), 1, fp);
}

static void writeString(FILE* fp, const char* str)
{
    unsigned int len = strlen(str);
    fwrite(&len, sizeof(unsigned int), 1, fp);
    if (len > 0)
    {
        fwrite(str, 1, len, fp);
    }
}

int writeFont(const char* inFilePath, const char* outFilePath, unsigned int fontSize, const char* id, bool fontpreview = false)
{
    Glyph glyphArray[END_INDEX - START_INDEX];
    
    // Initialize freetype library.
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        LOG(1, "FT_Init_FreeType error: %d \n", error);
        return -1;
    }
    
    // Initialize font face.
    FT_Face face;
    error = FT_New_Face(library, inFilePath, 0, &face);
    if (error)
    {
        LOG(1, "FT_New_Face error: %d \n", error);
        return -1;
    }
    
    // Set the pixel size.
    error = FT_Set_Char_Size(
            face,           // handle to face object.
            0,              // char_width in 1/64th of points.
            fontSize * 64,   // char_height in 1/64th of points.
            0,              // horizontal device resolution (defaults to 72 dpi if resolution (0, 0)).
            0 );            // vertical device resolution.
    
    if (error)
    {
        LOG(1, "FT_Set_Char_Size error: %d \n", error);
        return -1;
    }

    /* 
    error = FT_Set_Pixel_Sizes(face, FONT_SIZE, 0);
    if (error)
    {
        LOG(1, "FT_Set_Pixel_Sizes error : %d \n", error);
        exit(1);
    }
    */

    // Save glyph information (slot contains the actual glyph bitmap).
    FT_GlyphSlot slot = face->glyph;
    
    int actualfontHeight = 0;
    int rowSize = 0; // Stores the total number of rows required to all glyphs.
    
    // Find the width of the image.
    for (unsigned char ascii = START_INDEX; ascii < END_INDEX; ++ascii)
    {
        // Load glyph image into the slot (erase previous one)
        error = FT_Load_Char(face, ascii, FT_LOAD_RENDER);
        if (error)
        {
            LOG(1, "FT_Load_Char error : %d \n", error);
        }
        
        int bitmapRows = slot->bitmap.rows;
        actualfontHeight = (actualfontHeight < bitmapRows) ? bitmapRows : actualfontHeight;
        
        if (slot->bitmap.rows > slot->bitmap_top)
        {
            bitmapRows += (slot->bitmap.rows - slot->bitmap_top);
        }
        rowSize = (rowSize < bitmapRows) ? bitmapRows : rowSize;
    }

    // Include padding in the rowSize.
    rowSize += GLYPH_PADDING;
    
    // Initialize with padding.
    int penX = 0;
    int penY = 0;
    int row = 0;
    
    double powerOf2 = 2;
    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    bool textureSizeFound = false;

    int advance;
    int i;

    while (textureSizeFound == false)
    {
        imageWidth =  (unsigned int)pow(2.0, powerOf2);
        imageHeight = (unsigned int)pow(2.0, powerOf2);
        penX = 0;
        penY = 0;
        row = 0;

        // Find out the squared texture size that would fit all the require font glyphs.
        i = 0;
        for (unsigned char ascii = START_INDEX; ascii < END_INDEX; ++ascii)
        {
            // Load glyph image into the slot (erase the previous one).
            error = FT_Load_Char(face, ascii, FT_LOAD_RENDER);
            if (error)
            {
                LOG(1, "FT_Load_Char error : %d \n", error);
            }

            // Glyph image.
            int glyphWidth = slot->bitmap.pitch;
            int glyphHeight = slot->bitmap.rows;

            advance = glyphWidth + GLYPH_PADDING; //((int)slot->advance.x >> 6) + GLYPH_PADDING;

            // If we reach the end of the image wrap aroud to the next row.
            if ((penX + advance) > (int)imageWidth)
            {
                penX = 0;
                row += 1;
                penY = row * rowSize;
                if (penY + rowSize > (int)imageHeight)
                {
                    powerOf2++;
                    break;
                }
            }

            // penY should include the glyph offsets.
            penY += (actualfontHeight - glyphHeight) + (glyphHeight - slot->bitmap_top);

            // Set the pen position for the next glyph
            penX += advance; // Move X to next glyph position
            // Move Y back to the top of the row.
            penY = row * rowSize;

            if (ascii == (END_INDEX-1))
            {
                textureSizeFound = true;
            }

            i++;
        }
    }

    // Try further to find a tighter texture size.
    powerOf2 = 1;
    for (;;)
    {
        if ((penY + rowSize) >= pow(2.0, powerOf2))
        {
            powerOf2++;
        }
        else
        {
            imageHeight = (int)pow(2.0, powerOf2);
            break;
        }
    }
    
    // Allocate temporary image buffer to draw the glyphs into.
    unsigned char* imageBuffer = (unsigned char *)malloc(imageWidth * imageHeight);
    memset(imageBuffer, 0, imageWidth * imageHeight);
    penX = 0;
    penY = 0;
    row = 0;
    i = 0;
    for (unsigned char ascii = START_INDEX; ascii < END_INDEX; ++ascii)
    {
        // Load glyph image into the slot (erase the previous one).
        error = FT_Load_Char(face, ascii, FT_LOAD_RENDER);
        if (error)
        {
            LOG(1, "FT_Load_Char error : %d \n", error);
        }

        // Glyph image.
        unsigned char* glyphBuffer =  slot->bitmap.buffer;
        int glyphWidth = slot->bitmap.pitch;
        int glyphHeight = slot->bitmap.rows;

        advance = glyphWidth + GLYPH_PADDING;//((int)slot->advance.x >> 6) + GLYPH_PADDING;

        // If we reach the end of the image wrap aroud to the next row.
        if ((penX + advance) > (int)imageWidth)
        {
            penX = 0;
            row += 1;
            penY = row * rowSize;
            if (penY + rowSize > (int)imageHeight)
            {
                free(imageBuffer);
                LOG(1, "Image size exceeded!");
                return -1;
            }
        }
        
        // penY should include the glyph offsets.
        penY += (actualfontHeight - glyphHeight) + (glyphHeight - slot->bitmap_top);

        // Draw the glyph to the bitmap with a one pixel padding.
        drawBitmap(imageBuffer, penX, penY, imageWidth, glyphBuffer, glyphWidth, glyphHeight);
        
        // Move Y back to the top of the row.
        penY = row * rowSize;

        glyphArray[i].index = ascii;
        glyphArray[i].width = advance - GLYPH_PADDING;
        
        // Generate UV coords.
        glyphArray[i].uvCoords[0] = (float)penX / (float)imageWidth;
        glyphArray[i].uvCoords[1] = (float)penY / (float)imageHeight;
        glyphArray[i].uvCoords[2] = (float)(penX + advance - GLYPH_PADDING) / (float)imageWidth;
        glyphArray[i].uvCoords[3] = (float)(penY + rowSize) / (float)imageHeight;

        // Set the pen position for the next glyph
        penX += advance; // Move X to next glyph position
        i++;
    }


    FILE *gpbFp = fopen(outFilePath, "wb");
    
    // File header and version.
    char fileHeader[9]     = {'«', 'G', 'P', 'B', '»', '\r', '\n', '\x1A', '\n'};
    fwrite(fileHeader, sizeof(char), 9, gpbFp);
    fwrite(gameplay::GPB_VERSION, sizeof(char), 2, gpbFp);

    // Write Ref table (for a single font)
    writeUint(gpbFp, 1);                // Ref[] count
    writeString(gpbFp, id);             // Ref id
    writeUint(gpbFp, 128);              // Ref type
    writeUint(gpbFp, ftell(gpbFp) + 4); // Ref offset (current pos + 4 bytes)
    
    // Write Font object.
    
    // Family name.
    writeString(gpbFp, face->family_name);

    // Style.
    // TODO: Switch based on TTF style name and write appropriate font style unsigned int
    // For now just hardcoding to 0.
    //char* style = face->style_name;
    writeUint(gpbFp, 0); // 0 == PLAIN

    // Font size.
    writeUint(gpbFp, rowSize);

    // Character set.
    // TODO: Empty for now
    writeString(gpbFp, "");
    
    // Glyphs.
    unsigned int glyphSetSize = END_INDEX - START_INDEX;
    writeUint(gpbFp, glyphSetSize);
    fwrite(&glyphArray, sizeof(Glyph), glyphSetSize, gpbFp);
    
    // Texture.
    unsigned int textureSize = imageWidth * imageHeight;
    writeUint(gpbFp, imageWidth);
    writeUint(gpbFp, imageHeight);
    writeUint(gpbFp, textureSize);
    fwrite(imageBuffer, sizeof(unsigned char), textureSize, gpbFp);
    
    // Close file.
    fclose(gpbFp);

    LOG(1, "%s.gpb created successfully. \n", getBaseName(outFilePath).c_str());

    if (fontpreview)
    {
        // Write out font map to an image.
        std::string pgmFilePath = getFilenameNoExt(outFilePath);
        pgmFilePath.append(".pgm");
        FILE *imageFp = fopen(pgmFilePath.c_str(), "wb");
        fprintf(imageFp, "P5 %u %u 255\n", imageWidth, imageHeight);
        fwrite((const char *)imageBuffer, sizeof(unsigned char), imageWidth * imageHeight, imageFp);
        fclose(imageFp);
    }

    // Cleanup resources.
    free(imageBuffer);
    
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return 0;
}

}
