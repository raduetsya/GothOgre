#ifndef PLAY_SOUND_TOOL_H
#define PLAY_SOUND_TOOL_H


namespace GUISystem
{
	class BaseSoundFileDialog;
	class SoundPlayerDialog;


	//-------------------------------------------------------------------
	class PlaySoundTool : public Tool
	{
	public:
		PlaySoundTool();
		~PlaySoundTool();

		// Tool parameter is used: if it equals to "
		// then the tool will merge worlds
		ToolOperation* activate(const Any& _toolParameter);

		struct ToolParam
		{
			String resourceGroup;
		};
	};


	//-------------------------------------------------------------------
	class PlaySoundToolOperation : public ToolOperation
	{
	public:
		PlaySoundToolOperation( const PlaySoundTool::ToolParam& _tp );
		~PlaySoundToolOperation();

	private:
		void notifyEndFileDialog(const String& _filename);
		void notifyEndPlayerDialog();

	private:
		BaseSoundFileDialog*  mFileDialog;
		SoundPlayerDialog*    mPlayerDialog;
	};


	//-------------------------------------------------------------------
	class PlaySoundToolFactory : public ToolFactory
	{
	public:
		PlaySoundToolFactory();
		Tool* createTool();
	};

	
} // namespace GUISystem
	
#endif // PLAY_SOUND_TOOL_H
