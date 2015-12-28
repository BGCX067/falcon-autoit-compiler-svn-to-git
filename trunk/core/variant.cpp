/**************************************************
 *    Falcon Variant System - Variable Storage
 *    General purpose variable class
 *    meant for intermediate language
 *    to C++.
 *    Written by Shaggi, 2012. Modified by hyperzap.
 *************************************************/
 
//File assumes that variant.h has already be appended to the filestream previously.
 
 
Variant var_resolve( std::string attr,  Variant* var)
{
	return Variant(attr.c_str());
}
 
 
 
//Constructors
inline Variant::Variant(int in)
{
    type = VAR_INTTYPE;
    int64value = in;
    databuff = (char*)0;
}
inline Variant::Variant(long long in)
{
    type = VAR_INTTYPE;
    int64value = in;
    databuff = (char*)0;
}
Variant::Variant(double in)
{
    type = VAR_DOUBLETYPE;
    dvalue = in;
    databuff = (char*)0;
}
inline Variant::Variant(const char* in)
{
    type = VAR_STRTYPE;
    databuff = (char*)in;
    databuffsize = 0;
}
Variant::Variant(char* in)
{
    type = VAR_STRTYPE;
    int len = strlen(in);
    databuff = new char[len+1];
    strcpy(databuff, in);
    databuffsize = len+1;
}
//Copy Constructor - DESTRUCTOR ALREADY CALLED THEREFORE NO NEED TO FREE
Variant::Variant(const Variant & in)
{
    _DEBUG_MSG( "Copy constructor called" );

    int len;
    databuff = (char*)0;
    switch(in.type)
    {
    case VAR_INTTYPE:        //Int
        type = VAR_INTTYPE;
        int64value = in.int64value;
        break;
    case VAR_DOUBLETYPE:    //decimal
        type = VAR_DOUBLETYPE;
        dvalue = in.dvalue;
        break;
    case VAR_STRTYPE:        //String
        type = VAR_STRTYPE;
        len = strlen(in.databuff);
        databuff = new char[len+1];
        strcpy(databuff, in.databuff);
        databuffsize = len+1;
        break;

    case VAR_ARRAYTYPE:
      this->type = VAR_ARRAYTYPE;
	  this->arrayaccess = new ArrayData;
	  this->arrayaccess->totalelements = in.arrayaccess->totalelements;
	  this->arrayaccess->num_dimensions = in.arrayaccess->num_dimensions;
	  len = 0;
	  for(;len<this->arrayaccess->num_dimensions;len++)
		this->arrayaccess->dim_sizes[len] = in.arrayaccess->dim_sizes[len];
	  _DEBUG_MSG("Copying Array: Dimension data copied.");
	  this->arrayaccess->data = new Variant*[this->arrayaccess->totalelements];
	  _DEBUG_MSG("Copying Array: Pointer array allocated.");
	  for(len=0;len<this->arrayaccess->totalelements;len++){
	    if(this->arrayaccess->data[len] != (Variant*)0xCDCDCDCD)
		  this->arrayaccess->data[len] = new Variant((*in.arrayaccess->data[len]));
		else
		  this->arrayaccess->data[len] = 0;
	  }

	  _DEBUG_MSG( "Copying Array: Element variables constructed.");
	  break;

    case VAR_RESTYPE:
	this->type = VAR_RESTYPE;
	this->Resource_id = in.Resource_id;
	break;

    default:                //Other - Cannot copy.
        type = VAR_INTTYPE;
        int64value = 0;
    }
 
	_DEBUG_MSG( "Leaving Copy Constructor.");
}
 
 
 
 
 
//Conditional operators
 
Variant Variant::operator> (Variant in){
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(strcmp(this->String(), in.String()) > 0)
            return Variant(1);
        else return Variant(0);
        break;
    case VAR_INTTYPE: case VAR_DOUBLETYPE:
        if(this->Double() > in.Double())
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return Variant(0);
    };
}
Variant Variant::operator< (Variant in){
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(strcmp(this->String(), in.String()) < 0)
            return Variant(1);
        else return Variant(0);
        break;
    case VAR_INTTYPE: case VAR_DOUBLETYPE:
        if(this->Double() < in.Double())
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return Variant(0);
    };
}
Variant Variant::operator>= (Variant in){
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(strcmp(this->String(), in.String()) >= 0)
            return Variant(1);
        else return Variant(0);
        break;
    case VAR_INTTYPE: case VAR_DOUBLETYPE:
        if(this->Double() >= in.Double())
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return Variant(0);
    };
}
Variant Variant::operator<= (Variant in){
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(strcmp(this->String(), in.String()) <= 0)
            return Variant(1);
        else return Variant(0);
        break;
    case VAR_INTTYPE: case VAR_DOUBLETYPE:
        if(this->Double() <= in.Double())
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return Variant(0);
    };
}
 
 
 
Variant Variant::operator== (Variant in){
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(strcmp(this->String(), in.String()) == 0)
            return Variant(1);
        else return Variant(0);
        break;
    case VAR_INTTYPE: case VAR_DOUBLETYPE:
        if(this->Double() == in.Double())
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return Variant(0);
    };
}
Variant Variant::operator| (Variant in){    //Conditional '=' operator
    switch(this->type)
    {
    case VAR_STRTYPE:
        if(stricmp(this->String(), in.String()) == 0)
            return Variant(1);
        else return Variant(0);
        break;
    default:
        return this->operator==(in);
    };
}
 
 
Variant Variant::operator!= (Variant in){    //Conditional '=' operator
        return !(this->operator|(in));
}
 
 
Variant Variant::operator&& (Variant in){
    if(!(*this))return Variant(0);
    if(!in)return Variant(0);
    return Variant(1);
}
Variant Variant::operator|| (Variant in){
    if(*this)return Variant(1);
    if(in)return Variant(1);
    return Variant(0);
}
 
Variant Variant::operator! (){
    return Variant(!((bool)*this));
}
 
 
 
//Assignment operator
Variant& Variant::operator= (Variant in){//Remember to update copy constructor with near duplicate!
                                        //But without the StorageReset() as copy calls destructor.


	//Destroy our own data in prep for assignment
    this->StorageReset();
 
    _DEBUG_MSG("Assignment Operation:" );

    int len;
    switch(in.type)
    {
    case VAR_INTTYPE:        //Int
        type = VAR_INTTYPE;
        int64value = in.int64value;
        break;
    case VAR_DOUBLETYPE:    //decimal
        type = VAR_DOUBLETYPE;
        dvalue = in.dvalue;
        break;
    case VAR_STRTYPE:        //String
        type = VAR_STRTYPE;
        len = strlen(in.databuff);
        databuff = new char[len+1];
        strcpy(databuff, in.databuff);
        databuffsize = len+1;
        break;

    case VAR_ARRAYTYPE:
      this->type = VAR_ARRAYTYPE;
	  this->arrayaccess = new ArrayData;
	  this->arrayaccess->totalelements = in.arrayaccess->totalelements;
	  this->arrayaccess->num_dimensions = in.arrayaccess->num_dimensions;
	  len = 0;
	  for(;len<this->arrayaccess->num_dimensions;len++)
		this->arrayaccess->dim_sizes[len] = in.arrayaccess->dim_sizes[len];
	  _DEBUG_MSG( "Copying Array: Dimension data copied.");
	  this->arrayaccess->data = new Variant*[this->arrayaccess->totalelements];
	  _DEBUG_MSG( "Copying Array: Pointer array allocated.");
	  for(len=0;len<this->arrayaccess->totalelements;len++){
	    if((this->arrayaccess->data[len] != (Variant*)0xCDCDCDCD) && 
		(this->arrayaccess->data[len] != (Variant*)0x2a)){
		  this->arrayaccess->data[len] = new Variant((*in.arrayaccess->data[len]));
		}else
		  this->arrayaccess->data[len] = 0;
	  }
	  _DEBUG_MSG( "Copying Array: Element variables constructed.");
	  break;

    case VAR_RESTYPE:
	this->type = VAR_RESTYPE;
	this->Resource_id = in.Resource_id;
	break;

    default:                //Other - Cannot copy.
        type = VAR_INTTYPE;
        int64value = 0;
    }
    _DEBUG_MSG("Leaving Assignment handler.");
    return *this;
}
 





















Variant& Variant::operator&= (Variant in){
    int len = strlen(this->String()) + strlen(in.String()) + 1;
    this->concat_op(this->String(), in.String(), len);
    return *this;
}
Variant& Variant::operator+= (Variant in){
    *this = Variant(*this + in);
    return *this;
}
Variant& Variant::operator-= (Variant in){
    *this = Variant(*this - in);
    return *this;
}
Variant& Variant::operator*= (Variant in){
    *this = Variant(*this * in);
    return *this;
}
Variant& Variant::operator/= (Variant in){
    *this = Variant(*this / in);
    return *this;
}
 
 
//Operators
Variant Variant::operator+ (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value+in.int64value));
    else{
    long long outl = (long long)(this->Double() + in.Double());
    double outf = this->Double() + in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() + in.Double()));
    else
        return Variant(this->Double() + in.Double());
    }
}
 
Variant Variant::operator- (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value-in.int64value));
    else{
    long long outl = (long long)(this->Double() - in.Double());
    double outf = this->Double() - in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() - in.Double()));
    else
        return Variant(this->Double() - in.Double());
    }
}
 
Variant Variant::operator- (){
    if(type==VAR_INTTYPE)
        return Variant((long long)(-int64value));
    else return Variant(-this->Double());
}
 
Variant Variant::operator* (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value*in.int64value));
    else{
    long long outl = (long long)(this->Double() * in.Double());
    double outf = this->Double() * in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() * in.Double()));
    else
        return Variant(this->Double() * in.Double());
    }
}
 
 
 
Variant Variant::operator/ (Variant in){
    if((type==VAR_INTTYPE)&&(in.type==VAR_INTTYPE))
        return Variant((long long)(int64value/in.int64value));
    else{
    long long outl = (long long)(this->Double() / in.Double());
    double outf = this->Double() / in.Double();
    if (outf == (double)outl)
         return Variant((long long)(this->Double() / in.Double()));
    else
        return Variant(this->Double() / in.Double());
    }
}
 
Variant Variant::operator& (Variant in){
    Variant ret;
    this->Cache_StrVal();
    char* str = in.String();
    ret.concat_op(this->databuff, str, strlen(this->String()) + strlen(str) + 1);
    return ret;
}
 
void Variant::concat_op(char* former, char* latter, int finallen)
{
    char* all = new char[finallen];
    strcpy(all, former);
    strcat(all, latter);
    this->StorageReset();
    databuff = all;
    databuffsize = finallen;
    type = VAR_STRTYPE;
}
 
 
 
 
 
 
//Destructor
Variant::~Variant()
{
    this->StorageReset();
}
 
 
 
 
 
 
 
 
//Internal functions
void inline Variant::StorageReset(void)
{

    if(databuff != 0)
    {
        if(databuffsize > 0)
        {
            delete [] databuff;
        }
    }
    
    if(this->type == VAR_ARRAYTYPE)
    {
		int i = 0;
		for(;i<this->arrayaccess->totalelements;i++)
		{
			delete this->arrayaccess->data[i];
		}
		delete [] this->arrayaccess->data;
		delete this->arrayaccess;
    }
    
    type = VAR_INTTYPE;
    int64value = 0;
    databuff = (char*)0;
}
 
void Variant::Cache_StrVal(void)
{
    char buffer[32];
    
    if(databuff)//Already Cached.
        return;
    
    switch(type){
        case VAR_INTTYPE:
            sprintf(buffer, "%d", int64value);
            databuffsize = strlen(buffer)+1;
            databuff = new char[databuffsize];
            strcpy(databuff, buffer);
            break;
    	  case VAR_RESTYPE:
            sprintf(buffer, "%d", Resource_id);
            databuffsize = strlen(buffer)+1;
            databuff = new char[databuffsize];
            strcpy(databuff, buffer);
		break;
        case VAR_DOUBLETYPE:
            sprintf(buffer, "%f", dvalue);
            databuffsize = strlen(buffer)+1;
            databuff = new char[databuffsize];
            strcpy(databuff, buffer);
            break;
        default://All other cases: empty string.
            databuff = '\0';
            break;
    }
}
 
char* Variant::String(void){
    this->Cache_StrVal();
    return databuff;
}
 
int Variant::Short(void){
    switch(type){
        case VAR_INTTYPE:
            return (int)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (int)dvalue;
            break;
        case VAR_STRTYPE:
            return atoi(databuff);
            break;
    	  case VAR_RESTYPE:
		return Resource_id;
        default:
            return 0;
            break;
    }
}
 
long long Variant::Int(void){
    switch(type){
        case VAR_INTTYPE:
			//std::cout << "DEBUG INT() VAL: " << int64value << '\n';
            return (long long)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (long long)dvalue;
            break;
        case VAR_STRTYPE:
#ifdef _MSC_VER
            return _atoi64(databuff);
#else
            return atoll(databuff);
#endif
            break;
    	  case VAR_RESTYPE:
		return (long long)Resource_id;
        default:
            return 0;
            break;
    }
}
 
double Variant::Double(void){
    switch(type){
        case VAR_INTTYPE:
            return (double)int64value;
            break;
        case VAR_DOUBLETYPE:
            return dvalue;
            break;
        case VAR_STRTYPE:
            return atof(databuff);
            break;
        default:
            return 0;
            break;
    }
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
Variant::operator bool()
{
    switch(type){
        case VAR_INTTYPE:
            return (bool)int64value;
            break;
        case VAR_DOUBLETYPE:
            return (bool)dvalue;
            break;
        case VAR_STRTYPE:
            if(databuff=='\0')return false;
            return true;
            break;
    	  case VAR_RESTYPE:
		return (double)Resource_id;
        default:
            return false;
            break;
    }
}
 
 
 
 
 
 
 Variant Variant::operator() (int count, ...){
    va_list ap;
    int i, sum;
 
    va_start (ap, count);
 
    sum = 0;
    for (i = 0; i < count; i++)
      std::cout << va_arg (ap, Variant*)->String();
 
 
    va_end (ap);
 
    return Variant(sum);
}
 
 
 
 
 
 
 
 


//Array methods
Variant::Variant(int type, int dims, int s0, int s1=0, int s2=0, int s3=0, int s4=0, int s5=0, int s6=0, int s7=0, int s8=0)
{
    _DEBUG_MSG( "#### Declared Array: ");
    this->arrayaccess = new ArrayData;
    this->arrayaccess->num_dimensions = dims;

    int i=0;
    this->arrayaccess->totalelements = 1;
    for(;i<dims;i++){
       int size = 0;
	 switch(i)
	 {
		case 0:
			size = s0;
			break;
		case 1:
			size = s1;
			break;
		case 2:
			size = s2;
			break;
		case 3:
			size = s3;
			break;
		case 4:
			size = s4;
			break;
		case 5:
			size = s5;
			break;
		case 6:
			size = s6;
			break;
		case 7:
			size = s7;
			break;
		case 8:
			size = s8;
			break;
	 }
       this->arrayaccess->dim_sizes[i] = size;
       this->arrayaccess->totalelements *= size;
    }

    this->arrayaccess->data = new Variant*[this->arrayaccess->totalelements+2];

	//*********ALL VARIABLES ARE ALLOCATED HERE, NO NULL POINTERS. THIS OPTIMIZES LOOPTIME OPERATIONS***********
    for(i=0;i<=this->arrayaccess->totalelements;i++)
       	this->arrayaccess->data[i] = new Variant;
		//this->arrayaccess->data[i] = (Variant*)42;//NULL;//eww

	this->type = VAR_ARRAYTYPE;
	this->databuffsize = 0;
	this->databuff = 0;

	_DEBUG_MSG("#### Array Construction finished: ");

}
 


Variant* ArAcc(Variant* in_variable, int s0, int s1=0, int s2=0, int s3=0, int s4=0, int s5=0, int s6=0, int s7=0, int s8=0)
{
	int index=0;

	int i, j;
	int mult;
	_DEBUG_MSG("Start processing array access: ");

    if (in_variable->type != VAR_ARRAYTYPE){
		_DEBUG_MSG("\nSUBSCRIPT USED WITH NON-ARRAY VARIABLE.\n");
		exit(0);
	}

      for(i=0;i<in_variable->arrayaccess->num_dimensions;i++){
		mult = 1;
		for(j=i+1;j<in_variable->arrayaccess->num_dimensions;j++){
			mult = mult * in_variable->arrayaccess->dim_sizes[j];

		}
		switch (i)
		{
			case 0:
				mult = mult * s0;
				break;
			case 1:
				mult = mult * s1;
				break;
			case 2:
				mult = mult * s2;
				break;
			case 3:
				mult = mult * s3;
				break;
			case 4:
				mult = mult * s4;
				break;
			case 5:
				mult = mult * s5;
				break;
			case 6:
				mult = mult * s6;
				break;
			case 7:
				mult = mult * s7;
				break;
			case 8:
				mult = mult * s8;
				break;
		}
		index += mult;
      }
	

	return &((*in_variable)[index]);
}



int Variant::GetArrayElementCount()
{
   return this->arrayaccess->totalelements;
}

Variant& Variant::operator[] (int in){

    _DEBUG_MSG( "ArrayAccess: " );

	    if((this->arrayaccess->data[in] == (Variant*)0xCDCDCDCD) && 
		(this->arrayaccess->data[in] == (Variant*)0x0000002a)){
        	this->arrayaccess->data[in] = new Variant;
	}

    return (*(this->arrayaccess->data[in]));
}

typedef Variant vr;



/**************************************************
* END FALCON Variant System
**************************************************/
