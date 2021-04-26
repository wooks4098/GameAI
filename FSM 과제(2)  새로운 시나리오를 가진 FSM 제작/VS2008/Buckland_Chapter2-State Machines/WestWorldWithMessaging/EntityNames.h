#ifndef NAMES_H
#define NAMES_H

#include <string>

enum
{
	ent_Miner_Bob,

	ent_Elsa,

	ent_Gold_Coin_Manager,

	ent_Cat,
	ent_Cat_Master,
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "Miner Bob";

  case ent_Elsa:
    
    return "Elsa"; 

  case ent_Gold_Coin_Manager:
	  return "Gold Coin Manager";

  case 	ent_Cat_Master:
	  return "����";

  case ent_Cat:
	  return "�����";
		  
  default:

    return "UNKNOWN!";
  }
}

#endif