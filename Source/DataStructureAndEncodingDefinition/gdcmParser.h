
#ifndef __gdcmParser_h
#define __gdcmParser_h

#include "gdcmIStream.h"
#include "gdcmTag.h"

namespace gdcm
{
/**
 * \brief Parser ala XML_Parser from expat (SAX)
 *
 * Detailled description here
 * \note Simple API for DICOM
 */
class GDCM_EXPORT Parser : private IStream
{
public:
  typedef enum {
    NoError,
    NoMemoryError,
    SyntaxError,
    NoElementsError,
    TagMismatchError,
    DuplicateAttributeError,
    JunkAfterDocElementError,
    UndefinedEntityError,
    UnexpectedStateError
  } ErrorType;

  Parser() : UserData(0),Buffer(0),ErrorCode(NoError) {}
  ~Parser() {}

  // Parse some more of the document. The string s is a buffer containing 
  // part (or perhaps all) of the document. The number of bytes of s that 
  // are part of the document is indicated by len. This means that s 
  // doesn't have to be null terminated. It also means that if len is 
  // larger than the number of bytes in the block of memory that s points 
  // at, then a memory fault is likely. The isFinal parameter informs the 
  // parser that this is the last piece of the document. Frequently, the 
  // last piece is empty (i.e. len is zero.) If a parse error occurred, 
  // it returns 0. Otherwise it returns a non-zero value.
  bool Parse(const char* s, size_t len, bool isFinal);

  // Set handlers for start and end tags. Attributes are passed to the 
  // start handler as a pointer to a vector of char pointers. Each 
  // attribute seen in a start (or empty) tag occupies 2 consecutive places
  // in this vector: the attribute name followed by the attribute value. 
  // These pairs are terminated by a null pointer.
  typedef void (*StartElementHandler) (void *userData,
                                       const Tag &tag,
                                       const char *atts[]);
  typedef void (*EndElementHandler) (void *userData, const Tag &name);
  void SetElementHandler(StartElementHandler start, EndElementHandler end);

  // Return what type of error has occurred.
  ErrorType GetErrorCode() const;

  // Return a string describing the error corresponding to code. 
  // The code should be one of the enums that can be returned from 
  // GetErrorCode.
  static const char *GetErrorString(ErrorType const &err);

  // Return the byte offset of the position.
  unsigned long GetCurrentByteIndex() const;

  // Miscellaneous functions

  // The functions in this section either obtain state information from 
  // the parser or can be used to dynamically set parser options.

  // This sets the user data pointer that gets passed to handlers.
  void SetUserData(void *userData);

  // This returns the user data pointer that gets passed to handlers. 
  void * GetUserData() const;

protected:

  // This is just like Parse, except in this case expat provides the buffer.
  // By obtaining the buffer from expat with the GetBuffer function, 
  // the application can avoid double copying of the input.
  bool ParseBuffer(size_t len, bool isFinal);

  // Obtain a buffer of size len to read a piece of the document into. 
  // A NULL value is returned if expat can't allocate enough memory for 
  // this buffer. This has to be called prior to every call to ParseBuffer.
  char *GetBuffer(size_t len);

private:
  void* UserData;
  char *Buffer;
  ErrorType ErrorCode;

  StartElementHandler StartElement;
  EndElementHandler EndElement;
};

} // end namespace gdcm

#endif //__gdcmParser_h
