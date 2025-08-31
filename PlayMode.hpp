#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

enum CatState {
	CAT_HAPPY,
	CAT_SAD,
	CAT_WORKING,
	CAT_ANGRY,
	CAT_CONFUSED,
	CAT_SLEEPY
};

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	//glm::vec2 player_at = glm::vec2(0.0f);
	
	struct Cat {
		CatState state;
		glm::vec2 pos;
	} cats[6];
	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
