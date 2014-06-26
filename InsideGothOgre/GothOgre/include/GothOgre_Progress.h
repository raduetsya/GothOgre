#ifndef GOTHOGRE_PROGRESS_H
#define GOTHOGRE_PROGRESS_H

#include "GothOgre_SafeList.h"

namespace GothOgre
{
	class GOTHOGRE_EXPORT Progress : public Singleton<Progress>
	{
	public:
		class Position
		{
		public:
			Position() {}
			Position(float _value) {mValue = _value;}
			Position(double _value) {mValue = _value;}
			Position(int _value) {mValue = (double) _value;}
			Position(size_t _value) {mValue = (double) _value;}
			
			operator double() const {return mValue;}

		private:
			double mValue;
		};
		
		/** Range [min..max]. */
		class Range
		{
		public:
			Range() {}
			Range(Position _min, Position _max) {mMin = _min; mMax = _max;}

			Position getMin() const {return mMin;}
			Position getMax() const {return mMax;}
			void setMin(Position _min) {mMin = _min;}
			void setMax(Position _max) {mMax = _max;}

		private:
			Position mMin, mMax;
		};

	public:
		/** Child operation.
		A pointer to an object of this class is returned by
		function Progress::beginChildOperation. 
		All members are private. */
		class Operation
		{
		private:
			friend class Progress;
			Operation*	mParent;
			Range		mInRange;
			Range		mOutRange;
			String		mMessage;
			String		mCaption;
		};

	public:
		Progress();
		~Progress();

		/** Starts the progress. */
		void begin(bool _redraw = false);

		/** Ends the progress. The starting and ending are counting-based,
		i.e. there must be equal number of calling of functions 
		Progress::begin and Progress::end.
		If the function Progress::begin was called twice,
		then the function Progress::end must be called twice too
		to really end the progress. */
		void end(bool _redraw = false);

		/** Sets the progress bar's range. */
		void setRange(Position _rangeMin, Position _rangeMax, bool _redraw = false);

		/** Sets the progress bar's range. */
		void setRange(const Range& _range, bool _redraw = false);

		/** Sets the progress bar's position. 
		This value must be greater than or equal to progressRangeMin
		and less than or equal to progressRangeMax.
		The progressRangeMin and progressRangeMax are set by
		function Progress::setProgressRange. */
		void setPosition(Position _position, bool _redraw = false);

		/** Returns the "real" progress bar's position.
		This is always in range [0..1]. 
		(The "real" progress bar's position is not the same value
		as set by setPosition, it's always the result of some calculation. */
		double getRealPosition();

		/** Sets the message which can be displayed in the progress bar. */
		void setMessage(const String& _message, bool _redraw = false);

		/** Returns the message which can be displayed in the progress bar. */
		const String& getMessage() const;

		/** Sets the caption which can be displayed in the progress bar. */
		void setCaption(const String& _caption, bool _redraw = false);

		/** Returns the caption which can be displayed in the progress bar. */
		const String& getCaption() const;

		/** Begins a child operation. 
		The child operation is a way to write C++ code
		which works with Progress in the usual way
		(see Progress::setProgressRange, Progress::setProgressPosition, 
		Progress::setMessage) but in fact the C++ code
		will operate not with entire progress bar but with its part only.
		This function allow you to specify the part which must be used
		instead of entire the progress bar.	*/
		Operation* beginChildOperation(Position _useRangeMin, Position _useRangeMax, bool _redraw = false);
		Operation* beginChildOperation(const Range& _useRange, bool _redraw = false);

		/** Ends action which was started by the corresponding 
		call of function Progress::beginChildOperation. */
		void endChildOperation(Operation* _operation, bool _redraw = false);

	private:
		void endCurrentOperation();
		void recalcStartAndCoef();
		void requestRedraw(bool _immediately);
		void redraw();

	private:
		friend class ProgressListener;
		void _initListener(ProgressListener* _listener);
		void _addListener(ProgressListener* _listener);
		void _removeListener(ProgressListener* _listener);

	private:
		int				mBeginCounter;
		Operation*		mRootOperation;
		Operation*		mCurrentOperation;
		Position        mPosition;
		double			mStart;
		double			mCoef;
		bool			mNeedRecalcStartAndCoef;
		double			mRealPosition;
		Timer			mTimer;
		ulong			mWaitRedrawTime;

		typedef SafeList<ProgressListener*> Listeners;
		Listeners      mListeners;
	};

} // namespace GothOgre

#endif // GOTHOGRE_PROGRESS_H