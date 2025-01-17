#include <pch.h>
#include "Settings.h"

#include <cheat-base/render/gui-util.h>
#include <cheat-base/render/renderer.h>
namespace cheat::feature 
{
    Settings::Settings() : Feature(),
		NF(f_MenuKey, "Show Cheat Menu Key", "General", Hotkey(VK_F1)),
		NF(f_HotkeysEnabled, "Hotkeys Enabled", "General", true),
		NF(f_FontSize, "Font size", "General", 16.0f),

		NF(f_StatusMove, "Move Status Window", "General::StatusWindow", true),
		NF(f_StatusShow, "Show Status Window", "General::StatusWindow", true),
		
		NF(f_InfoMove,   "Move Info Window", "General::InfoWindow", true),
		NF(f_InfoShow,   "Show Info Window", "General::InfoWindow", true),
		
		NF(f_FpsMove, "Move FPS Indicator", "General::FPS", false),
		NF(f_FpsShow, "Show FPS Indicator", "General::FPS", true),

		NF(f_NotificationsShow, "Show Notifications", "General::Notify", true), 
		NF(f_NotificationsDelay, "Notifications Delay", "General::Notify", 500),
  
		NF(f_FileLogging,    "File Logging",    "General::Logging", false),
		NF(f_ConsoleLogging, "Console Logging", "General::Logging", true),

		NF(f_FastExitEnable, "Fast Exit", "General::FastExit", false),
		NF(f_HotkeyExit, "Hotkeys", "General::FastExit", Hotkey(VK_F12))
		
    {
		renderer::SetGlobalFontSize(f_FontSize);
    }

    const FeatureGUIInfo& Settings::GetGUIInfo() const
    {
        static const FeatureGUIInfo info{ "", "Settings", false };
        return info;
    }

	void Settings::DrawMain()
	{

		BeginGroupPanel("General", ImVec2(-1, 0));
		{
			ConfigWidget(f_MenuKey, false,
				"Key to toggle main menu visibility. Cannot be empty.\n"\
				"If you forget this key, you can see or set it in your config file.");
			ConfigWidget(f_HotkeysEnabled, "Enable hotkeys.");
			if (ConfigWidget(f_FontSize, 1, 8, 64, "Font size for cheat interface."))
			{
				f_FontSize = std::clamp(f_FontSize.value(), 8, 64);
				renderer::SetGlobalFontSize(f_FontSize);
			}
		}
		EndGroupPanel();

		BeginGroupPanel("Logging", ImVec2(-1, 0));
		{
			bool consoleChanged = ConfigWidget(f_ConsoleLogging,
				"Enable console for logging information (changes will take effect after relaunch)");
			if (consoleChanged && !f_ConsoleLogging)
			{
				Logger::SetLevel(Logger::Level::None, Logger::LoggerType::ConsoleLogger);
			}

			bool fileLogging = ConfigWidget(f_FileLogging,
				"Enable file logging (changes will take effect after relaunch).\n" \
				"A folder in the app directory will be created for logs.");
			if (fileLogging && !f_FileLogging)
			{
				Logger::SetLevel(Logger::Level::None, Logger::LoggerType::FileLogger);
			}
		}
		EndGroupPanel();

		BeginGroupPanel("Status Window", ImVec2(-1, 0));
		{
			ConfigWidget(f_StatusShow);
			ConfigWidget(f_StatusMove, "Allow moving of 'Status' window.");
		}
		EndGroupPanel();

		BeginGroupPanel("Info Window", ImVec2(-1, 0));
		{
			ConfigWidget(f_InfoShow);
			ConfigWidget(f_InfoMove, "Allow moving of 'Info' window.");
		}
		EndGroupPanel();

		BeginGroupPanel("FPS indicator", ImVec2(-1, 0));
		{
			ConfigWidget(f_FpsShow);
			ConfigWidget(f_FpsMove, "Allow moving of 'FPS Indicator' window.");
		}
		EndGroupPanel();

		BeginGroupPanel("Show Notifications", ImVec2(-1, 0));
		{
			ConfigWidget(f_NotificationsShow, "Notifications on the bottom-right corner of the window will be displayed.");
			ConfigWidget(f_NotificationsDelay, 1,1,10000, "Delay in milliseconds between notifications.");
		}
		EndGroupPanel();

		BeginGroupPanel("Fast Exit", ImVec2(-1, 0));
		{
			ConfigWidget("Enabled",
				f_FastExitEnable,
				"Enable Fast Exit.\n" 
			);
			if (!f_FastExitEnable)
				ImGui::BeginDisabled();

			ConfigWidget("Key", f_HotkeyExit, true,
				"Key to exit the game.");

			if (!f_FastExitEnable)
				ImGui::EndDisabled();

			FastExit();
		}
		EndGroupPanel();
	}

	void Settings::FastExit()
	{
		if (!f_FastExitEnable)
			return;

		if (f_HotkeyExit.value().IsPressed())
		{
			ExitProcess(0);
		}
	}

    Settings& Settings::GetInstance()
    {
        static Settings instance;
        return instance;
    }
}

