#pragma once
#ifndef __REGION_NODE__CYAN__2021_01_26__H__
#define __REGION_NODE__CYAN__2021_01_26__H__


typedef struct _tag_node tag_node;
typedef tag_node* pENode;


struct _tag_node 
{
	int	nTag;	//联通域标记
	int	nArea;	//面积
	int nLeft;	//左右上下
	int nRight;
	int nTop;
	int nBottom;	
	unsigned long nCenterX;	//中心点
	unsigned long nCenterY;	
	pENode pNext;	//下一个节点
	
};

class CRgnNode
{
public:
	CRgnNode();
	~CRgnNode();

public:
	void CreateNode(int nTag, int x, int y);	
	pENode GetRegion(int nTag);
	int GetRegionCount();
	void AddElement(int nTag, int x, int y);

private:
	pENode m_pHeaderNode;  //头节点
	
};
#endif
