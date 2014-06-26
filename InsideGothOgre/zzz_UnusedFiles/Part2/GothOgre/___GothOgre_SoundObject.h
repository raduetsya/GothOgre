

namespace GothOgre
{

	class SoundObject : public MovableObject
	{
	public:
		SoundObject();

		void setSoundSource()
		SoundSource* getSoundSource() const;

        /** Internal method called to notify the object 
		that it has been moved. */
        void _notifyMoved();

	private:
		SoundSourcePtr  mSoundSource;
	};

}