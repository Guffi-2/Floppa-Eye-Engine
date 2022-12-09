#pragma once

std::map<const char*, float>htlastTick;

namespace helpTools {
	bool EveryTicks(float ticket, const char* name, short* repeat, bool every = false){
#ifdef _WIN64
		auto ticksNow = GetTickCount64();
#else
		auto ticksNow = GetTickCount();
#endif // _WIN64

		*repeat = ticksNow - htlastTick[name];

		if (!every) {
			if (ticksNow - htlastTick[name] >= ticket) {
				htlastTick[name] = ticksNow;
				return true;
			}
		}
		else {
			if (ticksNow != htlastTick[name]) {
				htlastTick[name] = ticksNow;
				return true;
			}
		}

		return false;
	}
}
