#ifndef EDITOR_COMMON_H
#define EDITOR_COMMON_H


namespace GothOgre
{
	class EditorCommon
	{
	public:
		EditorCommon();
		~EditorCommon();

		void _initialize();
		void _shutdown();

	private:
		class Initializer;
		class Shutdowner;
		Initializer*   mInitializer;
		Shutdowner*    mShutdowner;

	private:
		class Initializer : public UIModeListener
		{
		public:
			Initializer(EditorCommon* _common);
			~Initializer();
		protected:
			virtual void switchUIMode(const UIModeEvent& _evt);
		private:
			EditorCommon* mEditorCommon;
		};

	private:
		class Shutdowner : public UIModeListener
		{
		public:
			Shutdowner(EditorCommon* _common);
			~Shutdowner();
		protected:
			virtual void switchUIMode(const UIModeEvent& _evt);
		private:
			EditorCommon* mEditorCommon;
		};
	};

} // namespace GothOgre

#endif // EDITOR_COMMON_H
