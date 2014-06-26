#ifndef OIS_CONVERTER_H
#define OIS_CONVERTER_H

namespace GothOgre
{
	struct OISConverter
	{
		static OIS::KeyCode keyCodeToOIS(KeyCode _kc)
		{
			return (OIS::KeyCode) (int) _kc;
		}

		static KeyCode keyCodeFromOIS(OIS::KeyCode _oisKc)
		{
			return (KeyCode) (int) _oisKc;
		}

		static OIS::MouseButtonID mouseButtonToOIS(MouseButton _mb)
		{
			return (OIS::MouseButtonID) (int) _mb;
		}

		static MouseButton mouseButtonFromOIS(OIS::MouseButtonID _oisMb)
		{
			return (MouseButton) (int) _oisMb;
		}
	};

} // namespace GothOgre

#endif // OIS_CONVERTER_H