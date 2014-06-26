#ifndef ABOUT_GOTHOGRE_DIALOG_H
#define ABOUT_GOTHOGRE_DIALOG_H

namespace GUISystem
{
	ATTRIBUTE_CLASS_LAYOUT        ( AboutGothOgreDialog, "AboutGothOgreDialog.layout");
	ATTRIBUTE_CLASS_CONFIG_SECTION( AboutGothOgreDialog, "AboutGothOgreDialog");

	class AboutGothOgreDialog : public BaseLayout
	{
	public:
		AboutGothOgreDialog();
		
		// event for pressed "close" button
		typedef MyGUI::delegates::CDelegate0 EventHandle_Result;
		EventHandle_Result eventEndDialog;

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const String& _name);
		void close();
	};

} // namespace GUISystem

#endif // ABOUT_GOTHOGRE_DIALOG_H