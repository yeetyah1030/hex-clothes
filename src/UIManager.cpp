#include "UIManager.h"

void UIManager::setup()
{
	m_gui.setup();
}

void UIManager::draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, ofFloatColor& normalizedColor, ofColor& dominantColor, ofImage cam01)
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
	
	drawGui(appMode, appModes, normalizedColor, dominantColor);

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

    ofDrawBitmapString("Pause the video with spacebar!", 20, 500);
}

void UIManager::drawGui(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, ofFloatColor& normalizedColor, ofColor& dominantColor)
{
    m_gui.begin();
    {
        ImGui::SetNextWindowSize(ImVec2(250, 280), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10, 10));

        if (ImGui::Begin("Hex Clothes!", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style.FramePadding = ImVec2(5, 5);
            style.ItemSpacing = ImVec2(10, 10);

            ImVec4* colors = style.Colors;
            colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.4f, 0.8f, 1.0f);  // Pink
            colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.3f, 0.7f, 1.0f);  // Darker pink when active
 
            colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.4f, 0.8f, 1.0f);  // Pink (normal state)
            colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.3f, 0.7f, 1.0f);  // Darker pink when active

            ImGui::Text("Select an app state");
            static int currentListBoxIndex = (int)appMode;
            if (isAppModeChanged(appMode, currentListBoxIndex, appModes))
            {
                appMode = (CVC::APP_MODE)currentListBoxIndex;
            }

            ImGui::Separator();
            ImGui::Text("Dominant Color:");
            ImGui::ColorEdit3("Dominant \nColor", (float*)&normalizedColor);

            ImGui::Text("Hex Color: #%02X%02X%02X",
                (int)dominantColor.r,
                (int)dominantColor.g,
                (int)dominantColor.b);

            ofSetColor(dominantColor);
            ofDrawRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2 + 120, CVC::VIDEO_BORDER_SIZE + 35, 40, 40);

            if (ImGui::RadioButton("Complementary", &m_selectedColorMode, CVC::COMPLEMENTARY))
            {
            }
            if (ImGui::RadioButton("Analogous", &m_selectedColorMode, CVC::ANALOGOUS))
            {
            }
            if (ImGui::RadioButton("Triadic", &m_selectedColorMode, CVC::TRIADIC))
            {
            }

            ImGui::End(); 
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
	m_colorTheory.calculateAllColors(baseColor);
}
