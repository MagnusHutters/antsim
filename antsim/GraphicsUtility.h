#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics.hpp>







static class GraphicsUtility
{
public:
	GraphicsUtility()
	{
		if (!font.loadFromFile("arial.ttf"))
		{
			// error...
			while (1)
			{

			}
		}

	}


	static sf::Color GetHSVColor(uint8_t  h, uint8_t  s, uint8_t  v)
	{
		sf::Color rgb;
		unsigned char region, remainder, p, q, t;

		if (s == 0)
		{
			rgb.r = v;
			rgb.g = v;
			rgb.b = v;
			return rgb;
		}

		region = h / 43;
		remainder = (h - (region * 43)) * 6;

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region)
		{
		case 0:
			rgb.r = v; rgb.g = t; rgb.b = p;
			break;
		case 1:
			rgb.r = q; rgb.g = v; rgb.b = p;
			break;
		case 2:
			rgb.r = p; rgb.g = v; rgb.b = t;
			break;
		case 3:
			rgb.r = p; rgb.g = q; rgb.b = v;
			break;
		case 4:
			rgb.r = t; rgb.g = p; rgb.b = v;
			break;
		default:
			rgb.r = v; rgb.g = p; rgb.b = q;
			break;
		}

		return rgb;
	}
	static sf::Color createColorFromTypeId(int id, double pheromoneStrenght = 100.0, int offset = 0, float steepness = 0.5f)
	{
		uint8_t sat = (uint8_t)(255 * (pheromoneStrenght / (pheromoneStrenght + steepness)));
		uint8_t hue = (uint8_t)((id * 83) + (offset * 43));

		return GetHSVColor(hue, sat, 255);
	}



	static sf::Font font;

};

