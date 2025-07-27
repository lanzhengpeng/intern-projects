### 【2228】report non-master files of part revision 报告零件修订的非主文件

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static void report_part_rev_files(UF_UGMGR_tag_t revision)  
    {  
        int  
            ii,  
            file_count;  
        char  
            revision_id[UF_UGMGR_NAME_SIZE+1],  
            **file_types,  
            **file_names;  
        UF_CALL(UF_UGMGR_ask_part_revision_id(revision, revision_id));  
        UF_CALL(UF_UGMGR_list_part_rev_files(revision, &file_count,  
            &file_types, &file_names));  
        ECHO("\tRev %s contains %d non-master file(s).\n", revision_id,  
            file_count);  
        for (ii = 0; ii < file_count; ii++)  
        {  
            ECHO("\t\t%s - %s\n", file_names[ii],file_types[ii]);  
        }  
        UF_free_string_array(file_count, file_types);  
        UF_free_string_array(file_count, file_names);  
    }  
    static UF_UGMGR_tag_t ask_ugmgr_part_tag(tag_t part)  
    {  
        char  
            encoded_name[MAX_FSPEC_SIZE + 1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        UF_UGMGR_tag_t  
            db_part_tag;  
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        UF_CALL(UF_UGMGR_ask_part_tag(part_number, &db_part_tag));  
        return db_part_tag;  
    }  
    static void do_it(void)  
    {  
        logical  
            active;  
        int  
            ii,  
            revision_count;  
         tag_t  
            part = UF_PART_ask_display_part();  
         UF_UGMGR_tag_t  
            db_part_tag,  
            *revisions;  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active)  
        {  
            ECHO("this is a not an NX Manager session\n");  
            return;  
        }  
        db_part_tag = ask_ugmgr_part_tag(part);  
        UF_CALL(UF_UGMGR_list_part_revisions(db_part_tag, &revision_count,  
            &revisions));  
        ECHO("Part has %d revisions.\n", revision_count);  
        for (ii = 0; ii < revision_count; ii++)  
            report_part_rev_files(revisions[ii]);  
        UF_free(revisions);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要功能是查询并显示当前显示部件的所有版本及其包含的非主文件。具体包括以下关键内容：
>
> 1. 宏定义：通过宏定义UF_CALL，将NX函数调用包装起来，用于在调用失败时输出错误信息。
> 2. 错误报告函数：定义了report_error函数，用于在NX函数调用失败时输出错误代码、错误信息以及调用代码行。
> 3. 查询版本信息函数：定义了report_part_rev_files函数，用于查询指定版本的部件信息，并输出包含的非主文件类型和名称。
> 4. 获取部件标签函数：定义了ask_ugmgr_part_tag函数，用于根据部件名称查询NX Manager数据库中的部件标签。
> 5. 主函数：定义了do_it函数，用于查询当前显示部件的所有版本，并调用report_part_rev_files函数输出每个版本包含的非主文件。
> 6. NX启动和结束函数：定义了ufusr函数，用于在NX启动时调用do_it函数，并在结束时关闭NX。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在NX卸载时立即卸载该代码。
>
> 综上，这段代码实现了查询和显示NX部件版本及其包含文件的功能，通过封装NX API调用，实现了版本信息的查询和错误处理。
>
