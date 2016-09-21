#pragma once

#include <cv.h>  
#include <highgui.h>  
#include <ft2build.h>  
#include FT_FREETYPE_H  

class CvxText
{
	// ��ֹcopy  

	CvxText& operator=(const CvxText&);

	//================================================================  
	//================================================================  

public:

	/**
	* װ���ֿ��ļ�
	*/

	CvxText(const char *freeType);
	virtual ~CvxText();

	//================================================================  
	//================================================================  

	/**
	* ��ȡ���塣Ŀǰ��Щ�����в�֧�֡�
	*
	* \param font        ��������, Ŀǰ��֧��
	* \param size        �����С/�հױ���/�������/��ת�Ƕ�
	* \param underline   �»���
	* \param diaphaneity ͸����
	*
	* \sa setFont, restoreFont
	*/

	void getFont(int *type,
		CvScalar *size = NULL, bool *underline = NULL, float *diaphaneity = NULL);

	/**
	* �������塣Ŀǰ��Щ�����в�֧�֡�
	*
	* \param font        ��������, Ŀǰ��֧��
	* \param size        �����С/�հױ���/�������/��ת�Ƕ�
	* \param underline   �»���
	* \param diaphaneity ͸����
	*
	* \sa getFont, restoreFont
	*/

	void setFont(int *type,
		CvScalar *size = NULL, bool *underline = NULL, float *diaphaneity = NULL);

	/**
	* �ָ�ԭʼ���������á�
	*
	* \sa getFont, setFont
	*/

	void restoreFont();

	//================================================================  
	//================================================================  

	/**
	* �������(��ɫĬ��Ϊ��ɫ)����������������ַ���ֹͣ��
	*
	* \param img  �����Ӱ��
	* \param text �ı�����
	* \param pos  �ı�λ��
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1��
	*/

	int putText(IplImage *img, const char    *text, CvPoint pos);

	/**
	* �������(��ɫĬ��Ϊ��ɫ)����������������ַ���ֹͣ��
	*
	* \param img  �����Ӱ��
	* \param text �ı�����
	* \param pos  �ı�λ��
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1��
	*/

	int putText(IplImage *img, const wchar_t *text, CvPoint pos);

	/**
	* ������֡���������������ַ���ֹͣ��
	*
	* \param img   �����Ӱ��
	* \param text  �ı�����
	* \param pos   �ı�λ��
	* \param color �ı���ɫ
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1��
	*/

	int putText(IplImage *img, const char    *text, CvPoint pos, CvScalar color);

	/**
	* ������֡���������������ַ���ֹͣ��
	*
	* \param img   �����Ӱ��
	* \param text  �ı�����
	* \param pos   �ı�λ��
	* \param color �ı���ɫ
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1��
	*/
	int putText(IplImage *img, const wchar_t *text, CvPoint pos, CvScalar color);

	//================================================================  
	//================================================================  

private:

	// �����ǰ�ַ�, ����m_posλ��  

	void putWChar(IplImage *img, wchar_t wc, CvPoint &pos, CvScalar color);

	//================================================================  
	//================================================================  

private:

	FT_Library   m_library;   // �ֿ�  
	FT_Face      m_face;      // ����  

	//================================================================  
	//================================================================  

	// Ĭ�ϵ������������  

	int         m_fontType;
	CvScalar   m_fontSize;
	bool      m_fontUnderline;
	float      m_fontDiaphaneity;

	//================================================================  
	//================================================================  
};