#include "UIManager.h"

UIManager::UIManager()
{
	m_threshold = 128;
	m_numContours = 1;
	m_minArea = 10.0f;
	m_maxArea = (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT);
}

void UIManager::setup()
{
	m_gui.setup();
}

void UIManager::update()
{
	m_gui.begin();
	{
		ImGui::Text("Your Clothes in Hex Codes :3");
		ImGui::SliderInt("Threshold", &m_threshold, 0, 255);
		ImGui::SliderInt("Num of Contours", &m_numContours, 0, 30);
		ImGui::SliderFloat("Min. Area", &m_minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
		ImGui::SliderFloat("Max. Area", &m_maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

		ImGui::Separator();
	}
	m_gui.end();
}

void UIManager::draw(ofApp* app)
{
	ofColor selectedColor = ofColor(
		app->m_trackedColor[0] * 255,
		app->m_trackedColor[1] * 255,
		app->m_trackedColor[2] * 255
	);

	m_colourTheory.calculateAllColours(selectedColor);

	int startX = 20;
	int startY = 200;

	m_colourTheory.drawAll();
}


