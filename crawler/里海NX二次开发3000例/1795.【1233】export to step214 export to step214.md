### 【1233】export to step214 export to step214

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
            filter[MAX_FSPEC_SIZE+1] = { "*.stp" },  
            ifn[MAX_FSPEC_SIZE+1],  
            lfn[MAX_FSPEC_SIZE+1],  
            ofn[MAX_FSPEC_SIZE+1],  
            *prog,  
            *shell,  
            pfn[MAX_FSPEC_SIZE+1];  
    #if defined(_WIN32)  
        prog = "%UGII_BASE_DIR%\\STEP214UG\\step214ug.cmd";  
        dfn = "%UGII_BASE_DIR%\\STEP214UG\\ugstep214.def";  
        shell = "cmd.exe /c";  
    #else  
        prog = "$UGII_BASE_DIR/step214ug/step214";  
        dfn = "$UGII_BASE_DIR/step214ug/ugstep214.def";  
        shell = "/bin/ksh -c";  
    #endif  
    /*  This does the whole part first - just to show how that works 里海译:翻译如下：

这首先处理了整个部分 - 只是为了展示那是如何工作的。 */  
    /*  the translator reads the last saved part not the active one 里海译:根据你的指示，翻译如下：

The translator reads the last saved part, not the active one. */  
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
    /*  Done the entire part, now do just selected geometry 里海译:翻译已完成整部分，现在只进行选择的几何图形处理。 */  
        while (((n = select_objects("Select objects to export", &objects)) > 0) &&  
            !UF_CALL(UF_UI_create_filebox("Specify STEP214 file to export into",  
                "STEP214 file", filter, "", ofn, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            build_similar_temp_filespec(ofn, 47, lfn);  
            build_similar_temp_filespec(ofn, 2, pfn);  
            sprintf(cmd, "%s \"\"%s\" o=\"%s\" d=\"%s\" l=\"%s\" \"%s\"\"",  
                shell, prog, ofn, dfn, lfn, pfn);  
            if (!UF_CALL(UF_PART_export(pfn, n, objects)))  
            {  
                ECHO("%s\n", cmd);  
                putenv("UGII_UGMGR_EXTERNAL_COMMS_LIB=");  
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

> 这段代码是一个NX Open的UF程序，主要用于导出NX部件到STEP 214格式文件。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于报告UF函数调用错误。
> 2. 定义了一个选择对象的函数select_objects，用于让用户选择要导出的几何体。
> 3. 定义了一个生成临时文件名的函数build_similar_temp_filespec。
> 4. do_it函数首先导出整个部件到STEP文件，然后循环让用户选择要导出的几何体，并将这些几何体导出到新的STEP文件。
> 5. ufusr是程序的入口函数，用于初始化和执行do_it。
> 6. ufusr_ask_unload函数用于卸载程序。
>
> 总的来说，这是一个简单的NX二次开发程序，实现了导出NX部件到STEP格式的功能。代码结构清晰，注释详细，易于理解和使用。
>
