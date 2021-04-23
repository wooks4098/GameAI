#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_GoBank,
  Msg_Deal,
  Msg_Deal_Success,
  Msg_Deal_Fail,


};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome: 
    return "HiHoneyImHome"; 

  case Msg_StewReady:   
    return "StewReady";

  case Msg_GoBank:
	  return "GoBank";

  case Msg_Deal:
	  return "Deal";
  case Msg_Deal_Success:
	  return "Deal_Success";
  case Msg_Deal_Fail:
	  return "Deal_Fail";

  default:

    return "Not recognized!";
  }
}

#endif