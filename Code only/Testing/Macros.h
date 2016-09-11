#ifndef _MACROS_H_
#define _MACROS_H_

//Delete an Array safely
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }

//Delete an object pointer
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }

//Release an object pointer
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#endif