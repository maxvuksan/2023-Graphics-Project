#pragma once

/*

    Keeping track of everything time related
*/
class Time {



	public:

		/*
			total elapsed time in seconds
		*/
		static float GetSeconds() { return time; }
		/*
			total elapsed time in milliseconds
		*/
		static float GetMilliseconds() { return time * 1000.0f; }

		static float FPS() { return 1.0f / deltaTime;}


		/*
			returns the time elapsed since the last frame, in milliseconds
		*/
		static float DeltaTimeRaw() { return deltaTime; }
		static float DeltaTime() { return deltaTime * 1000.0f; }
		// short hand for Time::DeltaTime()
		static float Dt() { return deltaTime * 1000.0f; }

		static void SetDeltaTime(float dt) { deltaTime = dt; }

		static void Increment() { time += deltaTime; }
		
	private:
		static float deltaTime;
		static float time;
};