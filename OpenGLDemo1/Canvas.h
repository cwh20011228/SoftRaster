#pragma once
#include "GTMATH.hpp"
#include <string.h>
#include <vector>
#include "Image.h"



namespace GT
{
	struct Point
	{
	public:
		float m_x;
		float m_y;
		float m_z;
		RGBA m_color;
		floatV2 m_uv;	// uv坐标

		Point(float _x=0, float _y=0,float _z=0, RGBA _color=RGBA(0,0,0,0), floatV2 _uv = floatV2(0.0, 0.0))
		{
			m_x = _x;
			m_y = _y;
			m_z = _z;
			m_color = _color;
			m_uv = _uv;
		}

		~Point()
		{

		}
	};

	enum DATA_TYPE
	{
		GT_FLOAT = 0,
		GT_INT=1
	};

	enum DRAW_MODE
	{
		GT_LINE=0,
		GT_TRIANGLE=1
	};

	struct DataElement
	{
		int				m_size;			// 每个点有多少数据
		DATA_TYPE		m_type;
		int				m_stride;		// 步长
		byte*			m_data;

		DataElement()
		{
			m_size = -1;
			m_type = GT_FLOAT;
			m_stride = -1;
			m_data = nullptr;
		}
	};

	struct Statement
	{
		// 状态变量
		bool					m_useBlend;				// 是否使用混合
		bool					m_enableTexture;		// 是否采用纹理贴图
		const Image*			m_texture;				// 图片
		Image::TEXTURE_TYPE		m_texType;				// 采样方式
		byte					m_alphaLimit;			// 大于此值的像素才可以绘制

		DataElement				m_vertexData;			// 顶点坐标
		DataElement				m_colorData;			// 颜色值
		DataElement				m_texCoordData;			// 纹理坐标

		Statement()
		{
			m_useBlend = false;
			m_enableTexture = false;
			m_texture = nullptr;
			m_texType = Image::TX_REPEAT;
			m_alphaLimit = 0;
		}
	};


	class Canvas
	{
	private:
		int				m_width;
		int				m_height;
		RGBA*			m_buffer;
		
		Statement		m_state;

	public:
		Canvas(int _width, int _height, void* _buffer)
		{
			if (_width <= 0 || _height <= 0)
			{
				m_width = -1;
				m_height = -1;
				m_buffer = nullptr;
			}
			m_width = _width;
			m_height = _height;
			m_buffer = (RGBA*)_buffer;
			
		}

		~Canvas()
		{
		}

		// 清洗操作
		void clear()
		{
			if (m_buffer != nullptr)
			{
				memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
			}
		}


		// 画点操作
		void drawPoint(int x,int y,RGBA _color)
		{
			if (x < 0 || x >= m_width || y<0 || y>=m_height)
			{
				return;
			}
			m_buffer[y * m_width + x] = _color;		// y表示行，x表示列
		}
		// 取背景颜色
		RGBA getColor(int x, int y)
		{
			if (x<0 || x>m_width - 1 || y<0 || y>m_height - 1)
			{
				return RGBA(0,0,0,0);
			}
			return m_buffer[y * m_width + x];
		}

		// 颜色插值  _color1是起始点的颜色   _color2 是终止点的颜色
		inline RGBA colorLerp(RGBA _color1, RGBA _color2, float _scale)		
		{
			RGBA _color;
			_color.m_r = _color1.m_r + (float)(_color2.m_r - _color1.m_r) * _scale;
			_color.m_g = _color1.m_g + (float)(_color2.m_g - _color1.m_g) * _scale;
			_color.m_b = _color1.m_b + (float)(_color2.m_b - _color1.m_b) * _scale;
			_color.m_a = _color1.m_a + (float)(_color2.m_a - _color1.m_a) * _scale;
			return _color;
		}

		// uv坐标系下的插值  _uv1是起点  _uv2是终点
		inline floatV2 uvLerp(floatV2 _uv1, floatV2 _uv2, float _scale)
		{
			floatV2 _uv;
			_uv.x = _uv1.x + (_uv2.x - _uv1.x) * _scale;
			_uv.y = _uv1.y + (_uv2.y - _uv1.y) * _scale;

			return _uv;
			
		}
		// 画线操作
		void drawLine(intV2 pt1, intV2 pt2, RGBA _color);
		void drawLine(Point pt1, Point pt2);

		// 画三角形
		void drawTriangle(Point pt1, Point pt2, Point pt3);
		// 画平底平顶三角形    ptFlat1  ptFlat2 是平底或平顶的两个点   pt是尖的那个点
		void drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt);	
		// 画任意三角形
		void drawTriangleAny(Point pt1, Point pt2, Point pt3);	
		// 判断点是否在屏幕之内（RecT）
		bool judgeInRecT(Point pt, GT_RECT _rect);
		// 判断屏幕中的点是否在三角形中
		bool judgeInTriangle(Point pt, std::vector<Point> _ptArray);
		// 剪裁x
		bool judgeLineAndRect(int _x1, int _x2, int& _x1Cut, int& _x2Cut);
		//TODO: 图片操作
		void drawImage(int _x,int _y,Image* _image);
		// 设置alpha透明度
		void setAlphaLimit(byte _limit)
		{
			m_state.m_alphaLimit = _limit;
		}
		// 设置是否使用颜色混色的变量
		void setBlend(bool _useBlend)
		{
			m_state.m_useBlend = _useBlend;
		}
		// 设置纹理贴图变量的值
		void enableTexture(bool _enable)
		{
			m_state.m_enableTexture = _enable;
		}

		// 绑定纹理到图片
		void bindTexture(const Image* _image)
		{
			m_state.m_texture = _image;
		}

		// 设置采样方式
		void setTextureType(Image::TEXTURE_TYPE _texType)
		{
			m_state.m_texType = _texType;
		}

		// 顶点坐标
		void gtVertexPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);
		// 颜色坐标
		void gtColorPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);
		// UV坐标
		void gtTexCoordPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);

		// _first是起始点位置23  _count是总共的点的数量
		void gtDrawArray(DRAW_MODE _mode,int _first,int _count);

	};
}


