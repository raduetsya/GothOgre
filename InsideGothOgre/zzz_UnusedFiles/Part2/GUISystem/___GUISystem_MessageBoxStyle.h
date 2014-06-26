/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/
#ifndef GUISYSTEM_MESSAGE_BOX_STYLE_H
#define GUISYSTEM_MESSAGE_BOX_STYLE_H


namespace GUISystem
{
	struct GUISYSTEM_EXPORT MessageBoxResult : public EnumValue
	{
		enum Enum
		{
			BUTTON_OK       = 0,
			BUTTON_YES      = 1,
			BUTTON_NO       = 2,
			BUTTON_ABORT    = 3,
			BUTTON_RETRY    = 4,
			BUTTON_IGNORE   = 5,
			BUTTON_CANCEL   = 6,
			BUTTON_TRY      = 7,
			BUTTON_CONTINUE = 8
		};
		MessageBoxResult(int _val) : EnumValue(_val) {}
	};


	struct GUISYSTEM_EXPORT MessageBoxStyle : public FlagSet
	{
		enum Enum
		{
			NONE            = GOTHOGRE_NONE_BITS,
			BUTTON_OK       = GOTHOGRE_BIT(0),
			BUTTON_YES      = GOTHOGRE_BIT(1),
			BUTTON_NO       = GOTHOGRE_BIT(2),
			BUTTON_ABORT    = GOTHOGRE_BIT(3),
			BUTTON_RETRY    = GOTHOGRE_BIT(4),
			BUTTON_IGNORE   = GOTHOGRE_BIT(5),
			BUTTON_CANCEL   = GOTHOGRE_BIT(6),
			BUTTON_TRY      = GOTHOGRE_BIT(7),
			BUTTON_CONTINUE = GOTHOGRE_BIT(8),

			_INDEX_USER_BUTTON1 = 9, // индекс первой кнопки юзера

			BUTTON1 = GOTHOGRE_BIT( _INDEX_USER_BUTTON1 + 0),
			Button2 = GOTHOGRE_BIT( _INDEX_USER_BUTTON1 + 1),
			Button3 = GOTHOGRE_BIT( _INDEX_USER_BUTTON1 + 2),
			Button4 = GOTHOGRE_BIT( _INDEX_USER_BUTTON1 + 3),

			_COUNT_USER_BUTTONS = 4, // количество кнопок юзера
			_INDEX_ICON1 = _INDEX_USER_BUTTON1 + _COUNT_USER_BUTTONS, // индекс первой иконки

			ICON_DEFAULT = GOTHOGRE_BIT( _INDEX_ICON1),

			ICON_INFO    = GOTHOGRE_BIT( _INDEX_ICON1),
			ICON_QUEST   = GOTHOGRE_BIT( _INDEX_ICON1 + 1),
			ICON_ERROR   = GOTHOGRE_BIT( _INDEX_ICON1 + 2),
			ICON_WARNING = GOTHOGRE_BIT( _INDEX_ICON1 + 3),

			ICON1 = GOTHOGRE_BIT( _INDEX_ICON1),
			ICON2 = GOTHOGRE_BIT( _INDEX_ICON1 + 1),
			ICON3 = GOTHOGRE_BIT( _INDEX_ICON1 + 2),
			ICON4 = GOTHOGRE_BIT( _INDEX_ICON1 + 3),
			ICON5 = GOTHOGRE_BIT( _INDEX_ICON1 + 4),
			ICON6 = GOTHOGRE_BIT( _INDEX_ICON1 + 5),
			ICON7 = GOTHOGRE_BIT( _INDEX_ICON1 + 6),
			ICON8 = GOTHOGRE_BIT( _INDEX_ICON1 + 7)
		};

		MessageBoxStyle(int _val) : FlagSet(_val) {}

		// возвращает индекс иконки
		size_t getIconIndex() const;

		// возвращает индекс иконки
		size_t getButtonIndex() const;

		// возвращает список кнопок
		vector<MessageBoxStyle>::type getButtons() const;
	};

} // namespace GUISystem

#endif // GUISYSTEM_MESSAGE_BOX_STYLE_H
