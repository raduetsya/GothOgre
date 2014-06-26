#include "GothOgre_Precompiled.h"
#include "GothOgre_Time.h"


namespace GothOgre
{
	//--------------------------------------------------------------------
	const Time Time::ZERO       = Time(0);
	const Time Time::ONE_SECOND = Time(1);
	const Time Time::ONE_MINUTE = Time(60);
	const Time Time::ONE_HOUR   = Time(3600);
	const Time Time::ONE_DAY    = Time(86400);
	const Time Time::INFINITY   = Time(Math::POS_INFINITY);
	//----------------------------------------------------------------------------------
	void Time::unpack(int& _days, int& _hours, int& _minutes, double& _seconds) const
	{
		// Chack if the time is negative
		bool negative = false;
		double absTotalSeconds = mTotalSeconds;
		if(absTotalSeconds < 0)
		{
			absTotalSeconds = -absTotalSeconds;
			negative = true;
		}

		// Calculations with positive values
		int iTotalSeconds = int(floor(absTotalSeconds));
		int iTotalMinutes = iTotalSeconds / 60;
		int iTotalHours   = iTotalMinutes / 60;
		int iTotalDays    = iTotalHours / 24;
		_days    = iTotalDays;
		_hours   = iTotalHours - iTotalDays * 24;
		_minutes = iTotalMinutes - iTotalHours * 60;
		_seconds = absTotalSeconds - iTotalMinutes * 60;

		// Apply the negative sign if it was
		if(negative)
		{
			_days    = -_days;
			_hours   = -_hours;
			_minutes = -_minutes;
			_seconds = -_seconds;
		}
	}
	//--------------------------------------------------------------------
	StrStream& operator <<(StrStream& _ss, const Time& _time)
	{
		int days, hours, minutes;
		double seconds;
		_time.unpack(days, hours, minutes, seconds);

		// Output a minus sign if the time is negative/
		if(_time < Time(0))
		{
			_ss << '-';
			days    = -days;
			hours   = -hours;
			minutes = -minutes;
			seconds = -seconds;
		}

		// Precision is used to decide should we output seconds 
		// and even smaller parts of seconds (microseconds, milliseconds, etc.).
		int precision = (int) _ss.precision();

		// Output days.
		if(days != 0)
		{
			_ss << "Day ";
			_ss << days;
			_ss << ", ";
		}
		
		// Output hours and minutes.
		_ss << (int) hours;
		_ss << ':';
		_ss.width(2);
		_ss.fill('0');
		_ss << (int) minutes;

		if(precision >= 0)
		{
			// Output seconds
			_ss << ':';
			_ss.width(2);
			_ss.fill('0');
			int iSeconds = int(floor(seconds));
			_ss << iSeconds;
			double partOfSecond = seconds - iSeconds;

			if(precision > 0)
			{
				// Output fractional part of a second (milliseconds, microseconds)
				int numdigits = std::max(precision, 9);
				numdigits = (numdigits + 2) % 3;
				int val = int(floor(partOfSecond * pow(10., numdigits)));
				_ss.width(numdigits);
				_ss.fill('0');
				_ss << val;
			}
		}

		return _ss;
	}
	//--------------------------------------------------------------------
	StrStream& operator >>(StrStream& _ss, Time& _time)
	{
		if(_ss.fail())
			return _ss;

		// Skip spaces
		int c;
		while((c = _ss.peek()) == ' ')
			_ss.ignore(1);

		int days = 0;
		int hours = 0;
		int minutes = 0;
		int seconds = 0;
		double partOfSecond = 0;

		// Try to read days
		bool daysRead = false;
		{
			String::value_type buf[3];
			size_t startpos = _ss.tellg();
			_ss.get(buf, 3);
			if(_ss.gcount() == 3 && strnicmp(buf, "day", 3) == 0)
			{
				_ss >> days;
				if(_ss.fail())
					return _ss; // An incorrect time representation

				while((c = _ss.peek()) == ' ' || c == ',')
					_ss.ignore(1);

				daysRead = true;
			}
			else
			{
				_ss.clear();
				_ss.seekg(startpos);
			}
		}

		// Try to read time as a single floating-point number of seconds
		double doubleNumber;
		{
			size_t startpos = _ss.tellg();
			_ss >> doubleNumber;
			if(_ss.fail())
			{
				// If the number of days were read before then success 
			    // else fail (because unable to read even one elements of time).
				if(daysRead)
				{
					// success
					_ss.clear(); // clear error state
					_ss.seekg(startpos);
					goto final;
				}
				return _ss;
			}
		}

		// Check if it's a true floating point number, i.e. it cannot be converted to int.
		int intNumber = (int) doubleNumber;
		if( (double) intNumber != doubleNumber)
		{
			// The time is represented by a single floating-point number
			_time.assign(doubleNumber);
			return _ss;
		}

		// Try to read days, try #2
		if(!daysRead)
		{
			while((c = _ss.peek()) == ' ')
				_ss.ignore(1);

			size_t startpos = _ss.tellg();
			String::value_type buf[3];
			_ss.get(buf, 3);
			if(_ss.gcount() == 3 && strnicmp(buf, "day", 3) == 0)
			{
				if((c = _ss.peek()) == 's' || c == 'S') // dayS
					_ss.ignore(1);

				while((c = _ss.peek()) == ' ' || c == ',')
					_ss.ignore(1);

				// days successfully read
				days = intNumber;
				daysRead = true;

				// read the next integer number which possibly is the number of hours
				{
					startpos = _ss.tellg();
					_ss >> intNumber;
					if(_ss.fail())
					{
						// Hours are not specified, return the time 
						// containing the number of days only
						_ss.clear();
						_ss.seekg(startpos);
						goto final;
						return _ss;
					}
				}
			}
			else
			{
				// days are not specified in the input string,
				// it's not error because there can be other elements 
				// in the input string: hours, minutes, seconds.
				_ss.clear();
				_ss.seekg(startpos);
			}
		}

		// Read hours
		hours = intNumber;

		// Read minutes (if specified)
		if(_ss.peek() == ':')
		{
			size_t startpos = _ss.tellg();
			_ss.ignore(1); // skip colon

			// Read minutes
			_ss >> minutes;
			if(_ss.fail())
			{
				// Unable to read minutes
				_ss.clear();
				_ss.seekg(startpos);
				goto final;
			}

			// Read seconds (if specified)
			if(_ss.peek() == ':')
			{
				size_t startpos = _ss.tellg();
				_ss.ignore(1); // skip colon

				// Read seconds
				_ss >> seconds;
				if(_ss.fail())
				{
					// Unable to read seconds
					_ss.clear();
					_ss.seekg(startpos);
					goto final;
				}

				// Read part of second (if specified)
				if(_ss.peek() == ':')
				{
					size_t startpos = _ss.tellg();
					_ss.ignore(1); // skip colon

					// Read 
					int val;
					_ss >> val;
					if(_ss.fail())
					{
						// Unable to read part of seconds
						_ss.clear();
						_ss.seekg(startpos);
						goto final;
					}

					int numdigits = int(_ss.tellg()) - startpos - 1;
					partOfSecond = val * pow(10., -numdigits);
				}
			}
		}

	final:
		// Process negative values
		bool negative = false;
		if(days != 0)
		{
			negative = (days < 0);
		}
		else if(hours != 0)
		{
			negative = (hours < 0);
		}
		else if(minutes != 0)
		{
			negative = (minutes < 0);
		}
		else if(seconds != 0)
		{
			negative = (seconds < 0);
		}

		int sign = negative ? -1 : 1;
		days    = abs(days)    * sign;
		hours   = abs(hours)   * sign;
		minutes = abs(minutes) * sign;
		seconds = abs(seconds) * sign;
		partOfSecond = abs(partOfSecond) * sign;

		// Pack the time value
		_time.pack( days, hours, minutes, seconds + partOfSecond);
		return _ss;
	}

}