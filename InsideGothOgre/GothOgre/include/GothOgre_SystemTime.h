#ifndef GOTHOGRE_SYSTEM_TIME_H
#define GOTHOGRE_SYSTEM_TIME_H

#include "GothOgre_Time.h"


namespace GothOgre
{
	/** System time, i.e. the local time of the computer in the real world. */
	class GOTHOGRE_EXPORT SystemTime : public Singleton<SystemTime>
	{
	public:
		SystemTime();
		~SystemTime();

		/// Retrieves the current local time;
		/// year and month are not retrieved.
		Time getCurrentTime() const;
	};
}

#endif // GOTHOGRE_SYSTEM_TIME_H
