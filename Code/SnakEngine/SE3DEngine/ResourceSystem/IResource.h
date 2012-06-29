//�̳б�����ʵ���ļ� ���� ��д
//����
//load�ӿڶ�ȡ��Դ
//prepare 
//���̴߳�����Դ
//����ȡ ���ڶ�ȡ�õ�stl �̲߳���ȫ ����һ���߳���
#ifndef _Resource_H_
#define _Resource_H_

#include <SE3DEngine/Comm/Def.h>

namespace SE
{
	enum ELoadState
	{
		LOADSTATE_Unload,
		LOADSTATE_Loaded,
		LOADSTATE_Loading,
		LOADSTATE_Unloading,
		LOADSTATE_Prepared


	};
	enum ELoadType
	{
		LT_Read_Only,
		LT_Write_Only,
		LT_Read_Write
	};
	//��ݶ���
	struct _DLL_API IRes 
	{
 

		virtual void SetResName(String const & sName) = 0;
		virtual String const & GetResName() const = 0;

		virtual void Prepare(bool bBackgroundThread=false) = 0;
		virtual void Load(bool bBackgroundThread=false) = 0;
		virtual void Reload() = 0;
		virtual void Unload() = 0;
		virtual bool IsLoad()const = 0;
		virtual bool IsPrepare()const = 0;
		virtual CFileDataStreamPtr Open(String const& sFileName, bool bReadOnly=true) const =0;
		virtual CFileDataStreamPtr Create(String const &sFileName) const = 0 ;
		virtual void Remove(String &sFileName) const = 0;
		virtual void Exists(String const&sFileName) = 0;



		virtual ELoadState GetLoadState()const = 0;
		virtual SEResourceManagerPtr getCreator() = 0;
 
	};
 
#endif
