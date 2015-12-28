#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <tchar.h>



/**************************************************
 *	Var.cpp - implementation for the Gen::Var class
 *	General purpose variable class
 *	meant for intermediate language
 *	to C++.
 *	Written by Shaggi, 2012.
 *************************************************/

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <tchar.h>
/**************************************************
 *	Array.h - implementation for the Gen::Array class
 *	General purpose container.
 *	Written by Shaggi, 2012.
 *************************************************/

#pragma once
#include <exception>

namespace Gen {

    template<typename elm>
        class Array
        {
        private:
            typedef elm Element;
            typedef elm * Element_ptr;
            Element_ptr * contents;
        public: typedef size_t size_type;

        private: size_type size;
        public: Array(int sz)
                : size(sz)
            {
                //Build table of non-initialized pointers.
                contents = new Element_ptr[size];
                //Null them out so we can distinguish.
                Nullify();

            }
        public:	~Array() {
                Reset();
            }
        public: Array(const Array<Element> & other) {
                    this->Copy(other);
                }
        public: Array<Element> & operator = (const Array<Element> & other) {
                    if(this == &other)
                        return *this;
                    this->Copy(other);
                    return *this;
                }
        public: bool operator == (const Array<Element> & other) {
                    if(this == &other)
                        return true; //same object, equal
                    if(this->size != other.size)
                        return false; //not same size, not equal
                    for(int i = 0; i < size; ++i) {
                        if(!contents[i] && !other.contents[i])
                            continue; //both empty elements
                        if(contents[i] && other.contents[i]) {
                            if(*contents[i] == *other.contents[i])
                                continue; //both contained objects equal
                        }
                        return false; //fall-through case
                    }
                    return true;//everything checked, ok equal
                }
        private: bool Copy(const Array<Element> & other) {
                    Element_ptr * temp_contents = new Element_ptr[other.size];
                    //Nullify
                    for(int i = 0; i < new_sz; ++i)
                        temp_contents[i] = (Element_ptr)NULL;
                    for(int i = 0; i < size; ++i) {
                        //loop through old array. if any element is filled, copy over
                        if(other.contents[i]) {
                            temp_contents[i] = new Element(*other.contents[i]);
                        }
                    }
                    //Now the old array should be in the new array, and we can safely dispose of the old.
                    Reset();
                    size = other.size;
                    contents = temp_contents;
                    return true;
                 }
        private: void Nullify()
            {
                //Ensure contents are set to null.
                //BEWARE only use this if you are sure the container doesn't
                //have any initialized elements.
                for(int i = 0; i < size; ++i)
                    contents[i] = (Element_ptr)NULL;
            }
        public: inline int UBound()
            {
                return size; //must subtract -1 to get last valid index
            }
        public: void Clean()
            {
                for(int i = 0; i < size; ++i)
                    delete contents[i];
                Nullify();
            }
        public: void Reset()
            {
                for(int i = 0; i < size; ++i)
                    if(contents[i])
                        delete contents[i];
                delete[] contents;
                size = 0;
                contents = NULL;
            }
        public: Element & operator [] (int index) {
                if(index > size)
                    throw std::exception("[Array] Element index out of bounds!"); //Autoit does this.
                if(!contents[index]) {
                    contents[index] = new Element();
                }
                return *contents[index];
            }

        public: bool Redim(int new_sz) {
                if(new_sz == size)
                    return true;
                if(size > new_sz) {
                    //array wanted smaller, preservation of elements impossible.
                    Reset();
                    size = new_sz;
                    //Build table of non-initialized pointers.
                    contents = new Element_ptr[size];
                    //Null them out so we can distinguish.
                    Nullify();
                    return true;
                }
                //array wanted larger. must copy contents over in new array.
                Element_ptr * temp_contents = new Element_ptr[new_sz];
                //Nullify
                for(int i = 0; i < new_sz; ++i)
                    temp_contents[i] = (Element_ptr)NULL;
                for(int i = 0; i < size; ++i) {
                    //loop through old array. if any element is filled, copy over
                    if(contents[i]) {
                        temp_contents[i] = new Element(*contents[i]);
                    }
                }
                //Now the old array should be in the new array, and we can safely dispose of the old.
                Reset();
                size = new_sz;
                contents = temp_contents;
                return true;
            }
        };
}


#if _MSC_VER < 1600 && !defined(_GNUG_)
    #error Gen\Var.h must be compiled with a visual studio compiler higher than or equal to 2010 version, or an GNU gcc compiler higher than version 2.
#else
    #ifdef _GNUG_
        #define _MUTYPEOF(x) typeof(x)
    #elif _MSC_VER
        #define _MUTYPEOF(x) decltype(x)
    #endif
#endif

/*****************************************************
 *	Forward declarations.
 *****************************************************/
char * UnicodeToAnsi(const wchar_t * buf);
wchar_t * AnsiToUnicode(const char * buf);

namespace Gen {
    class Var;
    std::wostream & operator << (std::wostream & o, const Gen::Var & v);
    std::ostream & operator << (std::ostream & o, const Gen::Var & v);
}
/*****************************************************
 *	Macros and ascii + unicode compability
 *****************************************************/
#ifdef _UNICODE
    #define _STDOUT std::wcout
    typedef std::wstring String;
    typedef std::wstringstream sstr;
    #define to_str std::to_wstring
#else
    typedef std::string String;
    #define _STDOUT std::cout
    typedef std::stringstream sstr;
    #define to_str std::to_string
#endif
#ifdef _DEBUG
    #define dbg(x) _STDOUT << _T("debug(): ") <<x << std::endl
    //#define dbg(x) std::cout << "debug("__FILE__ "," __LINE__ "): " <<x << std::endl
#else
    #define dbg(x)
#endif
#define _OUT(x) _STDOUT << x
#define _USE_ORDER //Defines whether the associated array uses a map or a hash
#define _INC_MATHOP //Defines whether to include math operators


namespace Gen {

    #pragma pack(push,1)
    #ifdef _MSC_VER
        #pragma warning(disable : 4482)
    #endif
    /*****************************************************
     *	Defines an casting operator that works with any type.
    *****************************************************/
    template <class Target, class Source>
        Target variable_cast(const Source & source) {
            Target target;
            #ifdef _DEBUG
                std::cout << "\nvariable_cast [ with " << typeid(Source).name() << " source = " << source;
            #endif
            sstr ss;
            ss << std::noskipws << source;
            ss >> target;
            #ifdef _DEBUG
                std::cout << ", " << typeid(Target).name() << " Target = ";
                _OUT(target);
                std::cout << "]\n";
            #endif
            return target;
        }
    /*****************************************************
     *	Defines an casting operator that works with any type,
     *	specialized and optimized for strings.
    *****************************************************/
    template <class Source>
        String variable_cast(const Source & source) {
            String  target;
            #ifdef _DEBUG
                std::cout << "\nvariable_cast [ with " << typeid(Source).name() << " source = " << source;
            #endif
            sstr ss;
            ss << std::noskipws << source;
            target = ss.str();
            #ifdef _DEBUG
                std::cout << ", " << typeid(target).name() << " Target = ";
                _OUT(target);
                std::cout << "]\n";
            #endif
            return target;
        }
        #ifdef _USE_ORDER
            typedef std::map<Gen::Var,Gen::Var> Assoc;
        #else
            typedef std::unordered_map<lol,lol,lol::hash> Assoc;
        #endif
    /*****************************************************
     *	The main class
     *****************************************************/
    class Var
    {
    public:
        #ifdef _UNICODE
            #define _STDSTREAM	std::wostream
            typedef wchar_t char_t;
            #define ATOI		_wtoi64
            #define ITOA		_i64tow
        #else
            #define _STDSTREAM std::ostream
            typedef char char_t;
            #define ATOI		_atoi64
            #define ITOA		_i64toa
        #endif
        /*****************************************************
         *	Working types.
         *****************************************************/
        typedef signed long long Int;
        typedef _MUTYPEOF(INT_MAX-2) NativeInt;
        typedef unsigned long long UInt;
        typedef double Double;
        typedef void * Ptr;
        typedef const char_t * CharArr;
        typedef Var * VarRef;
        typedef Array<Var> VarArray;

        /*****************************************************
         *	Class hash: returns a hash of the variable.
         *****************************************************/
        public: class hash //hashing functor
            {
            public: size_t operator () (const Var & tohash) const //just a wrapper but wont work else.
                {
                    switch(tohash.m_type)
                    {
                    case string_t:
                        return (size_t)std::hash<String>()(*tohash.m_contents.m_string);
                    case int_t:
                        return (size_t)std::hash<Int>()(tohash.m_contents.m_int);
                    case double_t:
                        return (size_t)std::hash<Double>()(tohash.m_contents.m_fpoint);
                    case assoc_t:
                        return (size_t)0;//std::hash< assoc_t >()(*tohash.m_contents.m_map);
                    case ptr_t:
                        return (size_t)std::hash<Ptr>()(tohash.m_contents.m_ptr);
                    }
                    return 0;
                }
            };
        /*****************************************************
         *	Internal types.
         *****************************************************/
        public:	enum type
            {
                int_t = 1,
                double_t,
                string_t,
                assoc_t,
                ptr_t,
                ref_t
            };

        /*****************************************************
         *	Implementation for the data the class holds.
         *****************************************************/
        private: union data
            {
                Int m_int;
                Double m_fpoint;
                Ptr m_ptr;
                VarRef m_ref;
                String
                    * m_string;
                Assoc
                    * m_map;
                VarArray
                    * m_array;
            };
        /*****************************************************
         *	Members of the class.
         *****************************************************/
        data m_contents;
        type m_type;
        bool is_complex;

        /*****************************************************
         *	Constructors for basic types. These allows nearly
         *	everything to be converted into a compatible type
         *	used in this variable.
        *****************************************************/
    public:
        Var(String & input)
            : m_type(type::string_t), is_complex(true)
        {
            Nullify();
            m_contents.m_string = new String(input);
        };

        Var(CharArr input)
            : m_type(type::string_t), is_complex(true)
        {
            Nullify();
            m_contents.m_string = new String(input);
        };

        Var(Int num)
            : m_type(type::int_t), is_complex(false)
        {
            Nullify();
            m_contents.m_int = num;
        };

        Var(NativeInt num)
            : m_type(type::int_t), is_complex(false)
        {
            Nullify();
            m_contents.m_int = num;
        };

        Var(Double num)
            : m_type(type::double_t), is_complex(false)
        {
            Nullify();
            m_contents.m_fpoint = num;
        };
        Var()
            : m_type(type::int_t), is_complex(false)
        {
            Nullify();
        };
        /*****************************************************
         *	Copy assignment.
        *****************************************************/
        Var & operator = (const Gen::Var & source);
        /*****************************************************
         *	The copy constructor that allows the variable to be used
         *	with the STL
        *****************************************************/
        Var(const Gen::Var & source)
            : m_type(source.m_type), is_complex(source.is_complex)
        {
            //if(this == &source) //Can this even happen?
            //	return;
            Nullify();
            switch(m_type)
            {
            case type::assoc_t:
                m_contents.m_map = new Assoc(*source.m_contents.m_map);
                break;
            case type::string_t:
                m_contents.m_string = new String(*source.m_contents.m_string);
                break;
            default:
                m_contents = source.m_contents;
            }
        }
        /*****************************************************
         *	Clears and resets the variable.
         *****************************************************/
        void Clear();
        /*****************************************************
         *	Sets the contents to NULL.
         *****************************************************/
        inline void Nullify() { ::memset(&m_contents,NULL,sizeof(data)); }
        /*****************************************************
         *	Destructor. Keep it simple!
         *****************************************************/
        ~Var() {

            Clear();
        }
        /*****************************************************
         *	Index operators.
        *****************************************************/
        Var & operator [] (Var  value);
        //Var & operator [] (Var & value); ambigious

        /*****************************************************
         *	Operator for retrieving a pointer to the contents.
         *	For strings: Returns a pointer to the char_t array.
         *	Removed for ambiguity.
         *****************************************************
        inline Ptr operator & () const
        {
            std::cout << "PTR operator called";
            if(m_type == type::string_t)
                return (Ptr)m_contents.m_string->c_str();
            return (Ptr)&m_contents;
            //Todo: Return null on map?
        }*/
        /*****************************************************
         *	C++ conversion routines templated. Could be extended
         *	and specialized to desired behavior.
         *****************************************************/
        template <class Type>
            operator Type ()
            {
                switch(this->m_type)
                {
                case int_t:
                    return (Type) (m_contents.m_int);
                case double_t:
                    return (Type) (m_contents.m_fpoint);

                }
                return (Type) 0;
            }
        /*****************************************************
         *	Checks for equality versus another object.
         *****************************************************/
        bool operator == (const Var & right) const;
        /*****************************************************
         *	Checks for less-than another object.
         *****************************************************/
        bool operator < (const Var & right) const;
        #ifdef _INC_MATHOP
            /*******************************************************************************************
             *	this macro declares an inline direct-math function of an binary math operator,
             *	with compound versions of the same. Signed / usigned versions left out due to ambiguity.
             *  Returns 0 in case of left side variable being complex (string, map etc.)
             *******************************************************************************************/
            #define bmath_f(op) inline Int operator op (signed am) { if(!is_complex) return this->m_contents.m_int op am; return 0; }  \
                                inline Int operator op##= (signed am) { if(!is_complex) return this->m_contents.m_int op##= am; return 0;}
            bmath_f(<<);
            bmath_f(>>);
            bmath_f(|);
            bmath_f(^);
            //bmath_f(&); reserved for autoit concation
            bmath_f(*);
            bmath_f(/);
            bmath_f(+);
            bmath_f(-);
            bmath_f(%);

        #undef bmath_f

            UInt inline operator ~ () { return ~ this->m_contents.m_int;}

        #endif
        /*****************************************************
         *	General addition + compound. Should work for any type.
         *****************************************************/
        Var operator + (Var & rhs);
        Var & operator += (Var & hrs);
        /*****************************************************
         *	Returns the string representation of the variable.
         *****************************************************/
        String ToString();
        /*****************************************************
         *	Returns the string representation of the type of the variable
         *****************************************************/
        String GetStringType();
        /*****************************************************
         *	Returns the type
         *****************************************************/
        inline type GetType() { return m_type; }
        /*****************************************************
         *	Returns a hash of the contents.
         *****************************************************/
        inline size_t ToHash() { return hash()(*this); }
        /*****************************************************
         *	Templated String concation using operators & and &=
         *****************************************************/
        template <typename Input>
            Var operator & (Input in)
            {
                if(m_type == string_t) {
                    return *m_contents.m_string + variable_cast(in);
                }
                return *this;
            }
        template <typename Input>
            Var & operator &= (Input in)
            {
                if(m_type == string_t) {
                    m_contents.m_string->operator+=(variable_cast(in));
                }
                return *this;
            }

        /*****************************************************
         *	Specialized and optimized template methods for var => var operations.
         *****************************************************/
        template <>
            Var & operator &= (Var in)
            {
                if(m_type == string_t) {
                    m_contents.m_string->operator+=(in.ToString());
                }
                return *this;
            }
        template <>
            Var operator & (Var & in)
            {
                if(m_type == string_t) {
                    return *m_contents.m_string + in.ToString();
                }
                return *this;
            }
        /*****************************************************
         *	Friends.
         *****************************************************/
        friend std::ostream & operator << (std::ostream &, const Gen::Var &);
        friend std::wostream & operator << (std::wostream &, const Gen::Var &);
        friend class Var::hash;
    };
}
#pragma pack(pop)
#ifdef _MSC_VER
    #pragma warning(default : 4482)
#endif


/**************************************************
 *	Var.cpp - implementation for the Gen::Var class
 *	General purpose variable class
 *	meant for intermediate language
 *	to C++.
 *	Written by Shaggi, 2012.
 *************************************************/
#define _IS_USING_WINDOWS _WIN32
#ifdef _MSC_VER
    #pragma warning(disable : 4482)
#endif
#define MAX_HEX_N 20

namespace Gen {

    /*	void Var::Clear()
        Frees up any content and resets the class to default state (zeroed int)
    */
    void Var::Clear() {
        if(m_type == string_t) {
            /*std::cout <<std::endl<< allocated << " deleting string with this = 0x" << (void*)this << " (" << (void*)m_contents.m_string << "<" << *this << ">)" <<std::endl; */
            delete m_contents.m_string;
        }
        else if(m_type == assoc_t)
            delete m_contents.m_map;
        m_type = int_t;
        is_complex = false;
        Nullify();
    };

    /*	String Var::ToString()
        Returns back a string representation of the content. Not allowed for maps
    */
    String Var::ToString() {
        switch(m_type)
        {
        case string_t:
            return (*m_contents.m_string);
        case int_t:
            return to_str(m_contents.m_int);
        case double_t:
            /*	using variable_cast because doubles can be many hundred chars long lol...	*/
            return variable_cast<String>(m_contents.m_fpoint);
        case ptr_t:
            {
                char_t buf[MAX_HEX_N];
                _stprintf(buf,_T("0x%XI"),m_contents.m_ptr);
                return buf;
            }
        }
        return _T("");
    }
    /*	String Var::GetStringType()
        Returns back a string representation of the type of the variable.
    */
    String Var::GetStringType() {
        switch(m_type)
        {
        case string_t:
            return L"String";
        case int_t:
            return L"Int64";
        case double_t:
            return L"Double";
        case ptr_t:
            return L"Pointer";
        case assoc_t:
            return L"Map";
        }
        return _T("");
    }
    /*	bool Var::operator < (const Var & right) const
        Compares another Gen::Var and evaluates whether it's bigger. Used in strict ordering systems.
    */
    bool Var::operator < (const Var & right) const
    {
        if(this == &right)
            return false;
        if(this->m_type > right.m_type)
            return false;
        if(this->m_type < right.m_type)
            return true;
        switch(m_type)
        {
        case string_t:
            return (*m_contents.m_string) < *right.m_contents.m_string;
        case int_t:
            return (m_contents.m_int) < right.m_contents.m_int;
        case double_t:
            return (m_contents.m_fpoint) < right.m_contents.m_fpoint;
        case assoc_t:
            return (*m_contents.m_map) < *right.m_contents.m_map;
        }
        return false;
    }

    /*	Var & Var::operator [] (Var & value)
        Index operator - takes a reference to a variable and looks it up in the table. The variable is converted to
        a table if it isnt. Returns a reference to the element looked up. (Not really working atm for ambiguity.)

    Var & Var::operator [] (Var & value)
    {
        if(m_type != type::assoc_t) {
            this->Clear();
            m_contents.m_map = new Assoc;
            m_type = type::assoc_t;
        }
        return m_contents.m_map->operator[](value);
    }*/

    /*	Var & Var::operator [] (Var value)
        Index operator - takes a variable and looks it up in the table. The variable is converted to
        a table if it isnt. Returns a reference to the element looked up.
    */
    Var & Var::operator [] (Var value)
    {
        if(m_type != type::assoc_t) {
            this->Clear();
            m_contents.m_map = new Assoc;
            m_type = type::assoc_t;
        }
        return m_contents.m_map->operator[](value);
    }

    /*	Var Var::operator + (Var & hrs)
        General purpose "addition", when arithmetic lookup fails.
    */
    Var Var::operator + (Var & hrs) {
        switch(this->m_type)
        {
            case int_t:
                switch(hrs.m_type)
                {
                    case int_t:
                        return this->m_contents.m_int + hrs.m_contents.m_int;
                    case double_t:
                        return this->m_contents.m_int + hrs.m_contents.m_fpoint;
                    case string_t:
                        // handle eventual conversion here?
                        return this->m_contents.m_int + ATOI(hrs.m_contents.m_string->c_str());
                    case assoc_t:
                        return *this;
                    default:
                        throw std::logic_error("Corrupt right-side addition.");
                }
            case double_t:
                switch(hrs.m_type)
                {
                    case int_t:
                        return this->m_contents.m_fpoint + hrs.m_contents.m_int;
                    case double_t:
                        return this->m_contents.m_fpoint + hrs.m_contents.m_fpoint;
                    case string_t:
                        // handle eventual conversion here?
                        return this->m_contents.m_fpoint + ATOI(hrs.m_contents.m_string->c_str());
                    case assoc_t:
                        return *this;
                    default:
                        throw std::logic_error("Corrupt right-side addition.");
                }
            case string_t:
                switch(hrs.m_type)
                {
                    case int_t:
                        {
                            Gen::Var::char_t temp_string[40];
                            ITOA(hrs.m_contents.m_int, temp_string, 10);
                            return *this->m_contents.m_string + temp_string;
                        }
                    case double_t:
                        return *this->m_contents.m_string;
                    case string_t:
                        // handle eventual conversion here?
                        return *this->m_contents.m_string + *hrs.m_contents.m_string;
                    case assoc_t:
                        return *this;
                    default:
                        throw std::logic_error("Corrupt right-side addition.");
                }
        }
                        throw std::logic_error("Corrupt left-side addition.");
    }

    /*	Var & Var::operator = (const Gen::Var & source)
        General purpose assignment, when no constructor suits.
    */
    Var & Var::operator = (const Gen::Var & source)
    {
        Clear();
        m_type = source.m_type;
        is_complex = source.is_complex;
        switch(m_type)
        {
        case type::assoc_t:
            m_contents.m_map = new Assoc(*source.m_contents.m_map);
            break;
        case type::string_t:
            m_contents.m_string = new String(*source.m_contents.m_string);
            break;
        default:
            m_contents = source.m_contents;
        }
        return *this;
    }
    /*	std::ostream & operator << (std::ostream & o, const Gen::Var & v)
        Inserts formatted string data from the variable into the stream
    */
    std::ostream & operator << (std::ostream & o, const Gen::Var & v) {
        switch(v.m_type)
        {
        case Gen::Var::type::string_t:
            {
                #ifdef _UNICODE
                    char * _sts = UnicodeToAnsi(v.m_contents.m_string->c_str());
                    if(_sts) {
                        o << _sts;
                        delete[] _sts;
                    }
                    return o;
                #else
                    return o << *(v.m_contents.m_string);
                #endif
            }
        case Gen::Var::type::int_t:
            return o << v.m_contents.m_int;
        case Gen::Var::type::double_t:
            return o << v.m_contents.m_fpoint;
        case Gen::Var::type::ptr_t:
            return o << "0x" << std::hex << (void*) v.m_contents.m_ptr << std::dec;
        case Gen::Var::type::assoc_t:
            return o;
        }
        return o /*<< _T("<!ERROR>\n")*/;
    }
    /*	std::ostream & operator << (std::ostream & o, const Gen::Var & v)
        Inserts formatted string data from the variable into the stream
    */
    std::wostream & operator << (std::wostream & o, const Gen::Var & v) {
        switch(v.m_type)
        {
        case Gen::Var::type::string_t:
            #ifndef _UNICODE
                wchar_t * _sts = AnsiToUnicode(v.m_contents.m_string->c_str());
                if(_sts) {
                    o << _sts;
                    delete[] _sts;
                }
                return o;
            #else
                return o << *(v.m_contents.m_string);
            #endif
        case Gen::Var::type::int_t:
            return o << v.m_contents.m_int;
        case Gen::Var::type::double_t:
            return o << v.m_contents.m_fpoint;
        case Gen::Var::type::ptr_t:
            return o << L"0x" << std::hex << (void*) v.m_contents.m_ptr << std::dec;
        case Gen::Var::type::assoc_t:
            return o;
        }
        return o << _T("<!ERROR>\n");
    }
    /*	bool Var::operator == (const Var & right) const
        Compares two Var objects.
    */
    bool Var::operator == (const Var & right) const
        {
            //check if same object
            if(this == &right)
                return true;
            //check if same type. this operator == acts like operator ===
            if(this->m_type != right.m_type)
                return false;

            switch(m_type)
            {
            case string_t:
                return (*m_contents.m_string) == *right.m_contents.m_string;
            case int_t:
                return (m_contents.m_int) == right.m_contents.m_int;
            case double_t:
                return (m_contents.m_fpoint) == right.m_contents.m_fpoint;
            case assoc_t:
                return (*m_contents.m_map) == *right.m_contents.m_map;
            }

            return false;
        }
};

/*	char * UnicodeToAnsi(const wchar_t * buf)
    If successfull, returns a char array corrosponding to the wchar array. On failure, returns null.
    Caller's responsibility to clean up!
*/
char * UnicodeToAnsi(const wchar_t * buf) {
    if(!buf)
        return NULL;
    char * res;
    size_t len = wcslen(buf);
    res = new char[len + 1];
    #ifdef _IS_USING_WINDOWS
        WideCharToMultiByte(CP_ACP, 0, buf, -1, res, len + 1, "?", NULL);
    #else
        wcstombs(res,buf,len + 1);
    #endif
    return res;
}
/*	wchar_t * AnsiToUnicode(const char * buf)
    If successfull, returns a wchar array corrosponding to the char array. On failure, returns null.
    Caller's responsibility to clean up!
*/
wchar_t * AnsiToUnicode(const char * buf) {
    if(!buf)
        return NULL;
    wchar_t * res;
    size_t len = strlen(buf);
    res = new wchar_t[len + 1];
    #ifdef _IS_USING_WINDOWS
        MultiByteToWideChar(CP_ACP, 0, buf, -1, res, len + 1);
    #else
        mbstowcs(res,buf,len + 1);
    #endif
    return res;
}
#ifdef _MSC_VER
    #pragma warning(default : 4482)
#endif



int main(int argc, char* argv[]){
    COOL = ("This is the way");
    HACK = (((5*6)+3));
    FLOAT = ((555.342+(3*(5+7))));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    MATE = ((-2));
    MATE = ((-2));
    MATE = ((-2));
    MATE = ((-2));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));
    TEST = ((((((9-4)*TEST)+(((5/6)*(9-4))*TEST))+(((5/6)*(9-4))*TEST))+(5/6)));


}

