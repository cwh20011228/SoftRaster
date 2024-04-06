#include "Canvas.h"
#include <cmath>
#include <vector>
#include <algorithm>



namespace GT
{
	// ʹ��bresenham�㷨����
	void Canvas::drawLine(intV2 pt1, intV2 pt2, RGBA _color)
	{
		int disY = abs(pt2.y - pt1.y);	// ��y
		int disX = abs(pt2.x - pt1.x);	// ��x
		
		int xNow = pt1.x;
		int yNow = pt1.y;

		int stepX = 0;
		int stepY = 0;

		// �ж��������򲽽�������
		if (pt1.x < pt2.x)
		{
			stepX = 1;
		}
		else 
		{
			stepX = -1;
		}

		if (pt1.y < pt2.y)
		{
			stepY = 1;
		}
		else
		{
			stepY = -1;
		}

		// �Ա�xyƫ���������������ķ���ѡȡ x �� y
		int sumStep = disX;
		bool useXStep = true;
		if (disX < disY)
		{
			sumStep = disY;
			useXStep = false;
			SWAP_INT(disX, disY);
		}

		// ��ʼ��p��ֵ��P0=2��y-��x
		int p = 2 * disY - disX;
		for (auto i = 0; i < sumStep; i++)
		{
			drawPoint(xNow, yNow, _color);
			if (p >= 0)
			{
				// ���������������
				if (useXStep)
				{
					yNow = yNow + stepY;
				}
				else
				{
					xNow = xNow + stepX;
				}
				
				p = p - 2 * disX;
			}
			// ���������꣨�Ա��������꣩
			if (useXStep)
			{
				xNow = xNow + stepX;
			}
			else
			{
				yNow = yNow + stepY;
			}
			p = p + 2 * disY;
		}

	}
	// ʹ��bresenham�㷨����
	void Canvas::drawLine(Point pt1, Point pt2)
	{
		int disY = abs(pt2.m_y - pt1.m_y);	// ��y
		int disX = abs(pt2.m_x - pt1.m_x);	// ��x

		int xNow = pt1.m_x;
		int yNow = pt1.m_y;

		int stepX = 0;
		int stepY = 0;

		// �ж��������򲽽�������
		if (pt1.m_x < pt2.m_x)
		{
			stepX = 1;
		}
		else
		{
			stepX = -1;
		}

		if (pt1.m_y < pt2.m_y)
		{
			stepY = 1;
		}
		else
		{
			stepY = -1;
		}

		// �Ա�xyƫ���������������ķ���ѡȡ x �� y
		int sumStep = disX;
		bool useXStep = true;
		if (disX < disY)
		{
			sumStep = disY;
			useXStep = false;
			SWAP_INT(disX, disY);
		}


		RGBA _color;
		// ��ʼ��p��ֵ��P0=2��y-��x
		int p = 2 * disY - disX;
		for (int i = 0; i <= sumStep; ++i)
		{
			
			float _scale=0;
			if (useXStep)
			{
				if (pt2.m_x == pt1.m_x)
				{
					_scale = 0;
				}
				else
				{
					_scale = (float)(xNow - pt1.m_x) / (float)(pt2.m_x - pt1.m_x);
				}
			}
			else
			{
				if (pt2.m_x == pt1.m_x)
				{
					_scale = 0;
				}
				else
				{
					_scale = (float)(yNow - pt1.m_y) / (float)(pt2.m_y - pt1.m_y);
				}
				
			}

			if (m_state.m_enableTexture)	// ��ͼƬ��ɫ  Ҫ�õ� uv����
			{
				floatV2 _uv = uvLerp(pt1.m_uv, pt2.m_uv, _scale);
				if (m_state.m_texture!=nullptr)
				{
					_color = m_state.m_texture->getColorByUV(_uv.x,_uv.y,m_state.m_texType);  // ȡ��ͼƬ��ɫ
				}
				else
				{
					_color = colorLerp(pt1.m_color, pt2.m_color, _scale);	// �ö����Լ�����ɫ
				}
			}
			else	// �ö����Լ�����ɫ
			{
				_color = colorLerp(pt1.m_color, pt2.m_color, _scale);
			}

			
			drawPoint(xNow, yNow, _color);

			if (p >= 0)
			{
				// ���������������
				if (useXStep)
				{
					yNow = yNow + stepY;
				}
				else
				{
					xNow = xNow + stepX;
				}

				p = p - 2 * disX;
			}
			// ���������꣨�Ա��������꣩
			if (useXStep)
			{
				xNow = xNow + stepX;
			}
			else
			{
				yNow = yNow + stepY;
			}
			p = p + 2 * disY;
		}

	}

	void Canvas::drawTriangle(Point pt1, Point pt2, Point pt3)
	{
		// ������Χ��
		int left = MIN(pt3.m_x,MIN(pt1.m_x, pt2.m_x));
		int right = MAX(pt3.m_x,MAX(pt1.m_x,pt2.m_x));
		int buttom = MIN(pt3.m_y, MIN(pt1.m_y, pt2.m_y));
		int top = MAX(pt3.m_y, MAX(pt1.m_y, pt2.m_y));

		// ������Ļ
		left = left < 0 ? 0 : left;
		right = right > (m_width - 1) ? (m_width - 1) : right;
		buttom = buttom < 0 ? 0 : buttom;
		top = top > (m_height - 1) ? (m_height - 1) : top;

		// ����ֱ�߲���ֵ
		float k1 = (float)(pt2.m_y - pt3.m_y) / (float)(pt2.m_x - pt3.m_x);	// e1��б��
		float k2 = (float)(pt1.m_y - pt3.m_y) / (float)(pt1.m_x - pt3.m_x);	// e2��б��
		float k3 = (float)(pt1.m_y - pt2.m_y) / (float)(pt1.m_x - pt2.m_x);	// e3��б��

		// ֱ�ߵ�bֵ
		float b1 = pt2.m_y - pt2.m_x * k1;
		float b2 = pt1.m_y - pt1.m_x * k2;
		float b3 = pt1.m_y - pt1.m_x * k3;

		// ѭ���ж�
		for (int x = left; x <= right; x++)
		{
			for (int y = buttom; y <= top; y++)
			{
				// �жϵ�ǰ���Ƿ��������η�Χ��
				float judge1 = (y - (k1 * x + b1)) * (pt1.m_y - (pt1.m_x * k1 + b1));	// ͬ���ţ���˴���0
				float judge2 = (y - (k2 * x + b2)) * (pt2.m_y - (pt2.m_x * k2 + b2));
				float judge3 = (y - (k3 * x + b3)) * (pt3.m_y - (pt3.m_x * k3 + b3));

				if (judge1 >=0 && judge2 >=0 && judge3 >=0)
				{
					drawPoint(x, y, RGBA(255, 0, 0,0));
				}

			}
		}

	}

	// ����x
	bool Canvas::judgeLineAndRect(int _x1, int _x2, int& _x1Cut, int& _x2Cut)
	{
		if (_x1 < 0 && _x2 < 0)
		{
			return false;
		}
		if (_x1 > m_width - 1 && _x2 > m_width - 1)
		{
			return false;
		}

		// ����x1
		if (_x1 < 0)
		{
			_x1Cut = 0;
		}
		if (_x1 > m_width - 1)
		{
			_x1Cut = m_width - 1;
		}

		// ����x2
		if (_x2 < 0)
		{
			_x2Cut = 0;
		}
		if (_x2 > m_width - 1)
		{
			_x2Cut = m_width - 1;
		}
		return true;

	}

	// ��ƽ��ƽ��������    ptFlat1  ptFlat2 ��ƽ�׻�ƽ����������   pt�Ǽ���Ǹ���
	void Canvas::drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt)
	{
		float k1 = 0.0;
		float k2 = 0.0;

		if (pt.m_x != ptFlat1.m_x)
		{
			k1 = (float)(pt.m_y - ptFlat1.m_y) / (float)(pt.m_x - ptFlat1.m_x);
		}
		if (pt.m_x != ptFlat2.m_x)
		{
			k2 = (float)(pt.m_y - ptFlat2.m_y) / (float)(pt.m_x - ptFlat2.m_x);
		}
		
		float b1 = (float)pt.m_y - pt.m_x * k1;
		float b2 = (float)pt.m_y - pt.m_x * k2;

		// ƽ�������� ptFlat1 ptFlat2��y����ͬ��
		int yStart = MIN(pt.m_y, ptFlat1.m_y);
		int yEnd = MAX(pt.m_y, ptFlat1.m_y);

		// ��ɫ��ֵ���		˭��yֵС��˭������ʼ��
		RGBA colorStart1;
		RGBA colorEnd1;
		RGBA colorStart2;
		RGBA colorEnd2;

		// ʹ��uv����ϵ
		floatV2 uvStart1;
		floatV2 uvEnd1;
		floatV2 uvStart2;
		floatV2 uvEnd2;

		if (pt.m_y < ptFlat1.m_y)	// ƽ�������Σ��������棩
		{
			yStart = pt.m_y;
			yEnd = ptFlat1.m_y;

			colorStart1 = pt.m_color;
			colorEnd1 = ptFlat1.m_color;
			colorStart2 = pt.m_color;
			colorEnd2 = ptFlat2.m_color;

			uvStart1 = pt.m_uv;
			uvEnd1 = ptFlat1.m_uv;
			uvStart2 = pt.m_uv;
			uvEnd2 = ptFlat2.m_uv;
		}
		else	// ƽ��������
		{
			yStart = ptFlat1.m_y;
			yEnd = pt.m_y;

			colorStart1 = ptFlat1.m_color;
			colorEnd1 = pt.m_color;
			colorStart2 = ptFlat2.m_color;
			colorEnd2 = pt.m_color;

			uvStart1 = ptFlat1.m_uv;
			uvEnd1 = pt.m_uv;
			uvStart2 = ptFlat2.m_uv;
			uvEnd2 = pt.m_uv;
		}

		// ����֮ǰ���㵽�׵ľ���
		float yColorStep = 1.0 / (float)(yEnd - yStart);
		// �������֮ǰyStart��ֵ
		int yColorStart = yStart;


		// ����y��
		if (yStart < 0)
		{
			yStart = 0;
		}
		if (yEnd > m_height)
		{
			yEnd = m_height-1;
		}

		for (int y = yStart; y < yEnd; y++)
		{
			int x1 = 0;
			if (k1 == 0)
			{
				x1 = ptFlat1.m_x;
			}
			else
			{
				x1 = ((float)y - b1) / k1;
			}

			int x2 = 0;
			if (k2 == 0)
			{
				x2 = ptFlat2.m_x;
			}
			else
			{
				x2 = ((float)y - b2) / k2;
			}


			// ����x1��x2
			int x1Cut = x1;
			int x2Cut = x2;
			if (!judgeLineAndRect(x1, x2, x1Cut, x2Cut))
			{
				continue;
			}
			// s��Ȩ�أ�������
			float s = (float)(y - yColorStart) *yColorStep;

			RGBA _color1;
			RGBA _color2;
			
			_color1 = colorLerp(colorStart1, colorEnd1, s);
			_color2 = colorLerp(colorStart2, colorEnd2, s);

			// �������x���������Ӱ��
			RGBA _color1Cut = _color1;
			RGBA _color2Cut = _color2;
			if (x2 != x1)
			{
				_color1Cut = colorLerp(_color1,_color2,(float)(x1Cut-x1)/(float)(x2-x1));
				_color2Cut = colorLerp(_color1, _color2, (float)(x2Cut - x1) / (float)(x2 - x1));
			}

			// uv����
			floatV2 _uv1 = uvLerp(uvStart1,uvEnd1,s);
			floatV2 _uv2 = uvLerp(uvStart2, uvEnd2, s);

			// �������x���������Ӱ��
			floatV2 _uv1Cut = _uv1;
			floatV2 _uv2Cut = _uv2;
			if (x2 != x1)
			{
				_uv1Cut = uvLerp(_uv1, _uv2, (float)(x1Cut - x1) / (float)(x2 - x1));
				_uv2Cut = uvLerp(_uv1, _uv2, (float)(x2Cut - x1) / (float)(x2 - x1));
			}
			Point pt1(x1Cut, y,0, _color1Cut,_uv1Cut);
			Point pt2(x2Cut, y,0, _color2Cut,_uv2Cut);

			drawLine(pt1, pt2);		// bresenham�㷨����

		}
	}
	// �������������
	void  Canvas::drawTriangleAny(Point pt1, Point pt2, Point pt3)
	{
		std::vector<Point> pVec;
		pVec.push_back(pt1);
		pVec.push_back(pt2);
		pVec.push_back(pt3);

		//��Ļ����
		GT_RECT _rect(0, m_width,0,m_height);	

		// �ж���Ļ�������������Ƿ��ཻ
		while (true)
		{
			// �ж������εĵ��Ƿ�����Ļ������
			if (judgeInRecT(pt1, _rect) || 
				judgeInRecT(pt2, _rect) || 
				judgeInRecT(pt3, _rect))
			{
				break;
			}

			Point rpt1(0, 0,0, RGBA());
			Point rpt2(m_width,0, 0,RGBA());
			Point rpt3(0, m_height, 0,RGBA());
			Point rpt4(m_width,m_height,0,RGBA());

			// �ж���Ļ�����еĵ��Ƿ�����������
			if (judgeInTriangle(rpt1, pVec) || 
				judgeInTriangle(rpt2, pVec) || 
				judgeInTriangle(rpt3, pVec) || 
				judgeInTriangle(rpt4, pVec))
			{
				break;
			}
			return;

		}
		
		// ��������
		std::sort(pVec.begin(), pVec.end(), 
					[](const Point& pt1, const Point& pt2) {return pt1.m_y > pt2.m_y; });	
		

		Point ptMax = pVec[0];
		Point ptMin = pVec[2];
		Point ptMiddle = pVec[1];

		// �����ƽ��������
		if (ptMax.m_y == ptMiddle.m_y)
		{
			drawTriangleFlat(ptMax, ptMiddle, ptMin);
			return;
		}

		// �����ƽ��������
		if (ptMin.m_y == ptMiddle.m_y)
		{
			drawTriangleFlat(ptMin, ptMiddle, ptMax);
			return;
		}

		float k = 0.0;
		if (ptMax.m_x != ptMin.m_x)
		{
			k = (float)(ptMax.m_y - ptMin.m_y) / (float)(ptMax.m_x-ptMin.m_x);
		}

		float b = (float)ptMax.m_y - (float)ptMax.m_x * k;

		Point npt(0, 0,0, RGBA(255,0, 0));

		if (k == 0)
		{
			npt.m_x = ptMax.m_x;
		}
		else
		{
			npt.m_x = ((float)ptMiddle.m_y - b) / k;
		}
		npt.m_y = ptMiddle.m_y;

		float s = (float)(npt.m_y - ptMin.m_y) / (float)(ptMax.m_y - ptMin.m_y);	// s��Ȩ�أ�������
		npt.m_color = colorLerp(ptMin.m_color, ptMax.m_color, s);	// npt����ɫ���������
		npt.m_uv = uvLerp(ptMin.m_uv, ptMax.m_uv,s);


		drawTriangleFlat(ptMiddle, npt,ptMax);
		drawTriangleFlat(ptMiddle, npt, ptMin);

		return;


	}
	// �жϵ��Ƿ�����Ļ��
	bool Canvas::judgeInRecT(Point pt, GT_RECT _rect)
	{
		if (pt.m_x > _rect.m_left && pt.m_x < _rect.m_right && pt.m_y > _rect.m_bottom && pt.m_y < _rect.m_top)
		{
			return true;
		}
		return false;
	}

	// �ж���Ļ�еĵ��Ƿ�����������
	bool Canvas::judgeInTriangle(Point pt, std::vector<Point> _ptArray)
	{
		Point pt1 = _ptArray[0];
		Point pt2 = _ptArray[1];
		Point pt3 = _ptArray[2];

		float k1 = (float)(pt2.m_y - pt3.m_y) / (float)(pt2.m_x - pt3.m_x);
		float k2 = (float)(pt1.m_y - pt3.m_y) / (float)(pt1.m_x - pt3.m_x);
		float k3 = (float)(pt1.m_y - pt2.m_y) / (float)(pt1.m_x - pt2.m_x);

		float b1 = (float)pt2.m_y - pt2.m_x * k1;
		float b2 = (float)pt1.m_y - pt1.m_x * k2;
		float b3 = (float)pt1.m_y - pt1.m_x * k3;

		
		// �жϵ��Ƿ�����������
		int y = pt.m_y;
		int x = pt.m_x;

		float judge1 = (y - (k1 * x + b1)) * (pt1.m_y - (pt1.m_x * k1 + b1));
		float judge2 = (y - (k2 * x + b2)) * (pt2.m_y - (pt2.m_x * k2 + b2));
		float judge3 = (y - (k3 * x + b3)) * (pt3.m_y - (pt3.m_x * k3 + b3));

		if (judge1 > 0 && judge2 > 0 && judge3 > 0)
		{
			return true;
		}
		return false;


	}

	// ͼƬ����   _x��_y��ͼƬ���½ǿ�ʼ��λ��
	void Canvas::drawImage(int _x, int _y, Image* _image)
	{
		for (int u = 0; u < _image->getWidth(); u++)
		{	// _srcColor  ͼƬ��ɫ		_dstColor	������ɫ 
			for (int v = 0; v < _image->getHeight(); v++)
			{
				// ȡ��ͼƬ��ɫ
				RGBA _srcColor = _image->getColor(u, v);

				if (!m_state.m_useBlend)
				{
					drawPoint(u + _x, v + _y, _srcColor);
				}
				else
				{
					// ȡ�ñ�����ɫ
					RGBA _dstColor = getColor(_x+u, _y+v);
					float _srcAlpha = (float)_srcColor.m_a / 255.0;		// ��ǰͼƬ��͸���Ȧ�
					RGBA _finalColor = colorLerp(_dstColor, _srcColor, _image->getAlpha()*_srcAlpha);
					drawPoint(_x+u,_y+v,_finalColor);
				}
			}
		}
	}


	void Canvas::gtVertexPointer(int _size, DATA_TYPE _type, int _stride, byte* _data)
	{
		m_state.m_vertexData.m_size = _size;
		m_state.m_vertexData.m_type = _type;
		m_state.m_vertexData.m_stride = _stride;
		m_state.m_vertexData.m_data = _data;
	}

	void Canvas::gtColorPointer(int _size, DATA_TYPE _type, int _stride, byte* _data)
	{
		m_state.m_colorData.m_size = _size;
		m_state.m_colorData.m_type = _type;
		m_state.m_colorData.m_stride = _stride;
		m_state.m_colorData.m_data = _data;
	}

	void Canvas::gtTexCoordPointer(int _size, DATA_TYPE _type, int _stride, byte* _data)
	{
		m_state.m_texCoordData.m_size = _size;
		m_state.m_texCoordData.m_type = _type;
		m_state.m_texCoordData.m_stride = _stride;
		m_state.m_texCoordData.m_data = _data;
	}

	void Canvas::gtDrawArray(DRAW_MODE _mode, int _first, int _count)
	{
		Point pt0, pt1,pt2;
		// ȡ��������
		byte* _vertexData = m_state.m_vertexData.m_data+_first*m_state.m_vertexData.m_stride;
		// ȡ��ɫ����
		byte* _colorData = m_state.m_colorData.m_data + _first * m_state.m_colorData.m_stride;
		// ȡuv��������
		byte* _texCoordData = m_state.m_texCoordData.m_data + _first * m_state.m_texCoordData.m_stride;

		_count = _count - _first;

		switch(_mode)
		{
		case GT_LINE:
		{
			_count = _count / 2;
			for (int i = 0; i < _count ; i ++)
			{
				// ȡ��������
				float* _vertexDataFloat = (float*)_vertexData;
				pt0.m_x = _vertexDataFloat[0];
				pt0.m_y = _vertexDataFloat[1];
				_vertexData += m_state.m_vertexData.m_stride;	// �Ӳ�����ָ����һ������

				_vertexDataFloat = (float*)_vertexData;
				pt1.m_x = _vertexDataFloat[0];
				pt1.m_y = _vertexDataFloat[1];
				_vertexData += m_state.m_vertexData.m_stride;

				// ȡ��ɫ����
				RGBA* _colorDataRGBA = (RGBA*)_colorData;
				pt0.m_color = _colorDataRGBA[0];
				_colorData += m_state.m_vertexData.m_stride;

				_colorDataRGBA = (RGBA*)_colorData;
				pt1.m_color = _colorDataRGBA[0];
				_colorData += m_state.m_vertexData.m_stride;

				drawLine(pt0, pt1);
			}
			
		}
			break;

		case GT_TRIANGLE:
			_count = _count / 3;
			for (int i = 0; i < _count; i++)
			{
				// ȡ��������
				float* _vertexDataFloat = (float*)_vertexData;
				pt0.m_x = _vertexDataFloat[0];
				pt0.m_y = _vertexDataFloat[1];
				_vertexData += m_state.m_vertexData.m_stride;		// �Ӳ���

				_vertexDataFloat = (float*)_vertexData;
				pt1.m_x = _vertexDataFloat[0];
				pt1.m_y = _vertexDataFloat[1];
				_vertexData += m_state.m_vertexData.m_stride;	// �Ӳ���

				_vertexDataFloat = (float*)_vertexData;
				pt2.m_x = _vertexDataFloat[0];
				pt2.m_y = _vertexDataFloat[1];
				_vertexData += m_state.m_vertexData.m_stride;	//�Ӳ���

				// ȡ��ɫ����
				RGBA* _colorDataRGBA = (RGBA*)_colorData;
				pt0.m_color = _colorDataRGBA[0];
				_colorData += m_state.m_colorData.m_stride;

				_colorDataRGBA = (RGBA*)_colorData;
				pt1.m_color = _colorDataRGBA[0];
				_colorData += m_state.m_colorData.m_stride;

				_colorDataRGBA = (RGBA*)_colorData;
				pt2.m_color = _colorDataRGBA[0];
				_colorData += m_state.m_colorData.m_stride;

				// ȡuv��������
				floatV2* _uvData = (floatV2*)_texCoordData;
				pt0.m_uv = _uvData[0];
				_texCoordData += m_state.m_texCoordData.m_stride;	// �Ӳ���

				_uvData = (floatV2*)_texCoordData;
				pt1.m_uv = _uvData[0];
				_texCoordData += m_state.m_texCoordData.m_stride;	// �Ӳ���

				_uvData = (floatV2*)_texCoordData;
				pt2.m_uv = _uvData[0];
				_texCoordData += m_state.m_texCoordData.m_stride;	// �Ӳ���


				drawTriangleAny(pt0, pt1, pt2);
			}
			break;
		default:
			break;
		}
	}

}