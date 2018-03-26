#include "ks_5rebd_func.cpp"
#include "sha1.c"

void mksha1(pfile me)
{
	unsigned char *lthash_string;     /* the hash string */
	unsigned char *ltpos;
	SHA_CTX ltc;
	lthash_string = (unsigned char *)malloc(SHA_DIGEST_LENGTH);

	FILE *lt4;
	unsigned char ltcc[1024*1024*10+10];
	unsigned char *ltp;
	long long lti,ltflen,ltlen,partlen;
	lt4=_wfopen(t2w(me->path),L"rb");
	free(me->path);
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
}

void wtfwks(pfile me)
{
	long long i,j;
	long handle,khandle=1;  
	_wfinddata64_t fileinto;  
	handle = _wfindfirst64((wchar_t*)L"./*", &fileinto); 
	if(handle!=-1)
		khandle=0;
	while(!khandle)
	{
		if(wcscmp(fileinto.name,L"."))
		if(wcscmp(fileinto.name,L".."))
		if(!wcscmp(fileinto.name,L"#ks"))
		if(fileinto.attrib&_A_SUBDIR)
		{
				me=wtfw_pdir(me,fileinto.name);
		} 
		else
		{
			me=wtfw_file(me,fileinto.name,fileinto.name,fileinto.size);
		}
	   khandle=_wfindnext64(handle, &fileinto);
	}
}

void wtfwall(pfile me)
{
	long long i,j;
	long handle,khandle=1;  
	_wfinddata64_t fileinto;  
	handle = _wfindfirst64((wchar_t*)L"./*", &fileinto); 
	if(handle!=-1)
		khandle=0;
	while(!khandle)
	{
		if(wcscmp(fileinto.name,L"."))
		if(wcscmp(fileinto.name,L".."))
		if(wcscmp(fileinto.name,L"#ks"))
		if(fileinto.attrib&_A_SUBDIR)
		{
				me=wtfw_pdir(me,fileinto.name);
		} 
		else
		{
			me=wtfw_file(me,fileinto.name,fileinto.name,fileinto.size);
		}
	   khandle=_wfindnext64(handle, &fileinto);
	}
}

wchar_t *hevc(const char* m2ts)
{
    int l=strlen(m2ts);
    char* rt = new char[l + 1];
    rt[l] = '\0';
    strcpy(rt,m2ts);
    rt[l-1]='c';
    rt[l-2]='v';
    rt[l-3]='e';
    rt[l-4]='h';
    wchar_t *wrt=t2w(rt);
    free(rt);
    return wrt;
}


wchar_t *hevc_bak(const char* m2ts)
{
    int l=strlen(m2ts)+4;
    char* rt = new char[l + 1];
    rt[l] = '\0';
    strcpy(rt,m2ts);
    rt[l-1]='k';
    rt[l-2]='a';
    rt[l-3]='b';
    rt[l-4]='.';
    rt[l-5]='c';
    rt[l-6]='v';
    rt[l-7]='e';
    rt[l-8]='h';
    wchar_t *wrt=t2w(rt);
    free(rt);
    return wrt;
}


main()
{
    if(_access( "#ks", 0 )==-1)
	    chdir("../");
    if(_access( "#ks", 0 )==-1)
    {
	    printf("No ksdir!\n");
	    getchar();
	    return 0;
    }


    pfile me,ks,all;
    me=(pfile)malloc(sizeof(tfile));
    pfile ksstart=me;
    initfile(me);
    wtfwks(me);
    me=(pfile)malloc(sizeof(tfile));
    pfile allstart=me;
    initfile(me);
    wtfwall(me);

    int stat=1;
    for(me=ksstart->next;me->next!=NULL;me=me->next)
    {
	if(_waccess(hevc(me->path),0)==-1)
   	{
	    printf("\nhevc lost: %s!\n",me->path);
	    stat=0;
    	}
    }
    if(!stat)
    {
        printf("\n\nPlease Check!\n");
        getchar();
        return 0;
    }
    //for(me=ksstart->next;me!=NULL;me=me->next)
//	    wprintf(L"Link: %ws \n",t2w(me->path));
    for(me=ksstart->next;me!=NULL;me=me->next)
	mksha1(me);
    for(me=allstart->next;me!=NULL;me=me->next)
	mksha1(me);
    for(ks=ksstart->next;ks!=NULL;ks=ks->next)
	for(all=allstart->next;all!=NULL;all=all->next)
	{
		if(all->color&&!strncmp(ks->sha1,all->sha1,20))
		{
			all->color=0;
			if(_waccess(hevc(all->path),0)!=-1)
			{
				if(_waccess(hevc(all->path),0)!=-1)
					_wremove(hevc_bak(all->path));
				_wrename(hevc(all->path),hevc_bak(all->path));
			}
//////////////////////////////////////////   need unicode
			if(!CreateHardLinkW(hevc(all->path),hevc(ks->path),NULL))
			{
//////////////////////////////////////////
				wprintf(L"ERR: Can't Link: %ws to %ws\n",hevc(ks->path),hevc(all->path));
				stat=0;
			}
			//wprintf(L"Link: %ws to %ws\n",hevc(ks->path),hevc(all->path));
			//stat=0;
		}
	}
//    for(me=ksstart->next;me->next!=NULL;me=me->next)
//	    printf("\n%s %s\n",me->path,me->sha1);
   if(!stat) 
       getchar();
}
