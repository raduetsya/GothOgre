#include "GothOgre_Precompiled.h"
#include "GothOgre_Progress.h"
#include "GothOgre_ProgressListener.h"

//-------------------------------------------------------------------------
GothOgre::Progress* 
	Ogre::Singleton<GothOgre::Progress>::ms_Singleton 
	= nullptr;
//-------------------------------------------------------------------------

namespace GothOgre
{
	Progress::Progress()
	{
		mBeginCounter	= 0;
		mRootOperation = new Operation;
		mRootOperation->mParent = nullptr;
		mRootOperation->mInRange.setMin(0);
		mRootOperation->mInRange.setMax(1);
		mRootOperation->mOutRange.setMin(0);
		mRootOperation->mOutRange.setMax(1);
		mRootOperation->mCaption = "Processing...";
		mRootOperation->mMessage = "";
		mCurrentOperation = mRootOperation;
		mNeedRecalcStartAndCoef = true;
		mWaitRedrawTime = 0;
	}
	//-------------------------------------------------------------------------
	Progress::~Progress()
	{
		Operation* op = mCurrentOperation;
		do
		{
			Operation* parent = op->mParent;
			delete op;
			op = parent;
		}
		while(op != nullptr);
	}
	//-------------------------------------------------------------------------
	void Progress::begin(bool _redraw)
	{
		if(++mBeginCounter == 1)
		{
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				it.getNext()->progressStarted();
			}
			requestRedraw(_redraw);
		}
	}
	//-------------------------------------------------------------------------
	void Progress::end(bool _redraw)
	{
		if(--mBeginCounter == 0)
		{
			Listeners::Iterator it = mListeners.getIterator();
			while(it.hasMoreElements())
			{
				it.getNext()->progressEnded();
			}
			requestRedraw(_redraw);
		}
	}
	//-------------------------------------------------------------------------
	Progress::Operation* Progress::beginChildOperation(Position _useRangeMin, Position _useRangeMax, bool _redraw)
	{
		return beginChildOperation(Range(_useRangeMin, _useRangeMax), _redraw);
	}
	//-------------------------------------------------------------------------
	Progress::Operation* Progress::beginChildOperation(const Range& _useRange, bool _redraw)
	{
		Operation* childOp = new Operation;
		childOp->mParent = mCurrentOperation;
		childOp->mOutRange = _useRange;
		childOp->mInRange.setMin(0);
		childOp->mInRange.setMax(1);
		childOp->mCaption = mCurrentOperation->mCaption;
		childOp->mMessage = mCurrentOperation->mMessage;
		mCurrentOperation = childOp;
		mNeedRecalcStartAndCoef = true;
		requestRedraw(_redraw);
		return childOp;
	}
	//-------------------------------------------------------------------------
	void Progress::endCurrentOperation()
	{
		Operation* op = mCurrentOperation;
		mCurrentOperation = op->mParent;
		mNeedRecalcStartAndCoef = true;
		delete op;
	}
	//-------------------------------------------------------------------------
	void Progress::endChildOperation(Operation* _operation, bool _redraw)
	{
		bool found = false;
		while(!found)
		{
			found = (mCurrentOperation == _operation);
			endCurrentOperation();
		}
		requestRedraw(_redraw);
	}
	//-------------------------------------------------------------------------
	void Progress::setMessage(const String& _message, bool _redraw)
	{
		mCurrentOperation->mMessage = _message;
		requestRedraw(_redraw);
	}
	//-------------------------------------------------------------------------
	const String& Progress::getMessage() const
	{
		return mCurrentOperation->mMessage;
	}
	//-------------------------------------------------------------------------
	void Progress::setCaption(const String& _caption, bool _redraw)
	{
		mCurrentOperation->mCaption = _caption;
		requestRedraw(_redraw);
	}
	//-------------------------------------------------------------------------
	const String& Progress::getCaption() const
	{
		return mCurrentOperation->mCaption;
	}
	//-------------------------------------------------------------------------
	void Progress::setRange(Position _rangeMin, Position _rangeMax, bool _redraw)
	{
		setRange(Range(_rangeMin, _rangeMax), _redraw);
	}
	//-------------------------------------------------------------------------
	void Progress::setRange(const Range& _range, bool _redraw)
	{
		mCurrentOperation->mInRange = _range;
		mNeedRecalcStartAndCoef = true;
		mPosition = _range.getMin();
		requestRedraw(_redraw);
	}
	//-------------------------------------------------------------------------
	void Progress::setPosition(Position _position, bool _redraw)
	{
		mPosition = _position;
		requestRedraw(_redraw);
	}
	//-------------------------------------------------------------------------
	double Progress::getRealPosition()
	{
		// calculate real position in range [0..1]
		recalcStartAndCoef();
		mRealPosition = mPosition * mCoef + mStart;
		return mRealPosition;
	}
	//-------------------------------------------------------------------------
	void Progress::requestRedraw(bool _immediately)
	{
		if(_immediately)
		{
			redraw();
		}
		else
		{
			// if elapsed time < mUpdateTime then skip the following update
			// to prevent from wasting most of processor time 
			// for updating the progress bar
			ulong msec = mTimer.getMilliseconds();
			if(msec >= mWaitRedrawTime)
			{
				redraw();
			}
		}
	}
	//-------------------------------------------------------------------------
	void Progress::redraw()
	{
		mTimer.reset();
		
		// update each render target
		Ogre::Root::getSingleton().renderOneFrame();

		ulong redrawTime = mTimer.getMilliseconds();
		mTimer.reset();
		mWaitRedrawTime = std::max<ulong>(redrawTime * 10, 200);
	}
	//-------------------------------------------------------------------------
	void Progress::recalcStartAndCoef()
	{
		if(!mNeedRecalcStartAndCoef)
			return;
	
		// These two variables will get the result
		mStart = 0;
		mCoef = 1;

		Operation* op = mCurrentOperation;
		while(op)
		{
			double inmin  = op->mInRange.getMin();
			double inmax  = op->mInRange.getMax();
			double outmin = op->mOutRange.getMin();
			double outmax = op->mOutRange.getMax();

			// Finding function g(x) to map interval [inmin..inmax] to [outmin..outmax]:
			//
			//                 g(x)
			// [inmin..inmax] -----> [outmin..outmax]
			//
			// We can define g(x) as following:
			//
			//           x - inmin
			// g(x) = --------------- * (outmax - outmin) + outmin
			//         inmax - inmin
			//
			// More usable expression for g(x):
			// g(x) = x * coef2 + start2, where
			// 
			//          outmax - outmin              -inmin * (outmax - outmin)
			// coef2 = ----------------- , start2 = ---------------------------- + outmin
			//           inmax - inmin                     inmax - inmin
			// 
			double coef2 = (outmax - outmin) / (inmax - inmin);
			double start2 = -inmin * coef2 + outmin;

			// Let f(x) = x * mCoef + mStart, g(x) = x * coef2 + start2, then 
			// g(f(x)) = (x * mCoef + mStart) * coef2 + start2
			// = x * (mCoef * coef2) + (mStart * coef2 + start2)
			mStart = mStart * coef2 + start2;
			mCoef *= coef2;
			op = op->mParent;
		}
		mNeedRecalcStartAndCoef = false;
	}
	//-------------------------------------------------------------------------
	void Progress::_initListener(ProgressListener* _listener)
	{
		_listener->mElementHandle = mListeners.getNullHandle();
	}
	//-------------------------------------------------------------------------
	void Progress::_addListener(ProgressListener* _listener)
	{
		if(_listener->mElementHandle == mListeners.getNullHandle())
		{
			_listener->mElementHandle = mListeners.add(_listener);
		}
	}
	//-------------------------------------------------------------------------
	void Progress::_removeListener(ProgressListener* _listener)
	{
		if(_listener->mElementHandle != mListeners.getNullHandle())
		{
			mListeners.remove(_listener->mElementHandle);
			_listener->mElementHandle = mListeners.getNullHandle();
		}
	}

} // namespace GothOgre