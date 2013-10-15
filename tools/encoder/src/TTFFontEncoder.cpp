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

unsigned char* createDistanceFields(unsigned char* img, unsigned int width, unsigned int height)
{
    short* xDistance = (short*)malloc(width * height * sizeof(short));
    short* yDistance = (short*)malloc(width * height * sizeof(short));
    double* gx = (double*)calloc(width * height, sizeof(double));
    double* gy = (double*)calloc(width * height, sizeof(double));
    double* data = (double*)calloc(width * height, sizeof(double));
    double* outside = (double*)calloc(width * height, sizeof(double));
    double* inside = (double*)calloc(width * height, sizeof(double));
    unsigned int i;

    // Convert img into double (data)
    double imgMin = 255;
    double imgMax = -255;
    for (i = 0; i < width * height; ++i)
    {
        double v = img[i];
        data[i] = v;
        if (v > imgMax) 
            imgMax = v;
        if (v < imgMin) 
            imgMin = v;
    }
    // Rescale image levels between 0 and 1
    for (i = 0; i < width * height; ++i)
    {
        data[i] = (img[i] - imgMin) / imgMax;
    }
    // Compute outside = edtaa3(bitmap); % Transform background (0's)
    computegradient(data, width, height, gx, gy);
    edtaa3(data, gx, gy, height, width, xDistance, yDistance, outside);
    for (i = 0; i < width * height; ++i)
    {
        if (outside[i] < 0 )
            outside[i] = 0.0;
    }
    // Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
    memset(gx, 0, sizeof(double) * width * height);
    memset(gy, 0, sizeof(double) * width * height);
    for (i = 0; i < width * height; ++i)
    {
        data[i] = 1 - data[i];
    }
    computegradient(data, width, height, gx, gy);
    edtaa3(data, gx, gy, height, width, xDistance, yDistance, inside);
    for (i = 0; i < width * height; ++i)
    {
        if( inside[i] < 0 )
            inside[i] = 0.0;
    }
    // distmap = outside - inside; % Bipolar distance field
    unsigned char* out = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    for (i = 0; i < width * height; ++i)
    {
        outside[i] -= inside[i];
        outside[i] = 128 + outside[i] * 16;
        if (outside[i] < 0) 
            outside[i] = 0;
        if (outside[i] > 255) 
            outside[i] = 255;
        out[i] = 255 - (unsigned char) outside[i];
    }
    free(xDistance);
    free(yDistance);
    free(gx);
    free(gy);
    free(data);
    free(outside);
    free(inside);

    return out;
}

int writeFont(const char* inFilePath, const char* outFilePath, unsigned int fontSize, const char* id, bool fontpreview = false, Font::FontFormat fontFormat = Font::BITMAP)
{
    TTFGlyph glyphArray[END_INDEX - START_INDEX];
    
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
    
    // Set the pixel size.  char_width in 1/64th of points.
    error = FT_Set_Char_Size(  face,  0, fontSize * 64, 0, 0 ); 
    
    if (error)
    {
        LOG(1, "FT_Set_Char_Size error: %d \n", error);
        return -1;
    }

    // Save glyph information (slot contains the actual glyph bitmap).
    FT_GlyphSlot slot = face->glyph;
    int actualfontHeight = 0;
    // Stores the total number of rows required to all glyphs.
    int rowSize = 0;
    
    // Find the width of the image.
    for (unsigned char ascii = START_INDEX; ascii < END_INDEX; ++ascii)
    {
        // Load glyph image into the slot (erase previous one)
        FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;
        error = FT_Load_Char(face, ascii, loadFlags);
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

            advance = glyphWidth + GLYPH_PADDING; 

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

            if (ascii == (END_INDEX - 1))
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
    unsigned char* imageBuffer = (unsigned char*)malloc(imageWidth * imageHeight);
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

        advance = glyphWidth + GLYPH_PADDING;

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
        penX += advance;
        i++;
    }
    
    // File header and version.
    FILE *gpbFp = fopen(outFilePath, "wb");    
    char fileHeader[9]     = {'\xAB', 'G', 'P', 'B', '\xBB', '\r', '\n', '\x1A', '\n'};
    fwrite(fileHeader, sizeof(char), 9, gpbFp);
    fwrite(gameplay::GPB_VERSION, sizeof(char), 2, gpbFp);

    // Write Ref table (for a single font)
    writeUint(gpbFp, 1);                // Ref[] count
    writeString(gpbFp, id);             // Ref id
    writeUint(gpbFp, 128);              // Ref type
    writeUint(gpbFp, ftell(gpbFp) + 4); // Ref offset (current pos + 4 bytes)

    // Family name.
    writeString(gpbFp, face->family_name);

    // Style.
    // TODO: Switch based on TTF style name and write appropriate font style unsigned int for now just hardcoding to 0 = PLAIN.
    writeUint(gpbFp, 0);

    // Font size.
    writeUint(gpbFp, rowSize);

    // Character set. TODO: Empty for now
    writeString(gpbFp, "");
    
    // Glyphs.
    unsigned int glyphSetSize = END_INDEX - START_INDEX;
    writeUint(gpbFp, glyphSetSize);
    fwrite(&glyphArray, sizeof(TTFGlyph), glyphSetSize, gpbFp);
    
    // Image dimensions
    unsigned int imageSize = imageWidth * imageHeight;
    writeUint(gpbFp, imageWidth);
    writeUint(gpbFp, imageHeight);
    writeUint(gpbFp, imageSize);
    
    unsigned char* distanceFieldBuffer = NULL;
    if (fontFormat == Font::DISTANCE_FIELD)
    {
        // Flip height and width since the distance field map generator is column-wise.
        distanceFieldBuffer = createDistanceFields(imageBuffer, imageHeight, imageWidth);
        fwrite(distanceFieldBuffer, sizeof(unsigned char), imageSize, gpbFp);
        writeUint(gpbFp, Font::DISTANCE_FIELD);
    }
    else
    {
        fwrite(imageBuffer, sizeof(unsigned char), imageSize, gpbFp);
        writeUint(gpbFp, Font::BITMAP);
    }

    // Close file.
    fclose(gpbFp);

    LOG(1, "%s.gpb created successfully. \n", getBaseName(outFilePath).c_str());

    // Save out a pgm monochome image file for preview
    if (fontpreview)
    {
        // Write out font map to an image.
        std::string pgmFilePath = getFilenameNoExt(outFilePath);
        pgmFilePath.append(".pgm");
        FILE* previewFp = fopen(pgmFilePath.c_str(), "wb");
        fprintf(previewFp, "P5 %u %u 255\n", imageWidth, imageHeight);
        
        if (fontFormat == Font::DISTANCE_FIELD)
        {
            // Write out the preview buffer
            fwrite((const char*)distanceFieldBuffer, sizeof(unsigned char), imageSize, previewFp);
        }
        else
        {
            fwrite((const char*)imageBuffer, sizeof(unsigned char), imageWidth * imageHeight, previewFp);
        }
        fclose(previewFp);

        LOG(1, "%s.pgm preview image created successfully. \n", getBaseName(pgmFilePath).c_str());
    }

    // Cleanup resources.
    free(imageBuffer);
    if (fontFormat == Font::DISTANCE_FIELD)
        free(distanceFieldBuffer);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return 0;
}

}
