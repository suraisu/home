#include "ks_5rebd_func.cpp"
#include "sha1.c"

void mksha1(pfile me)
{
	unsigned char *lthash_string;     /* the hash string */
	unsigned char *ltpos;
	SHA_CTX ltc;
	lthash_string = (unsigned char *)malloc(SHA_DIGEST_LENGTH);

	FILE *lt4;
	unsigned char *ltcc;
	ltcc=(unsigned char*)malloc(1024*1024*10+10);
	unsigned char *ltp;
	long long lti,ltflen,ltlen,partlen;
	lt4=_wfopen(t2w(me->path),L"rb");
	//free(me->path);
	fseeko64(lt4,0,2);
	ltlen=ltflen=_ftelli64(lt4);
	fseeko64(lt4,0,0);
	if(ltflen<1024*1024*10)
		fread(ltcc,ltlen,1,lt4);
	else
	{
		ltlen=1024*1024*10;
	partlen=(ltflen-1024*1024)/9;
	//printf("\n%ld\n",ltflen);
		for(lti=0;lti<9;++lti)
		{
	//printf("\n%ld\n",lti*partlen);
			ltp=ltcc+lti*1024*1024;
			fseek(lt4,lti*partlen,0);
			fread(ltp,1024*1024,1,lt4);
		}
		ltp=ltcc+9*1024*1024;
		fseek(lt4,-1024*1024,2);
		fread(ltp,1024*1024,1,lt4);
	}
	fclose(lt4);
	ltpos = lthash_string;
	SHA1_Init(&ltc);
	//printf("%s %ld %s\n",ltc,ltlen,me->path);
	SHA1_Update(&ltc, ltcc, ltlen);
	SHA1_Final(ltpos, &ltc);
	me->sha1= (char *)malloc(21);
	me->sha1[20]=0;
	memcpy(me->sha1,ltpos,20);
	//me->sha1=ltpos;
	free(ltpos);
	free(ltcc);
}

pfile *wtfw_pdir2(pfile *me,wchar_t *dir)
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

			me[0]=wtfw_pdir(me[0],dirrr);
		} 
		else
		{
			me[1]=wtfw_file(me[1],dirrr,fileinto.name,fileinto.size);
		}
		}
	   khandle=_wfindnext64(handle, &fileinto);
	}
	return me;
}

void wtfwks(pfile me1,pfile me2)
{
	long long i,j;
	pfile *me=(pfile *)malloc(sizeof(pfile)*2);
	me[0]=me1;
	me[1]=me2;
	long handle,khandle=1;  
	_wfinddata64_t fileinto;  
	handle = _wfindfirst64((wchar_t*)L"./*", &fileinto); 
	if(handle!=-1)
		khandle=0;
	while(!khandle)
	{
		if(wcscmp(fileinto.name,L"."))
		if(wcscmp(fileinto.name,L".."))
		if(fileinto.attrib&_A_SUBDIR)
		{
				wtfw_pdir2(me,fileinto.name);
		} 
	   khandle=_wfindnext64(handle, &fileinto);
	}
}

wchar_t *chapter(const char* m2ts)
{
	// hevc
	// chapter.txt
    int l=strlen(m2ts);
    char* rt = new char[l + 7 + 1];
    rt[l+7] = '\0';
    strcpy(rt,m2ts);
    rt[l+6]='t';
    rt[l+5]='x';
    rt[l+4]='t';
    rt[l+3]='.';
    rt[l+2]='r';
    rt[l+1]='e';
    rt[l]='t';
    rt[l-1]='p';
    rt[l-2]='a';
    rt[l-3]='h';
    rt[l-4]='c';
    wchar_t *wrt=t2w(rt);
    free(rt);
    return wrt;
}


wchar_t *chapter_bak(const char* m2ts)
{
    int l=strlen(m2ts)+4;
    char* rt = new char[l + 7 + 1];
    rt[l+7] = '\0';
    strcpy(rt,m2ts);
    rt[l+6]='t';
    rt[l+5]='x';
    rt[l+4]='t';
    rt[l+3]='.';
    rt[l+2]='k';
    rt[l+1]='a';
    rt[l]='b';
    rt[l-1]='.';
    rt[l-2]='r';
    rt[l-3]='e';
    rt[l-4]='t';
    rt[l-5]='p';
    rt[l-6]='a';
    rt[l-7]='h';
    rt[l-8]='c';
    wchar_t *wrt=t2w(rt);
    free(rt);
    return wrt;
}

main()
{


    pfile me,me1,me2,ks,all;
    me=NULL;
    me1=(pfile)malloc(sizeof(tfile));
    me2=(pfile)malloc(sizeof(tfile));
    initfile(me1);
    initfile(me2);
    pfile ksstart=me1;
    pfile allstart=me2;
    wtfwks(me1,me2);
   

    int stat=1;
    for(me=ksstart;me->next!=NULL;)
    {
	if(_waccess(chapter(me->next->path),0)==-1)
		me->next=me->next->next;
	else
		me=me->next;
	
    }
   for(me=allstart->next;me!=NULL;me=me->next)
	mksha1(me);
    for(me=ksstart->next;me!=NULL;me=me->next)
	mksha1(me);

   // for(ks=ksstart->next;ks!=NULL;ks=ks->next)
//	for(all=allstart->next;all!=NULL;all=all->next)
//	       wprintf(L"Link: %ws %ws \n\n",chapter(all->path),chapter(all->path));
  //  return 0;
    		
 
    if(!stat)
    {
        printf("\n\nPlease Check!\n");
        getchar();
        return 0;
    }
    //for(me=ksstart->next;me!=NULL;me=me->next)
//	    wprintf(L"Link: %ws \n",t2w(me->path));
 

    for(ks=ksstart->next;ks!=NULL;ks=ks->next)
	for(all=allstart->next;all!=NULL;all=all->next)
	{
		if(all->color&&!strncmp(ks->sha1,all->sha1,20))
		{
			all->color=0;
//////////////////////////////////////////   need unicode
			if(!CopyFileW(chapter(ks->path),chapter(all->path),false))
			{
//////////////////////////////////////////
				wprintf(L"ERR: Can't Link: %ws to %ws\n",chapter(ks->path),chapter(all->path));
				stat=0;
			}
			//wprintf(L"Link: %ws to %ws\n",chapter(ks->path),chapter(all->path));
			//stat=0;
		}
	}
//    for(me=ksstart->next;me->next!=NULL;me=me->next)
//	    printf("\n%s %s\n",me->path,me->sha1);
   if(!stat) 
       getchar();
}
