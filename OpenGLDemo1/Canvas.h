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
		floatV2 m_uv;	// uv����

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
		int				m_size;			// ÿ�����ж�������
		DATA_TYPE		m_type;
		int				m_stride;		// ����
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
		// ״̬����
		bool					m_useBlend;				// �Ƿ�ʹ�û��
		bool					m_enableTexture;		// �Ƿ����������ͼ
		const Image*			m_texture;				// ͼƬ
		Image::TEXTURE_TYPE		m_texType;				// ������ʽ
		byte					m_alphaLimit;			// ���ڴ�ֵ�����زſ��Ի���

		DataElement				m_vertexData;			// ��������
		DataElement				m_colorData;			// ��ɫֵ
		DataElement				m_texCoordData;			// ��������

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

		// ��ϴ����
		void clear()
		{
			if (m_buffer != nullptr)
			{
				memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
			}
		}


		// �������
		void drawPoint(int x,int y,RGBA _color)
		{
			if (x < 0 || x >= m_width || y<0 || y>=m_height)
			{
				return;
			}
			m_buffer[y * m_width + x] = _color;		// y��ʾ�У�x��ʾ��
		}
		// ȡ������ɫ
		RGBA getColor(int x, int y)
		{
			if (x<0 || x>m_width - 1 || y<0 || y>m_height - 1)
			{
				return RGBA(0,0,0,0);
			}
			return m_buffer[y * m_width + x];
		}

		// ��ɫ��ֵ  _color1����ʼ�����ɫ   _color2 ����ֹ�����ɫ
		inline RGBA colorLerp(RGBA _color1, RGBA _color2, float _scale)		
		{
			RGBA _color;
			_color.m_r = _color1.m_r + (float)(_color2.m_r - _color1.m_r) * _scale;
			_color.m_g = _color1.m_g + (float)(_color2.m_g - _color1.m_g) * _scale;
			_color.m_b = _color1.m_b + (float)(_color2.m_b - _color1.m_b) * _scale;
			_color.m_a = _color1.m_a + (float)(_color2.m_a - _color1.m_a) * _scale;
			return _color;
		}

		// uv����ϵ�µĲ�ֵ  _uv1�����  _uv2���յ�
		inline floatV2 uvLerp(floatV2 _uv1, floatV2 _uv2, float _scale)
		{
			floatV2 _uv;
			_uv.x = _uv1.x + (_uv2.x - _uv1.x) * _scale;
			_uv.y = _uv1.y + (_uv2.y - _uv1.y) * _scale;

			return _uv;
			
		}
		// ���߲���
		void drawLine(intV2 pt1, intV2 pt2, RGBA _color);
		void drawLine(Point pt1, Point pt2);

		// ��������
		void drawTriangle(Point pt1, Point pt2, Point pt3);
		// ��ƽ��ƽ��������    ptFlat1  ptFlat2 ��ƽ�׻�ƽ����������   pt�Ǽ���Ǹ���
		void drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt);	
		// ������������
		void drawTriangleAny(Point pt1, Point pt2, Point pt3);	
		// �жϵ��Ƿ�����Ļ֮�ڣ�RecT��
		bool judgeInRecT(Point pt, GT_RECT _rect);
		// �ж���Ļ�еĵ��Ƿ�����������
		bool judgeInTriangle(Point pt, std::vector<Point> _ptArray);
		// ����x
		bool judgeLineAndRect(int _x1, int _x2, int& _x1Cut, int& _x2Cut);
		//TODO: ͼƬ����
		void drawImage(int _x,int _y,Image* _image);
		// ����alpha͸����
		void setAlphaLimit(byte _limit)
		{
			m_state.m_alphaLimit = _limit;
		}
		// �����Ƿ�ʹ����ɫ��ɫ�ı���
		void setBlend(bool _useBlend)
		{
			m_state.m_useBlend = _useBlend;
		}
		// ����������ͼ������ֵ
		void enableTexture(bool _enable)
		{
			m_state.m_enableTexture = _enable;
		}

		// ������ͼƬ
		void bindTexture(const Image* _image)
		{
			m_state.m_texture = _image;
		}

		// ���ò�����ʽ
		void setTextureType(Image::TEXTURE_TYPE _texType)
		{
			m_state.m_texType = _texType;
		}

		// ��������
		void gtVertexPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);
		// ��ɫ����
		void gtColorPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);
		// UV����
		void gtTexCoordPointer(int _size, DATA_TYPE _type, int _stride, byte* _data);

		// _first����ʼ��λ��23  _count���ܹ��ĵ������
		void gtDrawArray(DRAW_MODE _mode,int _first,int _count);

	};
}


