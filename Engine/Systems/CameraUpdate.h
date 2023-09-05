#pragma once
#include "System.h"
#include <typeinfo>
namespace gde { class GoldDayEngine; }

namespace gde::system {
	class CameraUpdate : public System {
	public:
		CameraUpdate(GoldDayEngine& _engine);
		~CameraUpdate();

		std::string getType() const override { return typeid(CameraUpdate).name(); };

		CameraUpdate(const CameraUpdate&) = delete;
		CameraUpdate& operator=(const CameraUpdate&) = delete;

		void setSignature() override;
		

		void setCameraAspectRatio();
		void setCameraUBOData(GlobalUbo& ubo);
	};
}