#include "gamestate.h"

#include "glApi/test.h"

#include "text.h"

#define TEST_DRAW_FULL_CHAR_MAP (0)
#define MAX_NUM_TEXT (50)

enum texVboEnum {
	T00 = 0, T01, T02, T03, T04, T05,
	TMAX
};

enum scrEnum {
	scrShowIntro = 0, scrShowDogs, scrShowStart, scrMax
};

// struct img_2d_dimension {
// 	int w, h;
// 	float transX, transY, rotZ;
// };

class introstate : public gamestate {
public:
	introstate() {
		name = "_intro_state";
	}

	~introstate() {}

	virtual  int  Create() override;
	virtual void Destroy() override;

	virtual void InputHandle(const void *data_handle) override;

	virtual void Update(float delta) override;
	virtual void Render() override;

	virtual bool isRunning() override {
		return running;
	}

	void draw_picture(texVboEnum pic, float alpha, float up);

	#if TEST_DRAW_FULL_CHAR_MAP
		void draw_full_char_map(float up);
	#endif

	void draw_text(unsigned int prog, unsigned int tex, const glm::mat4 *projection2D_Mat, unsigned int offset, unsigned int count, float xStart, float yStart, float scaleX, float scaleY, float alpha);

private:

	// Shaders, textures, and vbos for attributes of each vertex.
	unsigned int imgShader, textShader;

	// unsigned int vertBuffIds[TMAX], charMapBuffId, uvBuffId, uvBuffQuatId;
	// unsigned int texIds[TMAX], textTexId;

	unsigned int imgTexIds[TMAX];
	unsigned int imgPosBuffIds[TMAX], imgUvBuffId;  // images' vob: pos and uv attri for img-shader
	img_2d_dimension imgDimensions[TMAX];

	unsigned int charMapTexId;
	Text tsText[MAX_NUM_TEXT];                                    // text's vob: pos and uv attri for text-shader

	#if TEST_DRAW_FULL_CHAR_MAP
		// These are supposed to be used in combination with char-map's texture 
		// to test draw_full_char_map()
		unsigned int charMapPosBuff, imgUvQuatBuff;
		img_2d_dimension charMapDimension;
	#endif

	int viewportWidth, viewportHeight;
	glm::mat4 projection2D_Mat;

	// If the enum-values are un-inited, calling render() before update() 
	// (inits the values) may crash the program 
	// due to their invalid default values.
	texVboEnum targetTexVboIdx = TMAX;
	scrEnum screen = scrMax;
	float alpha, up;

	unsigned int ticks = 0;
	// bool running = FALSE;
	bool running = 0;
};
