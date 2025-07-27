### 【1232】export to step203 将NX模型导出为STEP203格式

#### 代码

```cpp
    /*HEAD EXPORT_TO_STEP203 CCC UFUN */  
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
            filter[MAX_FSPEC_SIZE+1] = { "*.stp" },  
            ifn[MAX_FSPEC_SIZE+1],  
            lfn[MAX_FSPEC_SIZE+1],  
            ofn[MAX_FSPEC_SIZE+1],  
            *prog,  
            *shell,  
            pfn[MAX_FSPEC_SIZE+1];  
    #if defined(_WIN32)  
        prog = "%UGII_BASE_DIR%\\STEP203UG\\step203ug.cmd";  
        dfn = "%UGII_BASE_DIR%\\STEP203UG\\ugstep203.def";  
        shell = "cmd.exe /c";  
    #else  
        prog = "$UGII_BASE_DIR/step203ug/step203";  
        dfn = "$UGII_BASE_DIR/step203ug/ugstep203.def";  
        shell = "/bin/ksh -c";  
    #endif  
    /*  This does the whole part first - just to show how that works 里海译:This does the whole part first - just to show how that works 的翻译是：这首先完成整个部分 - 只是为了展示它是如何工作的。 */  
    /*  the translator reads the last saved part not the active one 里海译:根据提供的信息，我将翻译为：翻译器读取的是最后保存的部分，而不是活动部分。 */  
        if (UF_PART_is_modified(part)) UF_CALL(UF_PART_save());  
        UF_CALL(UF_PART_ask_part_name(part, cpn));  
        build_similar_temp_filespec(cpn, 164, ifn);  
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
    /*  Done the entire part, now do just selected geometry 里海译:已完成整个部分，现在只选择进行几何体的操作。 */  
        while (((n = select_objects("Select objects to export", &objects)) > 0) &&  
            !UF_CALL(UF_UI_create_filebox("Specify STEP203 file to export into",  
                "STEP203 file", filter, "", ofn, &resp)) && (resp != UF_UI_CANCEL))  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数执行失败时打印错误信息。
> 2. 对象选择函数：定义了一个对象选择函数select_objects，用于提示用户选择要导出的对象，并返回选择的数量和对象数组。
> 3. 临时文件生成函数：定义了一个临时文件生成函数build_similar_temp_filespec，用于根据输入的文件名生成临时文件路径。
> 4. 主体函数：do_it函数实现了主要功能，包括：保存当前激活的部件，获取其名称生成临时输入和日志文件路径调用系统命令执行UG的step203ug转换程序，将整个部件转换成STEP203格式循环提示用户选择要导出的对象，生成临时文件，调用UF_PART_export将对象数组导出到临时文件，然后执行系统命令转换成STEP203格式最后删除临时文件
> 5. 保存当前激活的部件，获取其名称
> 6. 生成临时输入和日志文件路径
> 7. 调用系统命令执行UG的step203ug转换程序，将整个部件转换成STEP203格式
> 8. 循环提示用户选择要导出的对象，生成临时文件，调用UF_PART_export将对象数组导出到临时文件，然后执行系统命令转换成STEP203格式
> 9. 最后删除临时文件
> 10. 入口函数：ufusr函数是程序的入口，初始化环境后调用do_it函数，最后结束环境。
> 11. 卸载函数：ufusr_ask_unload函数用于处理程序卸载时的一些清理工作。
>
> 这段代码实现了将NX部件或用户选择的对象导出为STEP203格式的功能，通过调用系统命令调用UG自带的转换程序实现。
>
