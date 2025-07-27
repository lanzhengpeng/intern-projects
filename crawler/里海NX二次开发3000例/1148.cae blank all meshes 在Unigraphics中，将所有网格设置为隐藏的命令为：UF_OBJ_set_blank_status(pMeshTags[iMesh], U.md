### cae blank all meshes 在Unigraphics中，将所有网格设置为隐藏的命令为：UF_OBJ_set_blank_status(pMeshTags[iMesh], U

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** NXOpen_Wizard1.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:Include files翻译为：包含文件。 */  
    #include <stdarg.h>  
        #include <stdio.h>  
    #include <uf.h>  
    #include <uf_sf.h>  
    #include <uf_obj.h>  
    #include <uf_sf_enums.h>  
    #include <uf_ui.h>  
    #include <uf_ui_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_NCHARS+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_NCHARS, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void do_it(void)  
    {  
        int x,    
            n;  
        tag_t  
            object;  
           tag_t* mesh_pointer = NULL;  
           object = NULL_TAG;  
           ECHO("Beginning\n");  
           // Internally the representation of meshes is done through several objects, this   
           // locate function returns one of nondisplayable objects and can't be blanks.   
           // This is wrong and should have a PR filed against it.   
           UF_CALL(UF_SF_locate_all_meshes(object, &n, &mesh_pointer));  
           ECHO("after UF_SF_locate_all_meshes\n");  
           ECHO("number of meshes %d \n" , n);  
           ECHO("Tag of mes_pointer= %d \n", mesh_pointer[0]);  
           uc1601("pause",1);  
           for ( x = 0; x < n; x ++)  
           {  
               // we can convert this nondisplayable one into the displayable mesh with this find call   
               int iMesh,  nMeshes;  
               tag_t object;  
               tag_t* pMeshTags = NULL;  
               object = NULL_TAG;  
               // Use this find to convert this tag to the displayable mesh  
               UF_CALL(UF_SF_find_mesh(mesh_pointer[x], UF_SF_DIMENSION_ANY,&nMeshes, &pMeshTags));  
               ECHO("after UF_SF_find_mesh\n");  
               ECHO("number of meshes %d \n" , nMeshes);  
               ECHO("Tag of mes_pointer= %d \n", pMeshTags[0]);  
               uc1601("pause",1);  
               for ( iMesh = 0; iMesh < nMeshes; iMesh ++)  
               {  
                     UF_CALL(UF_OBJ_set_blank_status(pMeshTags[iMesh], UF_OBJ_BLANKED) );  
                     ECHO("called UF_OBJ_set_blank_status at least %d times\n", iMesh);  
               }  
           }  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        /* Initialize the API environment 译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 译:Failed to initialize */  
            return;  
        }  
        do_it();  
        /* TODO: Add your application code here 译:翻译：在此添加您的应用程序代码。 */  
        /* Terminate the API environment 译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发示例，主要功能包括：
>
> 1. 初始化和终止环境：在ufusr函数中调用UF_initialize和UF_terminate来初始化和终止NX API环境。
> 2. 查找并隐藏网格：在do_it函数中，首先调用UF_SF_locate_all_meshes查找所有网格，然后调用UF_SF_find_mesh将不可见的网格转换为可见网格，最后调用UF_OBJ_set_blank_status隐藏网格。
> 3. 错误处理：使用宏UF_CALL包装UF函数调用，并在report_error中打印错误代码和消息。
> 4. 输出日志：使用ECHO宏输出日志信息到UG的日志窗口和系统日志。
> 5. 激活方式：通过ufusr函数作为显式激活的方式运行代码。
> 6. 卸载处理：通过ufusr_ask_unload函数处理卸载请求。
>
> 该代码的目的是演示如何在NX中查找并隐藏所有网格，同时包含了错误处理和日志输出等辅助功能。
>
