#ifndef _Vector2_H_
#define _Vector2_H_


class Vector2
{
public:
	union
	{
		struct 
		{
			float u,v;
		};
		float m_elem[2];
	};
	Vector2()
	{

	}
	Vector2(float a, float b)
	{
		u = a;
		v = b;
	}

};


#endif