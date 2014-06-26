#ifndef ZEN_OBJECT_PROPERTIES_WINDOW_H
#define ZEN_OBJECT_PROPERTIES_WINDOW_H


namespace GUISystem
{
	namespace ZenObjectProperties
	{
		class Group;

		ATTRIBUTE_CLASS_LAYOUT( Window, "ZenObjectPropertiesWindow.layout");

		class Window : public BaseLayout
		{
		public:
			Window();
			~Window();

			template<typename T>
			void initialiseByAttributes(T* _owner, MyGUI::Widget* _parent = nullptr);

			void addObject(ZenStringInterface* _object);
			void removeObject(ZenStringInterface* _object);
			void removeAllObjects();

			typedef set<ZenStringInterface*>::type ObjectSet;
			typedef ConstVectorIterator<ObjectSet> ObjectIterator;
			ObjectIterator getObjectIterator() const;

			void setNeedUpdateValues();

		private:
			void setNeedUpdateGroups();
			void setNeedUpdateGroupCoords();

			Group* findGroup(const String& _groupName) const;
			Group* addGroup(const String& _groupName);
			void removeGroup(Group* _group);
			void destroyGroups();
			
			void calcMinMaxHeights();

			void initialise2(Window* _owner, MyGUI::Widget* _parent);

			void updateGeneric();
			void updateGroups();
			void updateGroupList();
			void updateGroupSizes();
			void updateGroupCoords();
			void updateValues();

			GOTHOGRE_DECLARE_FRAME_STARTED( Window );

			void notifyWindowChangeCoord(MyGUI::Window* _sender);

		private:
			friend class Group;
			void widgetCreated() {mNumCreatedWidgets++;}
			void widgetDestroyed() {mNumDestroyedWidgets++;}
			ZenStringInterface* getFirstObject() const;

		private:
			ATTRIBUTE_FIELD_WIDGET_NAME(Window, mScrollView, "scrollView");
			MyGUI::ScrollView*			mScrollView;		

			ObjectSet					mObjects;
			list<Group*>::type			mGroups;

			size_t						mNumCreatedWidgets;
			size_t						mNumDestroyedWidgets;

			bool                        mNeedUpdateGroups;
			bool                        mNeedUpdateGroupCoords;
			bool                        mNeedUpdateValues;
		};
		//---------------------------------------------------------------------
		template <typename T>
		void Window::initialiseByAttributes(T* _owner, MyGUI::Widget* _parent)
		{
			applyClassAttributes(_owner);
			initialise2(_owner, _parent);
		}
	}

	typedef ZenObjectProperties::Window  ZenObjectPropertiesWindow;
}

#endif // ZEN_OBJECT_PROPERTIES_WINDOW_H