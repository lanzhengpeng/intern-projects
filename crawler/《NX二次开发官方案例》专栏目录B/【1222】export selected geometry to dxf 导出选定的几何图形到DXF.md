### 【1222】export selected geometry to dxf 导出选定的几何图形到DXF

#### 代码

```cpp
    /*HEAD EXPORT_SELECTED_GEOMETRY_TO_DXF CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文内容，UF_print_syslog是V18版本中新增的功能。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            *pfn,  
            filter[MAX_FSPEC_SIZE+1] = { "*.dxf" },  
            lfn[MAX_FSPEC_SIZE+1],  
            ofn[MAX_FSPEC_SIZE+1],  
            tfn[MAX_FSPEC_SIZE+1];  
    #if defined(_WIN32)  
        pfn = "%UGII_BASE_DIR%\\UGTODXF\\ugtodxf";  
    #else  
        pfn = "$UGII_BASE_DIR/ugtodxf/ugtodxf";  
    #endif  
        while (((n = select_objects("Select objects to export", &objects)) > 0) &&  
            !UF_CALL(UF_UI_create_filebox("Specify DXF file to export into",  
                "DXF file", filter, "", ofn, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            build_similar_temp_filespec(ofn, 47, lfn);  
            build_similar_temp_filespec(ofn, 2, tfn);  
            sprintf(cmd, "%s i=%s o=%s d=ugtodxf.def l=%s", pfn, tfn, ofn, lfn);  
            if (!UF_CALL(UF_PART_export(tfn, n, objects)))  
            {  
                WRITE_S(cmd);  
                system(cmd);  
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

> 根据代码内容，这是一段用于在NX中导出选中几何体到DXF格式的二次开发代码。具体功能如下：
>
> 1. 定义了错误处理函数report_error，用于捕获并报告函数调用过程中的错误。
> 2. 定义了选择对象函数select_objects，用于提示用户选择要导出的对象，并获取选择的几何体数组。
> 3. 定义了构建临时文件路径函数build_similar_temp_filespec，用于生成临时文件路径。
> 4. 定义了写入信息到列表窗口的函数write_string_to_listing_window。
> 5. 定义了主要处理函数do_it，用于循环提示用户选择对象，并调用NX的导出函数UF_PART_export将对象导出到临时文件，然后调用系统命令执行转换脚本，将临时文件转换为DXF格式。
> 6. 在ufusr函数中初始化NX环境，调用do_it函数进行导出处理，然后终止NX环境。
> 7. 定义了卸载函数ufusr_ask_unload，用于在卸载用户函数时立即释放资源。
>
> 综上所述，这段代码通过交互式选择和转换脚本调用，实现了在NX中将选中几何体导出为DXF格式的功能。
>
