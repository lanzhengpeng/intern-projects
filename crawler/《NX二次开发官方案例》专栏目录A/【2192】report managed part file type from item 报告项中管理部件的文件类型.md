### 【2192】report managed part file type from item 报告项中管理部件的文件类型

#### 代码

```cpp
    /* Include files 里海译:Include files翻译为：包含文件 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[256];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), 255, format, args);  
        va_end(args);  
        printf(msg);  
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
    static void do_it(const char *param)  
    {  
        char  
            sPartNumber[UF_UGMGR_PARTNO_BUFSIZE ],   
            sPartRevision[UF_UGMGR_PARTREV_BUFSIZE],   
            sPartFileType[UF_UGMGR_FTYPE_BUFSIZE],  
            sPartFileName[UF_UGMGR_FNAME_BUFSIZE],  
            itemid[UF_UGMGR_NAME_SIZE+1] = "",  
            encoded_name[MAX_FSPEC_SIZE+1];  
        UF_UGMGR_tag_t   
            db_tag = UF_UGMGR_null_tag,  
            database_part_tag = UF_UGMGR_null_tag,  
            part_rev_tag = UF_UGMGR_null_tag;  
        strcpy_s(itemid, UF_UGMGR_NAME_SIZE, param);  
        if( !UF_CALL(UF_UGMGR_ask_part_tag(itemid, &db_tag)) && db_tag != UF_UGMGR_null_tag &&  
            !UF_CALL(UF_UGMGR_ask_configured_rev(db_tag, &part_rev_tag)) && part_rev_tag != UF_UGMGR_null_tag &&  
            !UF_CALL(UF_UGMGR_ask_part_revision_id(part_rev_tag, sPartRevision))  )  
        {  
            UF_CALL(UF_UGMGR_encode_part_filename(itemid, sPartRevision, NULL, NULL, encoded_name));  
            UF_CALL( UF_UGMGR_decode_part_file_name(encoded_name, sPartNumber, sPartRevision, sPartFileType, sPartFileName) );  
            ECHO("UF_UGMGR_ask_part_tag returned <%d>\n", db_tag);  
            ECHO("UF_UGMGR_ask_configured_rev returned <%d>\n", part_rev_tag);  
            ECHO("UF_UGMGR_ask_part_revision_id returned <%s>\n", sPartRevision);  
            ECHO("UF_UGMGR_decode_part_file_name returned part file type <%s>\n", sPartFileType);  
        }  
        else  
            ECHO("DB Tag for Part Number <%s> not found\n", itemid);  
    }  
    extern void main( char argc, const char *argv[] )  
    {  
        int   
            rc;  
        char   
            *result,  
            len,  
            itembuf[100];  
        if (!UF_CALL(UF_UGMGR_initialize(argc, argv)))  
        {  
            ECHO("Enter Part Number (Item ID):");  
            result = fgets(itembuf, sizeof(itembuf), stdin);  
            len = strlen(itembuf);  
            if (result != NULL)  
            {  
                if (itembuf[len-1] == '\n')    itembuf[len-1] = '\0';  
                do_it(itembuf);  
            }  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的一个示例，用于演示如何使用NX Open C++ API获取NX部件的信息。下面是对代码的介绍：
>
> 1. 包含了必要的头文件，如stdio.h、string.h、uf.h等，这些头文件包含了C语言标准库函数以及NX Open C++ API的定义。
> 2. 定义了一个ECHO函数，用于打印信息到控制台和NX日志。
> 3. 定义了一个UF_CALL宏，用于调用NX Open C++ API函数，并检查返回码，如果出错则打印错误信息。
> 4. 定义了一个report_error函数，用于打印API函数调用出错时的错误码和错误信息。
> 5. 定义了一个do_it函数，用于根据部件Item ID获取部件的DB Tag、配置版本、版本号等信息，并解码部件文件名。
> 6. main函数首先初始化NX Open C++ API环境，然后提示用户输入部件Item ID，调用do_it函数获取部件信息，最后终止NX Open C++ API环境。
> 7. 代码通过示例展示了如何使用NX Open C++ API获取NX部件的基本信息，如DB Tag、版本号等，并解码部件文件名，为NX二次开发提供了参考。
> 8. 代码采用了NX Open C++ API的UF模块，该模块提供了一套统一的函数接口，用于访问NX的底层模型数据，是NX二次开发的基础。
> 9. 代码风格清晰，通过错误处理和日志输出，使得程序更加健壮。
>
