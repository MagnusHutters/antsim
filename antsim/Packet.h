#pragma once
class Packet
{
public:
	Packet(int type)
		: typeId(type)
	{
	}

	int getTypeId()
	{
		return typeId;
	}

private:
	int amount = 1;
	int typeId;

};

