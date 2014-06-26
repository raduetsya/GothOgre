#ifndef GOTHOGRE_ANY_LITE_H
#define GOTHOGRE_ANY_LITE_H


namespace GothOgre
{
	/** Non-printable version of Ogre::Any, i.e.
	it cannot be written to a stream, and it doesn't require 
	operator << to be defined for used types.
	AnyLite doesn't define own member variables and can be converted 
	in both direction easily (Any->AnyLite, AnyLite->Any). 
	*/
	class AnyLite : public Any
	{
	public: // constructors
        AnyLite()
        {
        }

        template<typename ValueType>
        explicit AnyLite(const ValueType& _value)
        {
			// at final initialise the content
			mContent = (OGRE_NEW_T(holderNP<ValueType>, MEMCATEGORY_GENERAL)(_value));
        }

        AnyLite(const Any& _other)
			: Any(_other)
        {
        }
        
	public: // assignment
		template<typename ValueType>
        AnyLite& operator=(const ValueType& _rhs)
        {
            AnyLite(_rhs).swap(*this);
            return *this;
        }

        AnyLite& operator=(const Any& _rhs)
        {
            AnyLite(_rhs).swap(*this);
            return *this;
        }

	protected:
		template<typename ValueType>
		class holderNP : public baseholder<ValueType>
		{
        public: // structors
            
			holderNP(const ValueType & value)
              : baseholder<ValueType>(value)
            {
            }

        public: // queries

            virtual const std::type_info & getType() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return OGRE_NEW_T(holderNP, MEMCATEGORY_GENERAL)(held);
            }

			virtual void writeToStream(std::ostream& o)
			{
			}
        };
	};


	//---------------------------------------------------------------------
	/* Details:
	The Ogre::Any class is useful, but has a small drawback:
	it requires operator << to be defined for all used type.

	Example:
	struct MyStruct
	{
		int a;
		int b;
	};

	MyStruct tmp;
	Any any;
	any = (Any) tmp;   // Compiler error: operator << doesn't exist!
	any = (AnyLite) tmp; // OK

	*/

} // namespace GothOgre

#endif // GOTHOGRE_ANY_NP_H