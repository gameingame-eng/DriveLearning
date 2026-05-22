#pragma once

#include <string>
#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

namespace DriveLearning
{
	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
	class SampleFpsTextRenderer
	{
	public:
		SampleFpsTextRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Resources related to home screen text rendering.
		float                                           m_blinkTimer;
		bool                                            m_showStartPrompt;
		std::wstring                                    m_titleText;
		std::wstring                                    m_subtitleText;
		std::wstring                                    m_startPromptText;
		std::wstring                                    m_menuText;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_titleBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_bodyBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_accentBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_titleFormat;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_subtitleFormat;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_promptFormat;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_menuFormat;
	};
}
