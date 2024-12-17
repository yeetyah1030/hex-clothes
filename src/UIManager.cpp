#include "UIManager.h"

void UIManager::setup()
{
	m_gui.setup();
}

void UIManager::draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor, ofImage cam01)
{
	ofPushMatrix();
	{
		ofTranslate(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);

		ofSetColor(255, 255, 255);
		float x2 = (ofGetWidth() - cam01.getWidth()) / 2;
		float y2 = (ofGetHeight() - cam01.getHeight()) / 2;
		cam01.draw(x2, y2);
	}
	ofPopMatrix();
	
	drawGui(appMode, appModes, threshold, numContoursConsidered, minArea, maxArea, normalizedColor, dominantColor);

	updateColorTheory(dominantColor);

	switch (m_selectedColorMode)
	{
	case CVC::COMPLEMENTARY:
		m_colorTheory.displayComplementaryResult(20, 320);
		break;
	case CVC::ANALOGOUS:
		m_colorTheory.displayAnalogousResult(20, 320);
		break;
	case CVC::TRIADIC:
		m_colorTheory.displayTriadicResult(20, 320);
		break;
	}

	std::string feedback = m_colorTheory.getFeedback((CVC::ColorMode)m_selectedColorMode);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(feedback, 20, 400);
}

void UIManager::drawGui(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor)
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

		if (ImGui::RadioButton("Complementary", &m_selectedColorMode, CVC::COMPLEMENTARY))
		{

		}
		if (ImGui::RadioButton("Analogous", &m_selectedColorMode, CVC::ANALOGOUS))
		{

		}
		if (ImGui::RadioButton("Triadic", &m_selectedColorMode, CVC::TRIADIC))
		{

		}
	}
	m_gui.end();
}

bool UIManager::isAppModeChanged(CVC::APP_MODE& appMode, int& currentListBoxIndex, std::vector<std::string>& appModes)
{
	return ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, appModes);
}

void UIManager::updateColorTheory(const ofColor& baseColor)
{
	m_colorTheory.calculateAllColours(baseColor);
}
