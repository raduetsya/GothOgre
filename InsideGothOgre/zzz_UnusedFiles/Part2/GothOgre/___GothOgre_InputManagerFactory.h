#ifndef GOTHOGRE_INPUT_MANAGER_FACTORY_H
#define GOTHOGRE_INPUT_MANAGER_FACTORY_H


namespace GothOgre
{
	class InputManager;


	//----------------------------------------------------------------------
	class GOTHOGRE_EXPORT InputManagerFactory
	{
	public:
		InputManagerFactory(const String& _name);
		virtual ~InputManagerFactory();

		const String& getName() const {return mName;}

		virtual InputManager* createInstance(RenderWindow* _window);
		virtual void destroyInstance(InputManager* _manager);

	protected:
		virtual InputManager* createInstanceImpl() = 0;

	private:
		String  mName;
	};
}

#endif // GOTHOGRE_INPUT_MANAGER_FACTORY_H