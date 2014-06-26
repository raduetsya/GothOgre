#ifndef GOTHOGRE_TIME_H
#define GOTHOGRE_TIME_H

#include "GothOgre_StrStream.h"

namespace GothOgre
{
	// ¬рем€ вообще, без прив€зки к конкретным событи€м
	class GOTHOGRE_EXPORT Time
	{
	public:
		Time() : mTotalSeconds(0) {}
		explicit Time(double _totalSeconds) : mTotalSeconds(_totalSeconds) {}
		Time(int _days, int _hours, int _minutes, double _seconds) {pack(_days, _hours, _minutes, _seconds);}
		Time(const Time& _src) : mTotalSeconds(_src.mTotalSeconds) {}

		const Time& operator =(const Time& _src) {mTotalSeconds = _src.mTotalSeconds; return *this;}
		const Time& operator =(double _src) {mTotalSeconds = _src; return *this;}
		const Time& operator =(float _src) {mTotalSeconds = _src; return *this;}

		const Time& operator +=(const Time& _b) {mTotalSeconds += _b.mTotalSeconds; return *this;}
		const Time& operator +=(double _b) {mTotalSeconds += _b; return *this;}
		const Time& operator +=(float _b) {mTotalSeconds += _b; return *this;}
		friend Time operator +(const Time& _a, const Time& _b) {return Time( (double)_a + (double)_b );}
		friend Time operator +(const Time& _a, double _b) {return Time( (double)_a + (double)_b );}
		friend Time operator +(const Time& _a, float _b) {return Time( (double)_a + (double)_b );}
		friend Time operator +(double _a, const Time& _b) {return Time( (double)_a + (double)_b );}
		friend Time operator +(float _a, const Time& _b) {return Time( (double)_a + (double)_b );}

		const Time& operator -=(const Time& _b) {mTotalSeconds -= _b.mTotalSeconds; return *this;}
		const Time& operator -=(double _b) {mTotalSeconds -= _b; return *this;}
		const Time& operator -=(float _b) {mTotalSeconds -= _b; return *this;}
		friend Time operator -(const Time& _a, const Time& _b) {return Time( (double)_a - (double)_b );}
		friend Time operator -(const Time& _a, double _b) {return Time( (double)_a - (double)_b );}
		friend Time operator -(const Time& _a, float _b) {return Time( (double)_a - (double)_b );}
		friend Time operator -(double _a, const Time& _b) {return Time( (double)_a - (double)_b );}
		friend Time operator -(float _a, const Time& _b) {return Time( (double)_a - (double)_b );}

		const Time& operator *=(double _f) {mTotalSeconds = mTotalSeconds * _f; return *this;}
		const Time& operator *=(float _f) {mTotalSeconds = mTotalSeconds * _f; return *this;}
		friend Time operator *(const Time& _a, double _b) {return Time( (double)_a * _b );}
		friend Time operator *(const Time& _a, float _b) {return Time( (double)_a * _b );}
		friend Time operator *(double _a, const Time& _b) {return Time( _a * (double)_b );}
		friend Time operator *(float _a, const Time& _b) {return Time( _a * (double)_b );}

		const Time& operator /=(double _f) {mTotalSeconds = mTotalSeconds / _f; return *this;}
		const Time& operator /=(float _f) {mTotalSeconds = mTotalSeconds / _f; return *this;}
		friend Time operator /(const Time& _a, double _b) {return Time( (double)_a / _b );}
		friend Time operator /(const Time& _a, float _b) {return Time( (double)_a / _b );}
		
		// double with implicit conversion to float.
		friend Real operator /(const Time& _a, const Time& _b) {return Real((double)_a / (double)_b);}

		friend bool operator <(const Time& _a, const Time& _b)  {return (double) _a < (double)_b;}
		friend bool operator >(const Time& _a, const Time& _b)  {return (double) _a > (double)_b;}
		friend bool operator <=(const Time& _a, const Time& _b) {return (double) _a <= (double)_b;}
		friend bool operator >=(const Time& _a, const Time& _b) {return (double) _a >= (double)_b;}
		friend bool operator ==(const Time& _a, const Time& _b) {return (double) _a == (double)_b;}
		friend bool operator !=(const Time& _a, const Time& _b) {return (double) _a != (double)_b;}

		operator double() const {return mTotalSeconds;}

		void assign(double _totalSeconds) {mTotalSeconds = _totalSeconds;}
		void assign(int _days, int _hours, int _minutes, double _seconds) {pack(_days, _hours, _minutes, _seconds);}

		int getDays() const;
		int getHours() const;
		int getMinutes() const;
		double getSeconds() const;

		int getDay() const {return getDays();}
		int getHour() const {return getHours();}
		int getMinute() const {return getMinutes();}
		double getSecond() const {return getSeconds();}

		void pack(int _days, int _hours, int _minutes, double _seconds);
		void unpack(int& _days, int& _hours, int& _minutes, double& _seconds) const;

		void setDays(int _days);
		void setHours(int _hours);
		void setMinutes(int _minutes);
		void setSeconds(double _seconds);

		void setDay(int _day) {setDays(_day);}
		void setHour(int _hour) {setHours(_hour);}
		void setMinute(int _minute) {setMinutes(_minute);}
		void setSecond(double _second) {setSeconds(_second);}

		static const Time ZERO;
		static const Time ONE_SECOND;
		static const Time ONE_MINUTE;
		static const Time ONE_HOUR;
		static const Time ONE_DAY;
		static const Time INFINITY;

	private:
		// врем€ хранитс€ в виде вещественного числа, означающего число секунд,
		// плюсы - очень быстрое копирование, сравнение и вычитание объектов класса Time,
		// минусы - медленна€ работа с компонентами времени - функции setDay, getDay, setHour, getHour и т.д.
		// тип float:
		// Epsilon = 1.1929093e-7. (Epsilon is the smallest x such that 1+x > 1.)
		// ѕоэтому погрешность на 100-ом дне игры равна
		// 100*24*3600*Epsilon = 1.0307 секунд, то есть больше секунды - слишком много.
		// поэтому тип double:
		// Epsilon = 2.220446049250313e-16.
		// ѕоэтому погрешность на 100-ом дне игры равна
		// 100*24*3600*Epsilon = 1.9185e-9 секунд, то есть пор€дка наносекунды.
		double mTotalSeconds;
	};

	/** Prints a time to a string.
	If days == 0 then days are not outputed.
	The standard "precision" manipulator of iostream is used to determine 
	how many digits the function must output after seconds.
	More details:
	precision            |  output                   | precision
	< 0                  |  Day 1, 9:26              | minutes
	0                    |  Day 1, 9:26:31           | seconds
	1 or 2 or 3          |  Day 1, 9:26:31:008       | milliseconds
	4 or 5 or 6(default) |  Day 1, 9:26:31:008945    | microseconds
	7 or 8 or 9          |  Day 1, 9:26:31:008945021 | nanoseconds.
	*/
	GOTHOGRE_EXPORT StrStream& operator <<(StrStream& _ss, const Time& _time);

	/** Gets a time from a string. */
	GOTHOGRE_EXPORT StrStream& operator >>(StrStream& _ss, Time& _time);

	//----------------------------------------------------------------------
	inline void Time::pack(int _days, int _hours, int _minutes, double _seconds)
	{
		mTotalSeconds = _days * 86400. + _hours * 3600. + _minutes * 60. + _seconds;
	}
	//----------------------------------------------------------------------
	inline int Time::getDays() const 
	{
		int iTotalDays = int(floor(mTotalSeconds)) / 86400;
		return iTotalDays;
	}
	//----------------------------------------------------------------------
	inline int Time::getHours() const 
	{
		int iTotalHours = int(floor(mTotalSeconds)) / 3600;
		return iTotalHours % 24;
	}
	//----------------------------------------------------------------------
	inline int Time::getMinutes() const 
	{
		int iTotalMinutes = int(floor(mTotalSeconds)) / 60;
		return iTotalMinutes % 60;
	}
	//----------------------------------------------------------------------
	inline double Time::getSeconds() const 
	{
		int iTotalMinutes = int(floor(mTotalSeconds)) / 60;
		return mTotalSeconds - iTotalMinutes * 60;
	}
	//----------------------------------------------------------------------
	inline void Time::setDays(int _days)
	{
		int days, hours, minutes;
		double seconds;
		unpack(days, hours, minutes, seconds);
		days = _days;
		pack(days, hours, minutes, seconds);
	}
	//----------------------------------------------------------------------
	inline void Time::setHours(int _hours)
	{
		int days, hours, minutes;
		double seconds;
		unpack(days, hours, minutes, seconds);
		hours = _hours;
		pack(days, hours, minutes, seconds);
	}
	//----------------------------------------------------------------------
	inline void Time::setMinutes(int _minutes)
	{
		int days, hours, minutes;
		double seconds;
		unpack(days, hours, minutes, seconds);
		minutes = _minutes;
		pack(days, hours, minutes, seconds);
	}
	//----------------------------------------------------------------------
	inline void Time::setSeconds(double _seconds)
	{
		int days, hours, minutes;
		double seconds;
		unpack(days, hours, minutes, seconds);
		seconds = _seconds;
		pack(days, hours, minutes, seconds);
	}
}

#endif // GOTHOGRE_TIME_H