#pragma once

class InputHandler
{

public:

	

	static void Update(uint32_t milliseconds, Trainer* player, Trainer* opponent);

	static void init();

	static void shutdown();

private:
	
	static void GameReset();
	

};

