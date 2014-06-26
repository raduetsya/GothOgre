#include "Precompiled.h"
#include "SoundFileDialog.h"


namespace GUISystem
{
	//------------------------------------------------------------------------------------
	BaseSoundFileDialog::BaseSoundFileDialog()
	{
		setType(FileDialog::LOAD);
		setCaption("#{SelectFileToPlay}");
		setOkText("#{Play}");
		setCancelText("#{Close}");

		// First filter
		{
			Filter filter;
			filter.setDescription("#{WaveAudio}");
			filter.addPattern("*.wav");
			filter.setDefaultExt(".wav");
			addFilter(filter);
		}

		// Second filter
		{
			Filter filter;
			filter.setDescription("#{OggVorbis}");
			filter.addPattern("*.ogg");
			filter.setDefaultExt(".ogg");
			addFilter(filter);
		}

		// Third filter
		{
			Filter filter;
			filter.setDescription("#{FLAC}");
			filter.addPattern("*.flac");
			filter.setDefaultExt(".flac");
			addFilter(filter);
		}

		// Fourth filter
		{
			Filter filter;
			filter.setDescription("#{DirectMusicSgt}");
			filter.addPattern("*.sgt");
			filter.setDefaultExt(".sgt");
			addFilter(filter);
		}
	}
	//------------------------------------------------------------------------------------
	SoundFileDialog::SoundFileDialog()
	{
		setResourceGroup(ResourceGroup::SOUND);
		setDefaultFilter(0);
		initialiseByAttributes(this);
		setModal(false);
	}
	//------------------------------------------------------------------------------------
	MusicFileDialog::MusicFileDialog()
	{
		setResourceGroup(ResourceGroup::MUSIC);
		setDefaultFilter(3);
		initialiseByAttributes(this);
		setModal(false);
	}

} // namespace GUISystem