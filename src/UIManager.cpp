#include "UIManager.h"

void UIManager::setup()
{
	m_gui.setup();
}

void UIManager::draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor)
{	
	m_gui.begin();
	{
		ImGui::Text("Color Detection");
		ImGui::SliderInt("Threshold", &threshold, 0, 255);
		ImGui::SliderInt("Contours", &numContoursConsidered, 0, 30);
		ImGui::SliderFloat("Min. Area", &minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
		ImGui::SliderFloat("Max. Area", &maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

		ImGui::Separator();

		ImGui::Text("\n Select an app state");
		static int currentListBoxIndex = (int)appMode;
		if (isAppModeChanged(appMode, currentListBoxIndex, appModes))
		{
			appMode = (CVC::APP_MODE)currentListBoxIndex;
		}

		ImGui::Text("Dominant Color:");
		ImGui::ColorEdit3("Dominant Color", (float*)&normalizedColor);

		ofSetColor(dominantColor);
		ofDrawRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2 + 10, CVC::VIDEO_BORDER_SIZE + 10, 50, 50);
	}
	m_gui.end();
}

bool UIManager::isAppModeChanged(CVC::APP_MODE& appMode, int& currentListBoxIndex, std::vector<std::string>& appModes)
{
	return ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, appModes);
}
