#pragma once
#include "GTMATH.hpp"
#include <string.h>

namespace GT
{
	class Image
	{
	private:
		int m_width;
		int m_height;
		RGBA* m_data;
		float m_alpha;	// 透明度
	public:
		// 规定采样方式
		enum TEXTURE_TYPE
		{
			TX_CLAMP_TO_EDGE = 0,
			TX_REPEAT
		};

	public:
		Image(int _width=0, int _height=0, byte* _data=nullptr)
		{
			m_width = _width;
			m_height = _height;
			m_alpha = 1.0;
			if (_data)
			{
				m_data = new RGBA[m_width * m_height];  // m_width* m_height 是这个图片有多少个像素（像素的数量）
				memcpy(m_data, _data, sizeof(RGBA)*m_width*m_height);
			}
		}

		~Image()
		{
			if (m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}
		}

		int getWidth() const
		{
			return m_width;
		}

		int getHeight() const
		{
			return m_height;
		}

		RGBA getColor(int x, int y) const
		{
			if (x<0 || x>=m_width || y<0 || y>=m_height)
			{
				return RGBA(0, 0, 0, 0);
			}
			return m_data[y * m_width + x];
		}

		RGBA getColorByUV(float _u, float _v,TEXTURE_TYPE _type) const
		{
			int x = (float)m_width * _u;
			int y = (float)m_height * _v;
			
			switch (_type)
			{
			case TX_CLAMP_TO_EDGE:
				x = x < m_width ? x : (m_width - 1);
				y = y < m_height ? y : (m_height - 1);
				break;
			case TX_REPEAT:
				x = x % m_width;
				y = y % m_height;
				break;
			default:
				break;
			}
			

			return m_data[y * m_width + x];
		}

		inline void setAlpha(float _alpha)
		{
			m_alpha = _alpha;
		}

		inline float getAlpha()
		{
			return m_alpha;
		}
	public:
		// 读取图片
		static Image* readFromFile(const char* _fileName);
		// 进行缩放图片		_zoomX	x方向的缩放值		_zoomY	y方向的缩放值	_image 是老图片
		static Image* zoomImageSimple(const Image* _image, float _zoomX, float _zoomY);
		// 双线性插值算法进行缩放		_zoomX	x方向的缩放值		_zoomY	y方向的缩放值	_image 是老图片
		static Image* zoomImage(const Image* _image, float _zoomX, float _zoomY);
	};
}