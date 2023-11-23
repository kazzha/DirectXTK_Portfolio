#pragma once

namespace DX
{
	class IDeviceNotify // interface
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		~IDeviceNotify() = default;

	};
	class DeviceResources
	{

	};
}