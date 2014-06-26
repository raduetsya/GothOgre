



	class Sound : public MovableObject
	{

	public:
		void setSound2D(const String& _filename, const String& _resourceGroup);

		void setSound2D(const String& _filename);
		void setSound3D(const String& _filename);
		void setMusic2D(const String& _filename);
		void setMusic3D(const String& _filename);

		void play(bool _3D);
		void stop();

		void setVolume(Real _volume);
		void setMaxDistance(Real _radius);
	};

	class SoundSystem : public Singleton<SoundSystem>
	{
		Sound* createSound();

		void setListener(MovableObject* _listener);
	};

	//-----------------------------------------------------------------------
	/** SoundChannel - Entity which is used to display Bsp. */
	class GOTHOGRE_EXPORT SoundChannel : public MovableObject
	{
	public:
		// Тип источника звука
		enum ChannelType
		{
			// Двухмерный (рассеянный)
			CHANNEL_2D,

			// Трехмерный (пространственный)
			CHANNEL_3D
		};

		// Текущий статус источника звука
		enum ChannelStatus
		{
			// Проигрывание остановлено (стутус по умолчанию)
			STOPPED,

			// Проигрывание поставлено на паузу
			PAUSED,

			// Проигрывается
			PLAYING,
			
			// Проигрывается где-то далеко от слушателя, 
			// так что звук не слышен
			PLAYING_FAR_AWAY
		};

		// Действие по завершению проигрывания трека
		enum EndAction
		{
			// Ничего не делать; звук проиграется до конца и все
			NONE,

			// Удалить источник звука после завершения проигрывания
			// полезно для временных звуков, появляющихся из-за текущих
			// событий, типа отдельных ударов мечом или крика
			DELETE_CHANNEL,
			
			// Повторить звук снова
			LOOP_BACK,

			// Подождать некоторое время, а затем повторить звук снова
			DELAYED_LOOP_BACK
		};

		// Получить указатель на ресурс, содержащий трек
		const SoundPtr& getSound() const {return mSound;}

		// Запустить проигрывание 2D звука
		void play2D()
		{
			stop();
			mChannelType = CHANNEL_2D;
			mChannelStatus = PLAYING_FAR_AWAY;
		}

		// Запустить проигрывание 3D звука
		void play3D()
		{
			stop();
			mChannelType = CHANNEL_3D;
			mChannelStatus = PLAYING_FAR_AWAY;
		}

		void _play2D()
		{
			mChannelData = mSound->play2D();
			mChannelData->setVolume(mVolume);
			mChannelStatus = PLAYING;
		}

		void _play3D()
		{
			mChannelData = mSound->play3D();
			mChannelData->setVolume(mVolume);
			mChannelData->setRadius(mRadius);
			mChannelData->setPosition(getParentNode()->getDerivedPosition());
			mChannelStatus = PLAYING;
		}

		// Остановить проигрывание
		void stop()
		{
			if(mChannelData)
			{
				delete mChannelData;
				mChannelData = nullptr;
			}
			mChannelStatus = STOPPED;
		}

		// Поставить на паузу
		void pause(bool _pause = true)
		{
			if(mChannelData)
				mChannelData->pause(_pause);
			if(mChannelStatus == PLAYING)
				mChannelStatus = PAUSED;
		}

		// Снять с паузы
		void unpause() {pause(false);}

		// Проверить, поставлено ли проигрывание на паузу
		bool isPaused() const {return getChannelStatus() == PAUSED;}

		// Получить длину трека в секундах
		Real getPlayDuration() const
		{
			if(mChannelData)
				return mChannelData->getSoundData()->getPlayDuration();
			return 0;
		}

		// Получить текущую позицию в секундах от начала трека
		Real getPlayPosition() const
		{
			if(mChannelData)
				return mChannelData->getPlayPosition();
			return 0;
		}

		// Получить тип источника звука
		ChannelType getChannelType() {return mChannelType;}

		// Получить текущий статус источника звука
		ChannelStatus getChannelStatus() {return mChannelStatus;}

		// Установить действие, которое должно быть выполнено 
		// по заверщению проигрывания
		void setEndAction(EndAction _ea)
		{
			mEndAction = _ea;
		}

		// Получить действие, которое должно быть выполнено 
		// по заверщению проигрывания
		EndAction getEndAction() const {return mEndAction;}

		// Установить громкость звука, в интервале от 0 до 1
		void setVolume(Real _volume)
		{
			mVolume = _volume;
			if(mChannelData)
				mChannelData->setVolume(_volume);
		}

		// Получить громкость звука, в интервале от 0 до 1
		Real getVolume() {return mVolume;}

		// Установить максимальное расстояние от источника звука,
		// на котором звук еще слышен (только для 3D звука)
		void setBoundingRadius(Real _radius)
		{
			mBoundingRadius = _radius;
			if(mChannelData)
				mChannelData->setBoundingRadius(_radius);
		}

		// Получить максимальное расстояние от источника звука,
		// на котором звук еще слышен (только для 3D звука)
		Real getBoundingRadius() const {return mBoundingRadius;}

		// Установить ограничивающий параллелепипед, внутри которого 
		// проигрывается звук, громкость звука не зависит от положения 
		// слушателя внутри параллелепипеда (только для 2D звука)
		void setBoundingBox(const AxisAlignedBox& _box)
		{
			mBoundingBox = _box;
		}

		// Получить ограничивающий параллелепипед, внутри которого 
		// проигрывается звук, громкость звука не зависит от положения 
		// слушателя внутри параллелепипеда (только для 2D звука)
		const AxisAlignedBox& getBoundingBox() const {return mBoundingBox;}

		// Установить минимальную задержку, в секундах (см. описание DelayedLoopBack)
		void setMinDelay(Real _minDelay)
		{
			mMinDelay = _minDelay;
		}

		// Получить минимальную задержку, в секундах (см. описание DelayedLoopBack)
		Real getMinDelay() const {return mMinDelay;}

		// Установить максимальную задержку, в секундах (см. описание DelayedLoopBack)
		void setMaxDelay(Real _maxDelay)
		{
			mMaxDelay = _maxDelay;
		}

		// Получить максимальную задержку, в секундах (см. описание DelayedLoopBack)
		Real getMaxDelay() const {return mMaxDelay;}

	public:
		// Returns the type name of this object. 
		const String& getMovableType() const;

		// Get the 'type flags' for this MovableObject.
		// Возвращает 0, т.к. источник звука в запросах не участвует,
		// и в тесте столкновений участия не принимает.
		uint32 getTypeFlags() const {return 0;}
		
		// Internal method by which the movable object must 
		// add Renderable subclass instances to the rendering queue.
		// Функция ничего не делает, источник звука никак не отрисовывается.
		void _updateRenderQueue(RenderQueue* queue) {}

	private:
		SoundChannel()
		{
			mChannelType = CHANNEL_2D;
			mChannelStatus = STOPPED;
			mEndAction = NONE;
		}

		SoundChannel(const String& _name, const SoundPtr& _sound);

	private:
		SoundPtr                mSound;
		SoundData::ChannelData*  mChannelData;
		ChannelType              mChannelType;
		ChannelStatus            mChannelStatus;
		EndAction               mEndAction;
		Real                    mBoundingRadius;
		AxisAlignedBox          mBoundingBox;
		Real                    mVolume;
		Real                    mMinDelay;
		Real                    mMaxDelay;
	};

	class GOTHOGRE_EXPORT SoundChannelFactory : public MovableObjectFactory
	{
	public:
		SoundChannelFactory();
		~SoundChannelFactory();
		bool requestTypeFlags() const;
		const String& getType() const;
		static String FACTORY_TYPE_NAME;
	};

	class GOTHOGRE_EXPORT SoundListener : public MovableObject
	{

	};

	class SoundData
	{
	public:
		class ChannelData
		{
		public:
			// Конструктор - начинает проигрывание
			ChannelData();

			// Деструктор - завершает проигрывание, 
			// если оно еще не завершилось
			virtual ~ChannelData();

			// Поставить на паузу или снять с паузы
			virtual void pause(bool _pause) = 0;

			// Зациклить звук
			virtual void setLooped(bool _looped) = 0;
			
			// Проверить, проигрывается ли звук еще
			virtual bool isPlaying() const = 0;
			
			// Получить позицию от начала трека в секундах
			virtual Real getPlayPosition() const = 0;

			// Установить громкость звука, от 0 до 1
			virtual void setVolume(Real _volume) = 0;
			
			// Установить максимальное расстояние от источника,
			// за пределами которого звук не слышен
			virtual void setRadius(Real _radius) = 0;

			// Установить позицию источника звука
			virtual void setPosition(const Vector3& _position);
		};

	public:
		SoundData();
		virtual ~SoundData();

		virtual ChannelData* play2D() = 0;
		virtual ChannelData* play3D() = 0;
		virtual Real getPlayDuration() const = 0;
	};