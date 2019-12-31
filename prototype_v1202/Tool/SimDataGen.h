#pragma once
#include <iostream>
#include <string>
 
using namespace std;
 
typedef struct SIMRE_LIB_PARAM{
    //�źŲ���-���ݽṹ 
	// ÿ���ֲ����Ĵ���������Ŀ����Ϊ�����ظ�Ƶ�ʡ���PRF(Pulse-Recurrence-Frequency) ��ʾ
	// ������������֮���ʱ��������Ϊ�����ظ����ڣ���PRT��ʾ�������������ظ�Ƶ�ʵĵ��� 
	int		nPRT;		//�״��ظ�����					�����ظ�Ƶ��ȡ4K, �ظ�����Ϊ�䵹�� 
	int		nPW;		//����   						����10us			
	int		nPulse;		//һ�������е����������		16
	int		nBand;		//��Ƶ����						��Ƶ����50M 
	int		nDeltaf;	//��Ƶ������Ƶ��				??
	int		nSampling;	//������						������60MHz

	int		nGroup;		//��������						��������512
	float	nRF;		//�״���Ƶ						����Ƶ��10G=10000 
	float	amplitude;	//Ŀ��ز�����
	float	range;		//Ŀ�꾶��λ��
	float	velcity;	//Ŀ�꾶���ٶ�
	float	omega;		//Ŀ����ת��
	float	acc;		//Ŀ�꾶����ٶ�
}SIMRE_LIB_PARAM;

class SimDataGen{
public:
    // �źŲ������
    int m_band;
    int m_count;
    int m_deltaf;
    int m_num;
    float m_omega;
    int m_prt;
    int m_psWide;
    float m_range;
    int m_sampling;
    float m_acc;
    int m_frqn;
    float m_ampt;
    float m_velcity;
    string m_file;    // ����ģ���ļ�·��
    char* m_echo;    // �����������·��

    // ���ݴ洢���
    FILE* fp;
    float *px;
    float *py;
    int	pos;

public:

    SimDataGen(SIMRE_LIB_PARAM param);
    ~SimDataGen();
    float GetTimeDelay(float range);
    void GetTgtRange(float x, float y, float w, float R, float v, float a, float* t, int count, int tm, float *rge);
    void ChirpSig(int psWide, float*tbuf, int count, float* rge, float amg, float cf, float gama, int tm, float* real, float* imag);
    void ChirpRef(int psWide, float*tbuf, int count, float rge, float amg, float cf, float gama, int tm, float* real, float* imag);
    void ComplexConjMult(int count, float* Irl, float*Iig, float* frl, float* fig, float *prl, float*pig);
    unsigned int SimIsarEchoes(int nCell, unsigned int nFrame, float *pRl, float *pIg);
    void GenData();
		
};
