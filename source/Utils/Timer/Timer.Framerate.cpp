#include "Utils/Timer/Timer.Framerate.h"

void FPS_Timer::SetTargetFPS(const double& value) {
	targetFPS = value;
	if (targetFPS == 0){
		frameTime = 0.0;
		Delta = 0.0;
	}
	else {
		frameTime = 1.0 / targetFPS;
		Delta = 0.0;
	}
	deltaTimer.start();
}

double FPS_Timer::GetTargetFPS() {
	return targetFPS;
}

double FPS_Timer::GetDeltaTime() {
	return Delta;
}

void FPS_Timer::wait() {
	deltaTimer.stop();
	Delta = deltaTimer.elapsedSeconds();

	if (targetFPS == 0) {
		deltaTimer.start();
		return;
	}


	#ifdef _WIN32
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		double elapsedTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;



		if (elapsedTime < frameTime) {
			DWORD sleepTime = static_cast<DWORD>((frameTime - elapsedTime) * 1000.0);
			if (sleepTime != 0)
				--sleepTime;
			if (sleepTime > 0)
				Sleep(sleepTime);

			do {
				QueryPerformanceCounter(&currentTime);
				elapsedTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
			} while (elapsedTime < frameTime);
		}

		Delta = elapsedTime;
		lastTime = currentTime;
	#endif
	
	#if defined(__linux__)
		struct timespec currentTime;
		clock_gettime(CLOCK_MONOTONIC, &currentTime);

		double elapsedTime = (currentTime.tv_sec - lastTime.tv_sec)
							+ (currentTime.tv_nsec - lastTime.tv_nsec) / 1e9;

		if (elapsedTime < frameTime) {
			double remaining = frameTime - elapsedTime;

			// Sleep the bulk of the remaining time with nanosleep (high resolution)
			struct timespec req;
			req.tv_sec = static_cast<time_t>(std::floor(remaining));
			req.tv_nsec = static_cast<long>((remaining - std::floor(remaining)) * 1e9);

			// nanosleep is usually precise, but we do a short busy-wait after it to reach target more accurately
			if (req.tv_sec != 0 || req.tv_nsec != 0) {
				nanosleep(&req, nullptr);
			}

			// busy-wait until we've reached frameTime
			do {
				clock_gettime(CLOCK_MONOTONIC, &currentTime);
				elapsedTime = (currentTime.tv_sec - lastTime.tv_sec)
							+ (currentTime.tv_nsec - lastTime.tv_nsec) / 1e9;
			} while (elapsedTime < frameTime);
		}

		Delta = elapsedTime;
		lastTime = currentTime;
	#endif



	deltaTimer.start();
}

FPS_Timer::FPS_Timer() {
	frameTime = 1.0 / targetFPS;
	#ifdef _WIN32
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&lastTime);
		timeBeginPeriod(1);
	#endif

	#if defined(__linux__)
    	clock_gettime(CLOCK_MONOTONIC, &lastTime);
	#endif
}

FPS_Timer::~FPS_Timer() {
	#ifdef _WIN32
		timeEndPeriod(1);
	#endif
}
