#pragma once

#include <list>
#include <SFML/Graphics.hpp>



struct KeyMap
{
	int id;
	sf::Keyboard::Key key;
	char symbol;
};


class Input
{
public:
	std::map<char, int> charToIndex{
		{'1', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7}, {'9', 8}, {'0', 9}, {'q', 10},
		{'w', 11}, {'e', 12}, {'r', 13}, {'t', 14}, {'y', 15}, {'u', 16}, {'i', 17}, {'o', 18}, {'p', 19}
	};
	std::vector<KeyMap> keys;  
	
	Input()
	{
		keys= {
		{0, sf::Keyboard::Num1, '1'},
		{1, sf::Keyboard::Num2, '2'},
		{2, sf::Keyboard::Num3, '3'},
		{3, sf::Keyboard::Num4, '4'},
		{4, sf::Keyboard::Num5, '5'},
		{5, sf::Keyboard::Num6, '6'},
		{6, sf::Keyboard::Num7, '7'},
		{7, sf::Keyboard::Num8, '8'},
		{8, sf::Keyboard::Num9, '9'},
		{9, sf::Keyboard::Num0, '0'},
		{10, sf::Keyboard::Q, 'q'},
		{11, sf::Keyboard::W, 'w'},
		{12, sf::Keyboard::E, 'e'},
		{13, sf::Keyboard::R, 'r'},
		{14, sf::Keyboard::T, 't'},
		{15, sf::Keyboard::Y, 'y'},
		{16, sf::Keyboard::U, 'u'},
		{17, sf::Keyboard::I, 'i'},
		{18, sf::Keyboard::O, 'o'},
		{19, sf::Keyboard::P, 'p'}
		};


	}
	enum KeyOperation{Pressed, Previous, Changed, Down, Up};

	void update()
	{
		keyState[Previous] = keyState[Pressed];
		keyState[Pressed] = 0;
		for (int i = keys.size()-1; i >= 0; i--)
		{
			
			keyState[Pressed] = keyState[Pressed] << 1;
			keyState[Pressed] |= sf::Keyboard::isKeyPressed(keys[i].key);
		}

		keyState[Changed] = keyState[Previous] ^ keyState[Pressed];
		keyState[Down] = keyState[Changed] & keyState[Pressed];
		keyState[Up] = keyState[Changed] & keyState[Previous];
	}

	bool _getKeyState(int index, KeyOperation operation)
	{
		long bit = 1 << index;
		return bit & keyState[operation];
	}


	struct _getKeyReturn { bool state; KeyMap key; };
	_getKeyReturn getKey(char character, KeyOperation operation)
	{
		int index = charToIndex[character];
		return { _getKeyState(index, operation), keys[index]};
	}

	long getKeys(KeyOperation operation)
	{
		return keyState[operation];
	}
	

	long keyState[5]; //0: current	1: previous	2:changed	3:pressed	4:released

};

