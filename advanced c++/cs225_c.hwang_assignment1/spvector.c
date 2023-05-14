/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: spvector.c
Purpose: to manage linked list
Language: c
Platform: Visual Studio 15.8.1 w/ C++17, Window64bit
Project: c.hwang_CS225_assignment1
Author: HwangChanIl, c.hwang, 5415279
Creation date: 09/29/2018
- End Header ----------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"

/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode {
  int    data;
  int    pos;
  struct ElementNode* next;
};

struct RowNode {
  int pos;
  struct ElementNode* elements;
  struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;


/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char *fmt, int dim) {
	int i,last_pos=-1;
	while (p_e) {
		for (i=last_pos+1;i<p_e->pos;++i) { printf(fmt,0); }
		printf(fmt,p_e->data);
		last_pos=p_e->pos;
		p_e = p_e->next;
	}
	for (i=last_pos+1;i<dim;++i) { printf(fmt,0); }
}

void print_elements(ConstElementNode_handle p_e) {
	while (p_e) {
		printf("%i at pos %i, ",p_e->data,p_e->pos);
		p_e = p_e->next;
	}
}

int insert_element(ElementNode_handle * p_e,int pos,int val)
{
	ElementNode_handle temp = (*p_e);
	ElementNode_handle prev = 0;

	if(val != 0)
	{
		if(!temp)
		{
			temp = (ElementNode_handle)malloc(sizeof(ElementNode));
			if (!temp)
			{
				return 1;
			}
			else
			{
				(*p_e) = temp;
			}
			temp->pos = pos;
			temp->data = val;
			temp->next = NULL;
		}
		else
		{
			while(1)
			{
				if(temp->pos < pos)
				{
					if(temp->next)
					{
						prev = temp;
						temp = temp->next;
					}
					else
					{
						temp->next = (ElementNode_handle)malloc(sizeof(ElementNode));
						if (!temp)
						{
							return 1;
						}
						temp->next->pos = pos;
						temp->next->data = val;
						temp->next->next = NULL;
						break;
					}
				}
				else if(temp->pos == pos)
				{
					temp->data = val;
					break;
				}
				else
				{
					if(temp == (*p_e))
					{
						temp = (ElementNode_handle)malloc(sizeof(ElementNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							temp->next = (*p_e);
							(*p_e) = temp;
						}
						temp->pos = pos;
						temp->data = val;
						break;
					}
					else
					{
						prev->next = (ElementNode_handle)malloc(sizeof(ElementNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							prev->next->next = temp;
						}
						prev->next->pos = pos;
						prev->next->data = val;
						break;
					}
			}

			}
		}
	}

	return 0;
}

void delete_element( ElementNode_handle *p_e,int pos )
{
	ElementNode_handle temp = (*p_e);
	ElementNode_handle prev = 0;

	while (temp)
	{
		if (temp->pos == pos)
		{
			if (temp == (*p_e))
			{
				(*p_e) = temp->next;
				free(temp);
			}
			else
			{
				prev->next = temp->next;
				free(temp);
			}
			break;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}

int  get( ConstElementNode_handle p_e, int pos )
{
	while(p_e)
	{
		if(pos == p_e->pos)
		{
			return p_e->data;
		}
		else if(pos < p_e->pos)
		{
			return 0;
		}
		else
		{
			p_e = p_e->next;
		}
	}
	return 0;
}

int scalar_product( ConstElementNode_handle p_e1, ConstElementNode_handle p_e2 )
{
	int val = 0;
	while(p_e1 && p_e2)
	{
		if(p_e1->pos == p_e2->pos)
		{
			val += p_e1->data * p_e2->data;
			p_e2 = p_e2->next;
			p_e1 = p_e1->next;
		}
		else if(p_e1->pos > p_e2->pos)
		{
			p_e2 = p_e2->next;
		}
		else
		{
			p_e1 = p_e1->next;
		}
	}
	return val;
}

ElementNode_handle add( ConstElementNode_handle p_e1,ConstElementNode_handle p_e2 )
{
	ElementNode_handle p_e = 0;

	while (p_e1 && p_e2)
	{
		if (p_e1->pos == p_e2->pos)
		{
			insert_element(&p_e, p_e2->pos, p_e1->data + p_e2->data);
			p_e2 = p_e2->next;
			p_e1 = p_e1->next;
		}
		else if (p_e1->pos > p_e2->pos)
		{
			insert_element(&p_e, p_e2->pos, p_e2->data);
			p_e2 = p_e2->next;
		}
		else
		{
			insert_element(&p_e, p_e1->pos, p_e1->data);
			p_e1 = p_e1->next;
		}
	}

	while (p_e1)
	{
		insert_element(&p_e, p_e1->pos, p_e1->data);
		p_e1 = p_e1->next;
	}

	while (p_e2)
	{
		insert_element(&p_e, p_e2->pos, p_e2->data);
		p_e2 = p_e2->next;
	}

	return p_e;
}

void free_elements( ElementNode_handle p_e )
{
	ElementNode_handle temp = 0;
	while(p_e)
	{
		temp = p_e;
		p_e = p_e->next;
		free(temp);
	}
}

void print_rows(ConstRowNode_handle p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim) {
	int i,j,last_pos=-1;
	while (p_r) {
		for (i=last_pos+1;i<p_r->pos;++i) { /* output row of 0's */
			for (j=0;j<dim;++j) printf(fmt,0); 
			printf("\n");
		}
		printf_elements(p_r->elements,fmt,dim);
		printf("\n");
		last_pos=p_r->pos;
		p_r = p_r->next;
	}
	for (i=last_pos+1;i<dim;++i) { /* output row of 0's */
		for (j=0;j<dim;++j) printf(fmt,0); 
		printf("\n");
	}
}

int insert_row(RowNode_handle *p_r, int pos, ElementNode_handle p_e)
{
	RowNode_handle temp = (*p_r);
	RowNode_handle prev = 0;

	if (p_e)
	{
		if (!temp)
		{
			temp = (RowNode_handle)malloc(sizeof(RowNode));
			if (!temp)
			{
				return 1;
			}
			else
			{
				(*p_r) = temp;
			}
			temp->pos = pos;
			temp->elements = p_e;
			temp->next = NULL;
		}
		else
		{
			while (1)
			{
				if (temp->pos < pos)
				{
					if (temp->next)
					{
						prev = temp;
						temp = temp->next;
					}
					else
					{
						temp->next = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						temp->next->pos = pos;
						temp->next->elements = p_e;
						temp->next->next = NULL;
						break;
					}
				}
				else if (temp->pos == pos)
				{
					return 2;
				}
				else
				{
					if (temp == (*p_r))
					{
						temp = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							temp->next = (*p_r);
							(*p_r) = temp;
						}
						temp->pos = pos;
						temp->elements = p_e;
						break;
					}
					else
					{
						prev->next = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							prev->next->next = temp;
						}
						prev->next->pos = pos;
						prev->next->elements = p_e;
						break;
					}
				}

			}
		}
	}

	return 0;
}

int insert_element2( RowNode_handle *pp_r,int pos_row,int pos_col,int val )
{
	RowNode_handle temp = (*pp_r);
	RowNode_handle prev = 0;

	if (val != 0)
	{
		if (!temp)
		{
			temp = (RowNode_handle)malloc(sizeof(RowNode));
			if (!temp)
			{
				return 1;
			}
			else
			{
				(*pp_r) = temp;
			}
			temp->pos = pos_row;
			temp->elements = 0;
			insert_element(&temp->elements, pos_col, val);
			temp->next = NULL;
		}
		else
		{
			while(1)
			{
				if (temp->pos < pos_row)
				{
					if (temp->next)
					{
						prev = temp;
						temp = temp->next;
					}
					else
					{
						temp->next = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						temp->next->pos = pos_row;
						temp->next->elements = 0;
						insert_element(&(temp->next)->elements, pos_col, val);
						temp->next->next = NULL;
						break;
					}
				}
				else if (temp->pos == pos_row)
				{
					insert_element(&temp->elements, pos_col, val);
					break;
				}
				else
				{
					if (temp == (*pp_r))
					{
						temp = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							temp->next = (*pp_r);
							(*pp_r) = temp;
						}
						temp->pos = pos_row;
						temp->elements = 0;
						insert_element(&temp->elements, pos_col, val);
					}
					else
					{
						prev->next = (RowNode_handle)malloc(sizeof(RowNode));
						if (!temp)
						{
							return 1;
						}
						else
						{
							prev->next->next = temp;
						}
						prev->next->pos = pos_row;
						prev->next->elements = 0;
						insert_element(&temp->elements, pos_col, val);
					}
				}
			}
		}
	}
	return 0;
}

RowNode_handle find_row( RowNode_handle *pp_r,int pos )
{
	RowNode_handle temp = (*pp_r);
	while (temp->pos == pos)
	{
		temp = temp->next;
	}
	return (*pp_r);
}

RowNode_handle transpose( ConstRowNode_handle p_r )
{
	RowNode_handle* tp_r = 0;
	int i, j;

	for (i = 0; p_r ; i++)
	{
		for (j = 0; ; j++)
		{
		}
	}

	return (*tp_r);
}

RowNode_handle mult( ConstRowNode_handle p_r1, ConstRowNode_handle p_r2 )
{
	RowNode_handle* tp_r = 0;

	if(p_r1 && p_r2)
	{

	}

	return (*tp_r);
}

void free_rows( RowNode_handle p_r )
{
	RowNode_handle temp = 0;
	ElementNode_handle temp2 = 0;
	while (p_r)
	{
		temp = p_r;
		while(p_r->elements)
		{
			temp2 = p_r->elements;
			p_r->elements = p_r->elements->next;
			free_elements(temp2);
		}
		p_r = p_r->next;
		free(temp);
	}
}


int  determinant(ConstRowNode_handle p_r,int dim) {
	/* this is a meaningless code to get rid of "unsed argument" warnings in 
	 * Borland an MS */
	int result=1;
	if (dim==0) ++result;
	if (p_r)    --result;
	return result;
}
