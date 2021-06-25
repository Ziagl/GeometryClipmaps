#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
private:
	bool createTexture();
	void deleteTexture();

	unsigned int ID;

	unsigned int width, height;

public:
	Texture();
	~Texture();

	void Load(char *fileName);

	/*bool loadAndCompressBMP( char *fileName, int format );
	bool saveCompressedTexture( char *fileName );
	bool loadCompressedTexture( char *fileName );*/

	void select();
};

#endif
