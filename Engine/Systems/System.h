#pragma once
#include <bitset>

namespace gde::system {
	using Signature = std::bitset<MAX_COMPONENTS>;
	class System {
	public:
		System(GoldDayEngine& _engine):engine(_engine) {}

		System(const System&) = delete;
		System& operator=(const System&) = delete;

		virtual std::string getType() const = 0;

		void setSignature(Signature signature) { systemSignature = signature; }

	protected:
		GoldDayEngine& engine;

		Signature systemSignature;
	};
}