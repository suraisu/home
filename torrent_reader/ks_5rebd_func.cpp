#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bencode.c"
#include "windows.h"
#include "wchar.h"
#include <inttypes.h>
#include "io.h"



char *w2t(const wchar_t* unicode)
{
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
    char *szUtf8 = (char*)malloc(len + 1);
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
    return szUtf8;
}
wchar_t *t2w(const char* szU8)
{
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, 0, szU8, strlen(szU8), NULL, 0);
    wchar_t* wszString = (wchar_t*)malloc(sizeof(wchar_t)*(wcsLen + 1));
    ::MultiByteToWideChar(CP_UTF8, 0, szU8, strlen(szU8), wszString, wcsLen);
    wszString[wcsLen] = '\0';
    return wszString;
}





typedef struct file * pfile;
typedef struct file {
	char *name;
    char *sha1;
    char *path;
    long long size;
    int color;
    pfile next;
    pfile pre;
} tfile;




void initfile(pfile me)
{
    me->next=NULL;
    me->name=NULL;
    me->path=NULL;
    me->sha1=NULL;
    me->size=0;
    me->color=1;
}


void wmakepath(char *p)
{
	int j;
	for(j=1;j<strlen(p);++j)
	if(p[j]=='/')
	{
		char tmp[j+1];tmp[j]=0;
		strncpy(tmp,p,j);
		_wmkdir(t2w(tmp));
	}
}

char * tpath(char *t,char *p)
{
	int a1=strlen(t);
	int a2=strlen(p);
	char *cc=(char *)malloc(sizeof(char)*(a1+a2+2));cc[a1+a2+1]=0;
	int i;
	for (i=0;i<a1;++i)
		cc[i]=t[i];
	cc[i]='/';
	for (i=0;i<a2;++i)
		cc[i+a1+1]=p[i];
	return cc;
}

pfile wtfw_pdir(pfile me,wchar_t *dir);
pfile wtfw_file(pfile me,wchar_t *dir,wchar_t *base,long long size);
pfile wtfw_pdir(pfile me,wchar_t *dir)
{
	int i,l1=wcslen(dir)+2;
	wchar_t dirr[l1+1];dirr[l1]=0;
	wcsncpy(dirr,dir,l1-2);
	dirr[l1-1]=L'*';
	dirr[l1-2]=L'/';
	long handle,khandle=1;  
	_wfinddata64_t fileinto;  
	handle = _wfindfirst64(dirr, &fileinto); 
	if(handle!=-1)
		khandle=0;
	while(!khandle)
	{
		if(wcscmp(fileinto.name,L"."))
		if(wcscmp(fileinto.name,L".."))
		{
		int l2=wcslen(dir)+wcslen(fileinto.name)+1;
		wchar_t dirrr[l2+1];dirrr[l2]=0;
		for(i=0;i<wcslen(dir);++i)
			dirrr[i]=dir[i];
		dirrr[i]=L'/';
		for(++i;i<l2;++i)
			dirrr[i]=fileinto.name[i-wcslen(dir)-1];
		if(fileinto.attrib&_A_SUBDIR)
		{

			me=wtfw_pdir(me,dirrr);
		} 
		else
		{
			me=wtfw_file(me,dirrr,fileinto.name,fileinto.size);
		}
		}
	   khandle=_wfindnext64(handle, &fileinto);
	}
	return me;
}
pfile wtfw_file(pfile me,wchar_t *dir,wchar_t *base,long long size)
{

	char *tf=w2t(base);
	int tfl=strlen(tf);
	if(size>0)
	if(tfl>5)
	if(tf[tfl-1]=='s')
	if(tf[tfl-2]=='t')
	if(tf[tfl-3]=='2')
	if(tf[tfl-4]=='m')
	if(tf[tfl-5]=='.')
	{
		pfile nf=(pfile)malloc(sizeof(tfile));
		me->next=nf;
		me=nf;
		initfile(me);
		me->path=w2t(dir);
		me->name=w2t(base);
		me->size=size;
	}
	free(tf);
	return me;
}


