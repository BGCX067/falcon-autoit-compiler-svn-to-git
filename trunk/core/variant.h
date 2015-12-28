/**************************************************
 *    HEADER ----
 *    Falcon Variant System - Variable Storage
 *    General purpose variable class
 *    meant for intermediate language
 *    to C++.
 *    Written by Shaggi, 2012. Modified by hyperzap.
 *************************************************/
 
 #ifndef VARIANT_SYS
 #define VARIANT_SYS
 
//Possible variable types.
enum{
    VAR_INTTYPE,
    VAR_DOUBLETYPE,
    VAR_STRTYPE,
    VAR_PTRTYPE,
    VAR_REFTYPE,
    VAR_ARRAYTYPE,
    VAR_RESTYPE
};
 
//Possible Iterative types (only array supported ATM)
#define ARRAY_ARRAYTYPE 0
#define ARRAY_DICTYPE 1
 
class Variant
{
public:
    //Constructors
    Variant(void) : type(VAR_INTTYPE), int64value(0), databuffsize(0), databuff((char*)0) {};
    Variant(int);
    Variant(long long);
    Variant(double);
    Variant(const char* in);
    Variant(char* in);
    Variant(const Variant & in);//Copy Constructor
    Variant(int type, int dims, int,int,int,int,int,int,int,int,int); //Array Constructor
 
    ~Variant();//Destructor
 
     int GetArrayElementCount();
 
    //Operators
    inline Variant& operator= (Variant);
    inline Variant& operator&= (Variant);
    inline Variant& operator+= (Variant);
    inline Variant& operator-= (Variant);
    inline Variant& operator*= (Variant);
    inline Variant& operator/= (Variant);
 
    inline Variant operator+ (Variant);//Addition
    inline Variant operator- (Variant);//Subtraction
    inline Variant operator- ();       //Unary Subtraction
    inline Variant operator* (Variant);//Multiplication
    inline Variant operator/ (Variant);//Division
    inline Variant operator& (Variant);//Concatenation
    void concat_op(char*, char*, int);//concatenation workhorse
    
 
    //Conditional operators
    inline Variant operator== (Variant);
    inline Variant operator!= (Variant);
    inline Variant operator| (Variant);    //Means '=' comparison
    inline Variant operator&& (Variant);     //Logical AND
    inline Variant operator|| (Variant);     //Logical OR
    inline Variant operator! ();         //Logical NOT
    inline Variant operator> (Variant);
    inline Variant operator< (Variant);
    inline Variant operator>= (Variant);
    inline Variant operator<= (Variant);
    inline Variant operator() (int count=0, ...);

    inline Variant& operator[] (int);
 
 
 
    //Inline type conversions
    inline operator bool();
 
    //Explicit type conversions
    char* String(void);
    int Short(void);
    long long Int(void);
    double Double(void);
 


    struct ArrayData
    {
      int num_dimensions;
      int dim_sizes[64];
      long int totalelements;
      Variant** data;
    } ;



    int type;
 
    //Different Datatypes
    union
    {
        long long int64value;
        void* ptr;
        double dvalue;
        Variant* varref;
        ArrayData* arrayaccess;
	  int Resource_id;
    };
 
    char* databuff;
    int databuffsize;
private:
    void StorageReset(void);
    void Cache_StrVal(void);
};
 

Variant var_resolve( std::string attr,  Variant* var);

 
 
 #endif
/**************************************************
* END FALCON Variant System HEADER
**************************************************/
