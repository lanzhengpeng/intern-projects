### 【3109】export selected geometry to dxfdwg 将选定的几何体导出为DXFDWG格式的文件

#### 代码

```cpp
    /*HEAD EXPORT_SELECTED_GEOMETRY_TO_DXFDWG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
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
        int  
            n,  
            resp;  
        tag_t  
            *objects;  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            *dfn,  
            filter[MAX_FSPEC_SIZE+1] = { "*.dxf" },  
            lfn[MAX_FSPEC_SIZE+1],  
            ofn[MAX_FSPEC_SIZE+1],  
            *prog,  
            *shell,  
            pfn[MAX_FSPEC_SIZE+1];  
    #if defined(_WIN32)  
        prog = "%UGII_BASE_DIR%\\DXFDWG\\dxfdwg.cmd";  
        dfn = "%UGII_BASE_DIR%\\DXFDWG\\dxfdwg.def";  
        shell = "cmd.exe /c";  
    #else  
        prog = "$UGII_BASE_DIR/dxfdwg/dxfdwg";  
        dfn = "$UGII_BASE_DIR/dxfdwg/dxfdwg.def";  
        shell = "/bin/ksh -c";  
    #endif  
        while (((n = select_objects("Select objects to export", &objects)) > 0) &&  
            !UF_CALL(UF_UI_create_filebox("Specify DXF/DWG file to export into",  
                "DXF/DWG file", filter, "", ofn, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            build_similar_temp_filespec(ofn, 47, lfn);  
            build_similar_temp_filespec(ofn, 2, pfn);  
            sprintf(cmd, "%s \"\"%s\" o=\"%s\" d=\"%s\" l=\"%s\" \"%s\"\"",  
                shell, prog, ofn, dfn, lfn, pfn);  
            if (!UF_CALL(UF_PART_export(pfn, n, objects)))  
            {  
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

> 这段代码是用于NX的二次开发，实现了将NX中选定的对象导出为DXF/DWG格式的功能。具体来说：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API函数出错时打印错误信息。
> 2. 选择对象函数：定义了一个选择对象函数select_objects，用于弹出一个选择对话框，让用户选择要导出的对象，并返回对象数组。
> 3. 构建临时文件名函数：定义了一个构建临时文件名的函数build_similar_temp_filespec，用于根据指定的文件名和文件类型构建一个临时文件名。
> 4. 导出操作函数：定义了一个导出操作函数do_it，用于实现导出流程：调用select_objects获取要导出的对象数组弹出文件对话框，让用户指定导出的DXF/DWG文件名构建导出的临时文件名调用NX的UF_PART_export函数将对象数组导出为NX内部格式的临时文件调用系统命令，使用NX提供的dxfdwg工具将临时文件转换为DXF/DWG格式删除临时文件
> 5. 调用select_objects获取要导出的对象数组
> 6. 弹出文件对话框，让用户指定导出的DXF/DWG文件名
> 7. 构建导出的临时文件名
> 8. 调用NX的UF_PART_export函数将对象数组导出为NX内部格式的临时文件
> 9. 调用系统命令，使用NX提供的dxfdwg工具将临时文件转换为DXF/DWG格式
> 10. 删除临时文件
> 11. ufusr主函数：定义了ufusr主函数，用于初始化NX环境，调用do_it函数执行导出操作，然后终止NX环境。
> 12. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载，表示该模块不需要延迟卸载。
>
> 综上所述，这段代码通过封装NX API，实现了将NX模型导出为DXF/DWG格式的功能。
>
