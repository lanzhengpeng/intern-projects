### 【1225】export selected geometry to iges 将选定几何体导出为IGES

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        int  
            n,  
            resp;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            cpn[MAX_FSPEC_SIZE+1],  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            *cli_name,  
            *dfn,  
            filter[MAX_FSPEC_SIZE+1] = { "*.igs" },  
            ifn[MAX_FSPEC_SIZE+1],  
            lfn[MAX_FSPEC_SIZE+1],  
            ofn[MAX_FSPEC_SIZE+1],  
            *prog,  
            *shell,  
            pfn[MAX_FSPEC_SIZE+1];  
    #if defined(_WIN32)  
        prog = "%UGII_BASE_DIR%\\IGES\\iges.cmd";  
        dfn = "%UGII_BASE_DIR%\\IGES\\igesexport.def";  
        shell = "cmd.exe /c";  
    #else  
        prog = "$UGII_BASE_DIR/iges/iges";  
        dfn = "$UGII_BASE_DIR/iges/igesexport.def";  
        shell = "/bin/ksh -c";  
    #endif  
    /*  This does the whole part first - just to show how that works 里海译:翻译：首先完成整个部分 - 只是为了展示这个过程是如何工作的。 */  
    /*  the translator reads the last saved part not the active one 里海译:翻译器读取的是最后保存的部分，而不是活动部分。 */  
        if (UF_PART_is_modified(part)) UF_CALL(UF_PART_save());  
        UF_CALL(UF_PART_ask_part_name(part, cpn));  
        build_similar_temp_filespec(cpn, 37, ifn);  
        build_similar_temp_filespec(cpn, 47, lfn);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            UF_CALL(UF_UGMGR_convert_file_name_to_cli(cpn, &cli_name));  
            sprintf(cmd, "%s \"\"%s\" -pim:yes o=\"%s\" d=\"%s\" l=\"%s\" \"%s\"\"",  
                shell, prog, ifn, dfn, lfn, cli_name);  
            UF_free(cli_name);  
        }  
        else  
        {  
            sprintf(cmd, "%s \"\"%s\" o=\"%s\" d=\"%s\" l=\"%s\" \"%s\"\"",  
                shell, prog, ifn, dfn, lfn, cpn);  
        }  
        ECHO("%s\n", cmd);  
        system(cmd);  
    /*  Done the entire part, now do just selected geometry 里海译:翻译：完成整个部分，现在只做选定的几何体。 */  
        while (((n = select_objects("Select objects to export", &objects)) > 0) &&  
            !UF_CALL(UF_UI_create_filebox("Specify IGES file to export into",  
                "IGES file", filter, "", ofn, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            build_similar_temp_filespec(ofn, 47, lfn);  
            build_similar_temp_filespec(ofn, 2, pfn);  
            sprintf(cmd, "%s \"\"%s\" o=\"%s\" d=\"%s\" l=\"%s\" \"%s\"\"",  
                shell, prog, ofn, dfn, lfn, pfn);  
            if (!UF_CALL(UF_PART_export(pfn, n, objects)))  
            {  
                ECHO("%s\n", cmd);  
                system(cmd);  
                uc4561(pfn, 0);  
            }  
            UF_free(objects);  
        }  
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

> 这段NX二次开发代码的主要功能是导出NX零件中的几何体到IGES文件。以下是代码的主要内容和流程：
>
> 1. 定义了一些宏和函数，用于错误报告和对象选择。
> 2. do_it函数实现了导出功能：首先，判断当前显示的零件是否已保存，若未保存则保存。构建输入和日志文件的路径，使用系统命令调用NX的IGES导出命令，将整个零件导出到IGES文件。循环提示用户选择要导出的几何体，若用户选择了对象，则构建输出和日志文件的路径，使用UF_PART_export函数将选定几何体导出到临时文件，然后调用系统命令将临时文件转换为IGES文件。在每次循环结束后释放内存。
> 3. 首先，判断当前显示的零件是否已保存，若未保存则保存。
> 4. 构建输入和日志文件的路径，使用系统命令调用NX的IGES导出命令，将整个零件导出到IGES文件。
> 5. 循环提示用户选择要导出的几何体，若用户选择了对象，则构建输出和日志文件的路径，使用UF_PART_export函数将选定几何体导出到临时文件，然后调用系统命令将临时文件转换为IGES文件。
> 6. 在每次循环结束后释放内存。
> 7. ufusr函数是NX二次开发程序的入口函数，调用do_it函数实现功能。
> 8. ufusr_ask_unload函数用于在插件卸载时被调用。
>
> 总体来看，这段代码实现了NX零件的IGES导出功能，通过系统命令调用实现了批量处理，并通过对象选择实现了部分几何体的导出。
>
