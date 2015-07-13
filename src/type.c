#include "type.h"


Word
Word_ofInt(const Int intNum)
{
  return (Word){.intNum = intNum};
}


Word
Word_ofReal(const Real realNum)
{
  return (Word){.realNum = realNum};
}
