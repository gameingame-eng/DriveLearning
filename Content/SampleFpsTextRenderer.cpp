#include "pch.h"
#include "SampleFpsTextRenderer.h"

#include "Common/DirectXHelper.h"

using namespace DriveLearning;
using namespace Microsoft::WRL;

// Initializes D2D resources used for text rendering.
SampleFpsTextRenderer::SampleFpsTextRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
	m_blinkTimer(0.0f),
	m_showStartPrompt(true),
	m_titleText(L"DRIVE LEARNING"),
	m_subtitleText(L"Learn math and spelling while you drive."),
	m_startPromptText(L"Press A to Start"),
	m_menuText(L"New Game    |    How to Play    |    Settings"),
	m_deviceResources(deviceResources)
{
	// Create device-independent resources.
	ComPtr<IDWriteTextFormat> titleFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI Black",
			nullptr,
			DWRITE_FONT_WEIGHT_HEAVY,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-US",
			&titleFormat
			)
		);
	DX::ThrowIfFailed(titleFormat.As(&m_titleFormat));
	DX::ThrowIfFailed(m_titleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_titleFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	ComPtr<IDWriteTextFormat> subtitleFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_SEMI_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			30.0f,
			L"en-US",
			&subtitleFormat
			)
		);
	DX::ThrowIfFailed(subtitleFormat.As(&m_subtitleFormat));
	DX::ThrowIfFailed(m_subtitleFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_subtitleFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	ComPtr<IDWriteTextFormat> promptFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI Semibold",
			nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			36.0f,
			L"en-US",
			&promptFormat
			)
		);
	DX::ThrowIfFailed(promptFormat.As(&m_promptFormat));
	DX::ThrowIfFailed(m_promptFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_promptFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	ComPtr<IDWriteTextFormat> menuFormat;
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			24.0f,
			L"en-US",
			&menuFormat
			)
		);
	DX::ThrowIfFailed(menuFormat.As(&m_menuFormat));
	DX::ThrowIfFailed(m_menuFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_menuFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);

	CreateDeviceDependentResources();
}

// Updates the text to be displayed.
void SampleFpsTextRenderer::Update(DX::StepTimer const& timer)
{
	m_blinkTimer += static_cast<float>(timer.GetElapsedSeconds());
	if (m_blinkTimer >= 0.6f)
	{
		m_showStartPrompt = !m_showStartPrompt;
		m_blinkTimer = 0.0f;
	}
}

// Renders a frame to the screen.
void SampleFpsTextRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();
	context->SetTransform(m_deviceResources->GetOrientationTransform2D());

	const float width = logicalSize.Width;
	const float height = logicalSize.Height;

	const D2D1_RECT_F titleRect = D2D1::RectF(0.0f, height * 0.16f, width, height * 0.36f);
	const D2D1_RECT_F subtitleRect = D2D1::RectF(0.0f, height * 0.37f, width, height * 0.48f);
	const D2D1_RECT_F startPromptRect = D2D1::RectF(0.0f, height * 0.58f, width, height * 0.68f);
	const D2D1_RECT_F menuRect = D2D1::RectF(0.0f, height * 0.80f, width, height * 0.88f);

	context->DrawText(
		m_titleText.c_str(),
		static_cast<UINT32>(m_titleText.length()),
		m_titleFormat.Get(),
		titleRect,
		m_titleBrush.Get()
	);

	context->DrawText(
		m_subtitleText.c_str(),
		static_cast<UINT32>(m_subtitleText.length()),
		m_subtitleFormat.Get(),
		subtitleRect,
		m_bodyBrush.Get()
	);

	if (m_showStartPrompt)
	{
		context->DrawText(
			m_startPromptText.c_str(),
			static_cast<UINT32>(m_startPromptText.length()),
			m_promptFormat.Get(),
			startPromptRect,
			m_accentBrush.Get()
		);
	}

	context->DrawText(
		m_menuText.c_str(),
		static_cast<UINT32>(m_menuText.length()),
		m_menuFormat.Get(),
		menuRect,
		m_bodyBrush.Get()
	);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void SampleFpsTextRenderer::CreateDeviceDependentResources()
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(0.95f, 0.96f, 0.99f, 1.0f),
			&m_titleBrush)
		);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(0.86f, 0.90f, 0.97f, 1.0f),
			&m_bodyBrush)
		);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(0.17f, 0.75f, 1.0f, 1.0f),
			&m_accentBrush)
		);
}
void SampleFpsTextRenderer::ReleaseDeviceDependentResources()
{
	m_titleBrush.Reset();
	m_bodyBrush.Reset();
	m_accentBrush.Reset();
}
